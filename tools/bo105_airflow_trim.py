"""Engine airflowTable trim against the OGE chart. From HeliSim Core's python/dev, with BMKHS_CONFIG set:
    python <this file> <Core python/dev> solve            - need / delivered per engine-limited cell
    python <this file> <Core python/dev> check table.json - fly every cell with a {FAT, multiplier} table
"""
import sys, json
sys.path.insert(0, sys.argv[1])
import airframe as A
E = A.E

ALTS = [0, 2000, 4000, 6000, 8000, 10000]
CHART = {-40: [2500, 2500, 2500, 2500, 2475, 2280], -20: [2500, 2500, 2500, 2500, 2420, 2230],
         0: [2500, 2500, 2500, 2490, 2290, 2110], 20: [2500, 2500, 2500, 2300, 2120, 1950],
         40: [2500, 2450, 2260, 2080, 1930, 1820]}
TOT = 810.0

def engines():
    return [E.CFG['Engines'][k] for k in ('Engine01', 'Engine02') if k in E.CFG['Engines']]

def set_engine(table):
    for e in engines():
        e['Combustor']['maxTgt'] = TOT
        e['Combustor']['maxTgtSe'] = TOT
        e['Compressor']['ngLimitMax'] = 1.05
        e['Compressor']['ngLimitBase'] = 1.05
        e['Compressor']['ngLimitSlope'] = 0.0
        e['Compressor']['airflowTable'] = [list(r) for r in table]

af = A.Airframe(bc=(0, 1.248, -0.108), cg=(0, 1.096, -0.218))

def needed(w, pa, fat):
    r = A.trim(af, 0.0, w, A.density(pa, fat), pa, solveCyc=True, guess=(0, 0.6, 0))
    return r['tq'] if r['ok'] else None

table = [[f, 1.0] for f in (-40, -30, -20, -10, 0, 10, 15, 20, 30, 40)]
mode = sys.argv[2] if len(sys.argv) > 2 else 'solve'
if mode == 'check':
    table = json.load(open(sys.argv[3]))
set_engine(table)

ratios = {}
for fat, ws in CHART.items():
    for pa, w in zip(ALTS, ws):
        if w >= 2500:
            continue
        need = needed(w, pa, fat)
        r = E.max_power(pa, fat, False)
        avail, tgt, ng, why = (r[0], r[1], r[2], r[4]) if r else (None, None, None, '-')
        lim = 'rotor' if need is not None and need >= 0.855 else 'TOT'
        print('%4d C %5d ft %4d kg: need %5.1f%%  engine %5s%% at %4s C N1 %s held %s  -> %s' % (
            fat, pa, w, need * 100 if need else -1, '%.1f' % (avail * 100) if avail else '-',
            '%.0f' % tgt if tgt else '-', '%.3f' % ng if ng else '-', why, lim), flush=True)
        if mode == "solve" and need and lim == "TOT" and avail and why == "TGT":
            ratios.setdefault(fat, []).append(need / avail)

if mode == 'solve':
    print()
    for fat, rs in sorted(ratios.items()):
        print('%4d C: ratios %s  mean %.4f' % (fat, ' '.join('%.3f' % r for r in rs), sum(rs) / len(rs)))
