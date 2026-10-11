# Bo 105 flight model - how every value was derived

This pack started as a copy of the EC665 Tiger pack. This document records how each Tiger value
was replaced with the Bo 105's: the source, the conversion, and the command that reproduces it.
Read it before changing a value, and update it when you do.

Core's own procedures are in HeliSim Core's `docs/AIRCRAFT_GUIDE.md`; this file says how they
were applied to the Bo 105 and where the Bo 105 departs from them.

Contents:

1. [Frames and the source model](#1-frames-and-the-source-model)
2. [Aerosurfaces - fm.p3d](#2-aerosurfaces---fmp3d)
3. [Tuning positions in game - the nudge](#3-tuning-positions-in-game---the-nudge)
4. [Reading GM's Bo 105 - variants, seats, pylons](#4-reading-gms-bo-105---variants-seats-pylons)
5. [Force coefficient override](#5-force-coefficient-override)
6. [Mass and balance](#6-mass-and-balance)
7. [Fuel](#7-fuel)
8. [Rotors](#8-rotors)
9. [Main rotor tables - the fit](#9-main-rotor-tables---the-fit)
10. [Engine - Allison 250-C20B](#10-engine---allison-250-c20b)
11. [Engine and rotor limits](#11-engine-and-rotor-limits)
12. [Flight controls](#12-flight-controls)
13. [Still to do](#13-still-to-do)

---

## 1. Frames and the source model

Three frames are in play. Core's guide ("Mass and balance - converting the CG") defines the
first and last; the middle one is this pack's.

| Frame | What it is |
|---|---|
| **Model (Object Builder)** | GM's Bo 105 p3d. x right, y toward the nose, z up. Every `arm[]`, `fsDatum` and CG limit is in this frame. |
| **Source model** | The 3D model `fm.p3d` was built from. It is turned 180 deg about the vertical axis from GM's (its nose points at -y) and offset from it. |
| **Arma** | Model frame minus `boundingCenter`. GM's Bo 105: `boundingCenter = [0, 1.248, -0.108]` (PAH 1; -0.110 VBH, -0.133 Swooper). Rotor `pivot[]`, `comCorrection` and `casualModeCom` are in this frame. |

**Source model -> model frame:** turn 180 deg about vertical, then shift 0.300 m forward and
0.650 m down:

    model y = -source y + 0.300
    model z =  source z - 0.650
    model x = -source x

A distance measured "from the fm model origin" toward the nose of the source model becomes
`model y = distance + 0.300`. Every datum, CG limit and tank arm below was converted this way.

The offset was found in game with the nudge (section 3), not measured.

---

## 2. Aerosurfaces - fm.p3d

`addons/helisim/fm.p3d` is the source of `helisim_fuselage.hpp` and `helisim_wings.hpp`, built
as Core's guide describes ("The fuselage and wings - model them, do not type them").

fm.p3d is in the source model frame, so Core's generator places it as it reads it - turned
180 deg, then shifted (its `--turn`, `--forward`, `--down`, `--right` and `--add`, Core's
guide step 3 section 5). From HeliSim Core's `python/`:

    python tools\fm_generateAeroSurfacePoints.py C:\GitHub\BradMicks-HeliSim-Bo105\addons\helisim\fm.p3d C:\GitHub\BradMicks-HeliSim-Bo105\addons\helisim\config\bmkhs_config --turn --forward 0.300 --down 0.650

The current files record their placement in the old wrapper's format, which Core's `--add` does
not read - so the next regeneration gives the full placement above. After that, `--add` adds to
it.

It rewrites **both** files in full. After every regeneration:

1. **Put the front drag back.** The generator writes its default front table (0.800 rising to
   3.000). The fitted value is **0.357, flat at every altitude** (section 9).
2. Rerun the rotor fit (section 9) if the geometry changed - the front drag is fitted against
   the airframe.

Problems the generator reports, and their fixes, are in Core's guide. Two that came up here:

- `X must be exactly one 4-sided face` - the selection held stray vertices from a neighbouring
  quad. Reselect only the face.
- `you didn't name X correctly` - a selection named `fuselageTop04_1`. Numbers only:
  `fuselageTop08`.

The horizontal stabilizer is modelled facing **down** (lift downward, holding the tail down).

---

## 3. Tuning positions in game - the nudge

HeliSim Core's `tools/fm_nudge.sqf` moves the aerosurfaces, each rotor hub and the centre of
mass live. Its keys and what it copies are in Core's guide, step 3 section 7. For fuselage +
wings it copies Core's generator `--add` command (section 2 above).

---

## 4. Reading GM's Bo 105 - variants, seats, pylons

GM's config is encrypted on disk, so it is read in game.

**HeliSim Core's `tools/dump.sqf`**, run sitting in a Bo 105 - every variant's turrets, weapons,
magazines, pylons and animation sources, to the RPT (`BMKHSDUMP|`) and the clipboard. Results:

| Variant | Class (parent `<class>_base`) | Seats |
|---|---|---|
| PAH 1 / PAH 1A1 | `gm_ge_army_bo105p_pah1`, `..._pah1a1` | Pilot, gunner `[0]` |
| VBH 1, VBH 1A1, BGS, Police, ADAK | `gm_ge_army_bo105m_vbh`, `..._bo105p1m_vbh`, `gm_ge_bgs_bo105m_vbh`, `gm_ge_pol_bo105m_vbh`, `gm_ge_adak_bo105m_vbh` | Pilot, copilot `[0]`, rear seats `[1]`-`[4]` |
| VBH 1A1 Swooper | `gm_ge_army_bo105p1m_vbh_swooper` | Pilot, copilot `[0]`, benches `[1]`-`[4]`, rear seats `[5]`-`[8]` |

- No variant has guns or internal magazines.
- The PAHs carry HOT on six pylons, all fired from turret `[0]`. PAH 1: HOT 1
  (`gm_1Rnd_hot_heat_dm72_pylon`); PAH 1A1: HOT 2 (`..._dm102_pylon`). The flag and Hornet
  presets are paint.
- GM's magazine `mass` (300) is Arma inventory mass, not kg.
- The only equipment-like animation sources are the Swooper's `swooperBenches_unhide` and
  `swooperRopes_unhide`.

**HeliSim Core's `tools/seats.sqf`** - puts a temporary soldier in every empty seat, logs each
seat's and pylon's position in the model frame (the seat at the occupant's pelvis), then removes
them. RPT (`BMKHSSEATS|`) and clipboard. It waits for game time, so **close the pause menu** after
running it. Results are in section 6.

---

## 5. Force coefficient override

`BMKHS_FM_OVERRIDE` zeroes Arma's own rotor forces. GM sets those coefficients again below
`gm_bo105_base`, so `cfgVehicles.hpp` reopens every variant under its parent and applies the
override there too (the `BO105_VARIANT` macro). Check it in game:

    copyToClipboard str (["liftForceCoef","cyclicForwardForceCoef","cyclicAsideForceCoef","backRotorForceCoef","bodyFrictionCoef"] apply {getNumber (configOf vehicle player >> _x)})

It must give `[0,0,0,0,0]`.

The mouse still flies the aircraft afterwards, by design: Core reads Arma's helicopter cyclic
actions, which Arma binds to the mouse by default. Unbind the mouse from them, or turn on
"Mouse as Joystick" in HeliSim's CBA settings. A mouse move arrives as a one-frame full
deflection, so tune control authority with a stick.

The Swooper is reopened with its own full `BMKHS_HeliSim` (`#define BO105_SWOOPER`), because its
turret paths `[1]`-`[4]` are different seats (section 6).

---

## 6. Mass and balance

`helisim_mass.hpp`. Conversions per Core's guide, "Mass and balance - converting the CG".

| Value | Source | Model frame |
|---|---|---|
| `fsDatum` | 5.145 m forward of the source model origin | 5.145 + 0.300 = **5.445** |
| `fwdCgLimit` | 2.07 m from the source model origin | **2.370** (FS 3.075 m) |
| `aftCgLimit` | 1.75 m from the source model origin | **2.050** (FS 3.395 m) |
| `emptyMass` | **1675 kg** | - |
| Empty CG | 0.03 m aft of the CG range's midpoint (2.210) | **2.180** |
| `emptyMom` | `1675 x (5.445 - 2.180)` | **5468.875** |
| `maxGrossMass` | Bo 105 maximum gross mass | **2500** |

The CG range runs from the main rotor hub (model y 2.058) to 0.31 m forward of it.

**Empty mass.** The published PAH-1 empty mass is 1,913 kg
([GlobalSecurity](https://www.globalsecurity.org/military/world/europe/bo105-specs.htm),
[helis.com](https://www.helis.com/database/model/82)). With two crew, full fuel and six HOTs
that came to 2,761 kg, over the maximum, so 1,675 kg is used. Fully loaded it is now 2,523 kg.

**`comCorrection = {0.000, 0.000, -0.326}`.** Only the vertical part is a correction. A forward
part was nudged in while the empty mass was still the Tiger's, and it put the CoM past the
forward limit once the masses were real.

**`casualModeCom = {0.000, 0.810, 0.975}`** (Arma frame), the Tiger's pattern: y under the main
rotor hub (its pivot y), z at the tail rotor's height (its pivot z), so pedal makes no roll.

**Seats** - measured with the seat tool (now HeliSim Core's `tools/seats.sqf`), 100 kg each:

| Seat | Turret | arm[] |
|---|---|---|
| Pilot | - | {0.364, 3.433, -1.022} |
| Copilot / gunner | `[0]` | {-0.375, 3.400, -1.020} - between VBH 3.468 and PAH 3.332 |
| Rear left / right | `[1]` / `[2]` | {-0.394, 2.538, -0.899} / {0.354, 2.544, -0.937} |
| Rear left / right outside | `[3]` / `[4]` | {-0.742, 2.816, -1.161} / {0.756, 2.851, -1.166} |

GM puts the copilot and passengers in turrets, so every seat but the pilot is `role = "turret"`
with its turret path.

Swooper (`#ifdef BO105_SWOOPER`): pilot {0.364, 3.432}, copilot `[0]` {-0.375, 3.468}, benches
`[1]`-`[4]` {-1.062, 2.254}, {1.049, 2.252}, {-1.028, 1.614}, {1.051, 1.622}, rear seats
`[5]`-`[8]` as the other variants' `[1]`-`[4]`.

**HOT** - two stations, the three-tube launchers, at their middle tubes (pylons measured at model
y 2.015, x +-1.367 / 1.614 / 1.860):

| | Value | Source |
|---|---|---|
| Station01 | pylons 1, 3, 5, arm {-1.614, 2.015, -1.153} | measured |
| Station02 | pylons 2, 4, 6, arm {1.614, 2.015, -1.153} | measured |
| Store `match` | `"hot_heat"` - HOT 1 and HOT 2 | GM magazine names |
| `massPerRound` | 23.5 kg, the missile | [GlobalSecurity](https://www.globalsecurity.org/military/world/europe/hot-specs.htm) |
| `launcherMass` | 25.5 kg, three 8.5 kg tubes (32 kg in tube - 23.5 kg) | [armsnet](https://armsnet.duckdns.org/equipment/e57371e1-beb1-473b-ef63-08de6e3c1543) |

The tube stays after firing, so it is launcher mass. The launcher frame's own mass is not known.

---

## 7. Fuel

`helisim_fuel.hpp`.

| Tank | Capacity | Source arm | Model arm |
|---|---|---|---|
| `supplyTank` | 74 kg | 2.87 m | {0, 3.170, 0} |
| `mainTank` | 382 kg | 1.91 m | {0, 2.210, 0} |

The engines draw from the supply tank, which the main tank keeps full. Low fuel when the total
is less than the supply tank: `lowFuelKg = 74` on the supply tank.

**Core workaround.** Core moves fuel tank to tank only between two `"main"` tanks
(`fn_fuelTransfer.sqf` exits with fewer); a single main fed by an `"xfer"` tank is never
refilled. So:

- both tanks are `role = "main"`;
- the one crossfeed mode feeds both engines from `supplyTank` only;
- `xferDestinations[] = {"SUPPLY", "MAIN"}`, and `fn_setup.sqf` sets
  `bmkhs_xferMode = "SUPPLY"` after Core's config load, so the pump always fills the supply tank.
  The pump (0.378 kg/s) outruns both engines' burn.

---

## 8. Rotors

`helisim_simpleRotor.hpp`.

| Value | Main | Tail | Source |
|---|---|---|---|
| `direction` | `"ccw"` | `"ccw"` | Bo 105 main rotor turns counter-clockwise |
| `numBlades` | 4 | **2** | Bo 105 |
| `bladeRadius` | 4.920 | 0.950 | 9.84 m / 1.90 m diameters |
| `bladeMass` | 25.0 kg | 1.49 kg | |
| Rotor rpm | 424 | 2219 | |
| `gearRatio` | 14.151 | 2.704 | 6000 rpm engine output / rotor rpm |
| `pivot[]` | {0.000, 0.810, 0.170} | {-1.000, -5.177, 0.975} | nudge (section 3) |
| `rollLiftCoef` | 0.166 | - | tuned in flight |
| `pitchLiftCoef` | 0.315 | - | tuned in flight |
| `nrLimits[]` | {0.98, 1.02, 1.05, 1.10} | | section 11 |

Tip speeds: main 218 m/s, tail 221 m/s.

**Counter-clockwise main rotor.** Left pedal carries the main rotor torque, so the tail **drag**
rows are swapped: highest drag at -1 (left). The tail **lift** rows keep the Tiger's sign
(negative at -1, positive at +1).

Do **not** copy the H-60's tail lift sign. The H-60 is also counter-clockwise and has positive
lift at left pedal, but its tail rotor is mounted the other way (`rotation[] = {0, 70, 0}`).
This pack keeps the Tiger's mounting (`{0, -90, 0}`), on which negating the lift reverses the
pedals: right pedal yaws left, and the auto-pedal and yaw SAS drive the aircraft into a spin.

**Tail rotor lift** is the Tiger's table, halved (+-1.1648 at full pedal, hover column) for too
much authority in flight. It has not been fitted.

**Cyclic authority** history, all tuned in flight: `pitchLiftCoef` 0.72 -> 0.36 -> 0.252 ->
0.315; `rollLiftCoef` 0.19 -> 0.133 -> 0.166. The two middle steps were partly against mouse
spikes (section 5), so the final values are what a stick wants.

---

## 9. Main rotor tables - the fit

The tables are solved by Core's `python/dev/rotortables.py`, as in Core's guide ("Fitting the
main rotor tables"). Its inputs came from two Bo 105 charts.

### 9.1 OGE hover - maximum gross mass at 86% torque, both engines, 810 C (takeoff power)

| FAT \ PA ft | 0 | 2000 | 4000 | 6000 | 8000 | 10000 |
|---|---|---|---|---|---|---|
| -40 | 2500 | 2500 | 2500 | 2500 | 2475 | 2280 |
| -20 | 2500 | 2500 | 2500 | 2500 | 2420 | 2230 |
| 0 | 2500 | 2500 | 2500 | 2490 | 2290 | 2110 |
| 20 | 2500 | 2500 | 2500 | 2300 | 2120 | 1950 |
| 40 | 2500 | 2450 | 2260 | 2080 | 1930 | 1820 |

Each cell below 2,500 kg was converted to the sea level, 15 C weight needing the same torque.
Hover induced power goes as W^1.5 / sqrt(sigma), so `W_SL = W x sigma^(1/3)`, with
`sigma = (1 - 6.8756e-6 h)^5.2559 / ((FAT + 273.15) / 288.15)`.

The cells do not collapse to one weight: the hot and high ones are cut short by the 810 C TOT
before they reach 86% torque. The rotor is set by the upper envelope:

| Cell | sigma | W_SL |
|---|---|---|
| -40 C, 8,000 ft | 0.918 | 2,405 kg |
| 0 C, 6,000 ft | 0.845 | 2,354 kg |
| Extended to sigma 1.0 | | **2,460 kg at 86%** |

Other hover points use the Tiger's torque-weight relationship, `TQ ~ W^1.3`
(`TQ = 86 x (W / 2460)^1.3`):

| Weight | Torque |
|---|---|
| 1,950 kg | 63.6% (fitting weight, section 9.2) |
| 2,460 kg | 86% |
| 3,179 kg | 120% (the peak) |

**The peak sets the rotor's stall**, and stall thrust falls with density. A peak of 2,763 kg at
100% (the first fits) stalls at 2,177 kg at 6,000 ft, 20 C - below the chart's 2,300 kg - so
every hot and high cell was past the stall. 120% (3,179 kg) clears every chart cell (2,012 kg
available at sigma 0.633 against 1,820 needed). A blade loading estimate (CT/solidity 0.12,
solidity 0.07) puts the real rotor's sea level stall near 3,800 kg.

### 9.2 Maximum airspeed - 1,950 kg, ISA, 86% torque, 779 C

| PA ft | SL | 2000 | 4000 | 6000 | 8000 | 10000 |
|---|---|---|---|---|---|---|
| kt | 133 | 135 | 137 | 136 | 134 | 132 |

On Core's standard power curve, 86% at 133 kt over a 63.6% hover puts max range at
**118.4 kt** (`86 / 63.6 = 1.352` falls at `V / Vmr = 1.123`). The altitude columns are not used
- the fit is sea level only.

### 9.3 Estimates (no data)

| Target | Value |
|---|---|
| Max endurance | 60 kt, 38.2% (0.6 x the hover torque) |
| Cruise attitude | 5 deg nose low at max range - solves the front fuselage drag |

### 9.4 Fitting CoM

At 1,950 kg: two crew, 75 kg fuel (74 supply + 1 main), no HOT.

    longCG = (1675 x 2.180 + 200 x 3.4165 + 74 x 3.170 + 1 x 2.210) / 1950 = 2.344
    CoM (Arma) = [0, 2.344 - 1.248 + 0, 0 + 0.108 - 0.326] = [0, 1.096, -0.218]

### 9.5 Flat-pitch drag

The fitter keeps the pack's collective-0 drag row. The Tiger's (0.0078) read 30% per engine on
the Bo 105's smaller 100% torque. It is scaled x0.471 to **0.0037**, which gives 14.1% main +
4.1% tail = **18.2% per engine** at collective 0, pedals centred, 100% Nr (rig).

Target 10-20% per engine. No Bo 105 figure was found; the Bell 206, on the same Allison 250
family, reads about 28-30% of its one engine at flat pitch
([Martin Woodside's 206B checklist](http://martinwoodside.blogspot.com/2009/12/bell-helicopter-checklist-bell-206b.html);
40% is called high on [PPRuNe](https://www.pprune.org/rotorheads/263656-b206-torque.html)),
which split over two engines is 15-20%.

Change this row only before a fit - the power curve depends on it.

### 9.6 The command

From HeliSim Core's root:

    set BMKHS_CONFIG=C:\GitHub\BradMicks-HeliSim-Bo105\addons\helisim\config\bmkhs_config
    python python/dev/rotortables.py --bc 0 1.248 -0.108 --cg 0 1.096 -0.218 ^
        --hover 1950:63.6 2460:86 3179:120 --me 60:38.2 --mr 118.4 --cruise-pitch -5

Paste both tables into `SimpleRotor01` and write the printed front drag into
`helisim_fuselage.hpp`, flat at every altitude. The run must end `Fit OK`.

### 9.7 Result

Rig, 1,950 kg, sea level, 15 C:

| Condition | Torque | Collective | Pitch |
|---|---|---|---|
| OGE hover | 63.5% | 0.475 | -0.7 |
| 60 kt (bucket) | 38.1% | 0.228 | -1.4 |
| 118.4 kt (max range) | 63.5% | 0.497 | -5.0 |
| 135.3 kt | 89.5% | 0.654 | -6.2 |
| OGE hover 2,460 kg | 86.0% | 0.629 | |
| OGE hover 3,179 kg (peak) | 120.2% | 0.851 | |

133 kt lands at 86%. Front fuselage drag **0.357**. Check any speed directly:

    python -c "import airframe as A; af=A.Airframe(bc=(0,1.248,-0.108),cg=(0,1.096,-0.218)); print(A.trim(af,133*0.514444,1950,A.density(0,15),0,solveCyc=True,guess=(-5,0.7,0)))"

(from `python/dev`, with `BMKHS_CONFIG` set). Game torque has read 2-4% above the rig before.

---

## 10. Engine - Allison 250-C20B

`helisim_engine.hpp`, per Core's guide, "The engines". `Engine02` inherits from `Engine01`.

| Value | Bo 105 | Source / reason |
|---|---|---|
| `powerKw` | 313 | 420 shp |
| `designRpm` | 6000 | 100% N2 (Bo 105 CB-5 manual: 6,000 rpm engine shaft, 424 rotor) |
| `pressureRatio` | 7.2 | 250-C20B |
| `massFlow` | 1.56 kg/s | see below |
| `maxFuelFlow` | 0.0161 | Tiger's 0.033 x 1.56 / 3.2 - same heat per kg of air |
| `fuelIdle` | 0.521 | from the map tool: Ng 0.679, 424 C, 2.6% torque |

**Why the engine had to be rebuilt.** With the Tiger's MTR390 (3.2 kg/s, PR 13) and the Bo 105's
313 kW, the engine at idle fuel made 12% of the Bo 105's 100% torque. Core's sprag clutch only
releases when the free turbine cannot keep up with the rotor, so in an autorotation Np followed
Nr (to 115%) and the clutch never released. The resized engine makes 2.6% at idle, below the
turbine's drag.

**Why 1.56 kg/s and not 1.0.** At 1.0 kg/s the map tool lands both ratings, but only by
stretching the airflow to 1.48x in the rating rows while the rows below stay on the T700 shape
- a step from 0.815 to 1.482 between N1 89% and 95%, a torque surge in the flying range. At
1.56 kg/s the map is smooth.

**Compressor map** - Core's `python/tools/compressor_map.py` with `tools/bo105_ratings.txt`:

    #name  FAT_C  N1     T45_C  torque
    MCP    15     0.950  779    0.860
    TOP    15     0.968  810    0.950

- N1 is the T700's for each rating, as the Tiger's was. The torque and TOT are the Bo 105's:
  MCP 86% at 779 C (both engines); 95% at 810 C is the single-engine emergency rating, the only
  810 C torque in the manual tables used.
- Fill in the spec numbers first, then from HeliSim Core's root:

      python python/tools/compressor_map.py C:\GitHub\BradMicks-HeliSim-Bo105\addons\helisim\config\bmkhs_config C:\GitHub\BradMicks-HeliSim-Bo105\tools\bo105_ratings.txt

- Paste `compressorMap[]` into `class Compressor` and set `fuelIdle` to what it reports. Both
  ratings must land on their N1, T45 and torque.

**N1 limiter** - flat at the 250-C20B's 105%: `ngLimitMax = ngLimitBase = 1.05`,
`ngLimitSlope = 0`.

### Airflow trim - the OGE chart

The Tiger's method: trim `airflowTable` per FAT so the OGE chart's engine-limited cells fall on
the rotor, with 15 C held at 1.0 (a standard day is the untrimmed engine, and keeps MCP at 86%
and 779 C). HeliSim Core's `python/dev/airflow_trim.py`, with the chart in
`tools/bo105_oge_chart.txt` and `BMKHS_CONFIG` set to this pack's `bmkhs_config`, from Core's root:

    python python/dev/airflow_trim.py solve --chart C:\GitHub\BradMicks-HeliSim-Bo105\tools\bo105_oge_chart.txt --tot 810 --rotor-tq 0.855 --bc 0 1.248 -0.108 --cg 0 1.096 -0.218

For every chart cell under 2,500 kg it trims the rotor rig in an OGE hover at that weight, PA
and FAT (the torque **needed**), and flies the engine rig to its limit with the TOT limiter at
810 C, takeoff power (the torque **delivered**). A cell needing 86% or more is rotor limited
and is skipped; the rest give `needed / delivered`.

| FAT | Cells | Held by | Trim |
|---|---|---|---|
| -40 C | 10,000 ft | N1 105% | 1.140 |
| -20 C | 10,000 ft | N1 105% | 1.046 |
| 0 C | 8,000, 10,000 ft | N1 105% | 0.971 |
| 15 C | - | - | 1.000 (held) |
| 20 C | 6,000-10,000 ft | TOT 810 | 1.136 |
| 40 C | 4,000-10,000 ft | TOT 810 | 1.288 |

-30, -10 and 10 C are interpolated. Check a table by flying every cell with it:

    python python/dev/airflow_trim.py check table.json --chart ... (the same options)

where `table.json` is `[[-40,1.140],[-30,1.093],...]`. Every engine-limited cell lands within
1.5% of the rotor's need.

The trim is per FAT only. Within a row the needed ratio falls about 3% from 6,000 to 10,000 ft
(the engine lapses with altitude a little faster than the rotor needs), and the mean is used.
The rotor-limited cells need 3-6% more than the chart's 86% at altitude, because the rotor fit
is sea level only.

---

## 11. Engine and rotor limits

From the Bo 105 manual's main-section tables (Model C variants - the PAH-1 is a Bo 105 C
derivative). Core's limit format is `{limit, seconds allowed above it, damage divisor}`; 0 s
damages at once.

| Config | Value | Manual |
|---|---|---|
| `nrLimits[]` | {0.98, 1.02, 1.05, 1.10} | Nr min continuous 98, max continuous 102, transient 105, power off 110 |
| `tqLimits[]` | {{0.86, 0, 5}} | Both engines: MCP and takeoff 2 x 86, red radial |
| `tqLimitsSe[]` | {{0.95, 0, 5}} | One engine: MCP and emergency 95 |
| `tgtLimits[]` | {{779, 300, 1000}, {810, 6, 2000}, {843, 0, 2000}} | MCP 779; takeoff 779-810 for 5 min; 843 transient 6 s |
| `tgtLimitsSe[]` | {{779, 1800, 1000}, {810, 6, 2000}, {843, 0, 2000}} | Emergency 779-810 for 30 min |
| `ngLimits[]` | {{1.05, 15, 10}, {1.06, 0, 20}} | N1 105, transient 106 for 15 s |
| `npLimits[]` | {{1.02, 15, 10}, {1.05, 0, 20}} | N2 102, transient 105 for 15 s |
| `maxTgt`, `maxTgtSe` | 843 | TOT limiter |
| `startTgt` | 927 | Start transient |
| `oilPsiLimits[]` | {0.385, 1.000} | 3.5-9.1 kp/cm2; Core's oil is a gauge fraction, 9.1 = full scale |

Takeoff power with both engines is a TOT rating, not more torque: at sea level on a standard day,
86% is reached at about 779 C. The manual's appendix tables (OEI 105 / 110 / 123%) are for
another variant and are not used.

---

## 12. Flight controls

`helisim_flightControls.hpp`. The Tiger's gains made the auto-pedal bang between full left and
full right every frame: the Bo 105 has about half the tail thrust on a 0.74x arm and far less
yaw inertia. P and I halved, D cut harder:

| | Tiger | Bo 105 |
|---|---|---|
| `pidAutoPedalHdg` | {0.100, 0.0050, 0.050, 30} | {0.050, 0.0025, 0.0125, 30} |
| `pidAutoPedalNtt` | {0.030, 0.0080, 0.010, 18.75} | {0.015, 0.0040, 0.0025, 18.75} |
| `pidAutoPedalAero` | {0.60, 0.50, 0.40, 0.20} | {0.30, 0.25, 0.10, 0.20} |
| `Sas yaw` | {0.30, 0.050, 0.025, 0.05} | {0.15, 0.025, 0.006, 0.05} |

To read a flight, turn on Enable Flight Log and run Core's `python/dev/flightlog.py --csv out.csv`
on the RPT. `ftYaw` is the auto-pedal output; `sasY` the yaw SAS.

---

## 13. Still to do

- **Tail rotor tables** - the Tiger's, halved and mirrored for drag. Fit them to the hover pedal.
- **Rotor at altitude** - the fit is sea level; at altitude the rotor needs 3-6% more than the
  OGE chart's 86%.
- **Engine start and spool** - `Starter`, `spoolInertia`, `idleNg` are the Tiger's.
- **Overspeed trips** `maxNg`, `maxNp` - the Tiger's.
- **Max endurance and cruise attitude** in the fit are estimates (section 9.3).
- **HOT launcher frame mass** - only the tubes are counted.
- **Swooper benches and ropes** as `Equipment`, once their mass is known.
- **Vendored header** - the game loads HeliSim Core 1.3.0.0; `include/bmkhs_helisim/README.md`
  says 1.2.1.0. `fmOverride.hpp` is unchanged between them.
