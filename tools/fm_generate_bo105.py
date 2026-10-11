"""Writes helisim_fuselage.hpp and helisim_wings.hpp from fm.p3d, turned 180 deg and shifted.

    python tools/fm_generate_bo105.py [--forward M] [--down M] [--right M] [--add] [--core PATH]

--add adds to the shift recorded in the current files.
"""
import argparse
import importlib.util
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
P3D = os.path.join(ROOT, 'addons', 'helisim', 'fm.p3d')
CONFIG = os.path.join(ROOT, 'addons', 'helisim', 'config', 'bmkhs_config')
CORE = os.path.join(os.path.dirname(ROOT), 'BradMicks-HeliSim-Core', 'python', 'tools',
                    'fm_generateAeroSurfacePoints.py')
SHIFT = '//fm_generate_bo105.py shift: forward %.3f down %.3f right %.3f'

args = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
args.add_argument('--forward', type=float, default=0.0)
args.add_argument('--down', type=float, default=0.0)
args.add_argument('--right', type=float, default=0.0)
args.add_argument('--add', action='store_true')
args.add_argument('--core', default=CORE)
args = args.parse_args()

if args.add:
    m = re.search(r'shift: forward (\S+) down (\S+) right (\S+)',
                  open(os.path.join(CONFIG, 'helisim_fuselage.hpp')).read())
    if not m:
        print('Nothing written: helisim_fuselage.hpp has no recorded shift to add to')
        raise SystemExit(1)
    args.forward += float(m.group(1))
    args.down    += float(m.group(2))
    args.right   += float(m.group(3))

spec = importlib.util.spec_from_file_location('gen', args.core)
gen = importlib.util.module_from_spec(spec)
spec.loader.exec_module(gen)

read_lod0 = gen.read_lod0
def moved(path):
    pts, faces, sels = read_lod0(path)
    #MLOD points are (right, up, forward): turn 180 deg (negate right and forward), then shift.
    return [(-x + args.right, y - args.down, -z + args.forward) for x, y, z in pts], faces, sels
gen.read_lod0 = moved

try:
    built, report = gen.build(P3D)
except gen.BuildError as e:
    print('Nothing written: %s' % e)
    raise SystemExit(1)
for line in report:
    print(line)
for s in gen.FUSELAGE + gen.WINGS:
    if s in built:
        print('  %-22s %-8s %d quads' % (s, built[s][0], len(built[s][1])))
shift = SHIFT % (args.forward, args.down, args.right)
print('  ' + shift[2:])
for name, text in (('helisim_fuselage.hpp', gen.fuselage_hpp(built)), ('helisim_wings.hpp', gen.wings_hpp(built))):
    first, rest = text.split('\n', 1)
    with open(os.path.join(CONFIG, name), 'w', newline='\n') as f:
        f.write(first + '\n' + shift + '\n' + rest)
    print('wrote %s' % name)
