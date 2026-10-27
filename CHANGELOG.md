# Changelog

## 1.1.1.0

**Requires HeliSim Core 1.4.2.0.**

- Engine starts on Core 1.4.2.0: the starter's `runawayNg` is now `motoringNg` (dry crank 25%).
- The compressor map's rows below the fitted ratings rebuilt, so the rotor turns as Ng rises;
  idle fuel re-solved, idle unchanged.
- Start fuel set so the start runs like the AH-64D's: lever to IDLE, self-sustaining in about
  20-25 s, peak TGT about 620-660 C.

## 1.1.0.0

**Requires HeliSim Core 1.4.0.**

- Flies on HeliSim's new physics rotor: the disc tilts as one, flap back comes from the
  rotor itself, and per-blade inflow asks for forward cyclic as speed builds. Rotor tables
  and fuselage drag refit to the Bo 105's torque and attitude data.
- Roll and pitch rates tuned.
- Stability augmentation, attitude hold and position hold gains set for the Bo 105's
  weight - no more pitch oscillation in forward flight.
- Barometric altitude hold now has the same authority as radar altitude hold.
- Auto pedal swings less into nose-to-tail flight and in the hover.
- Getting out leaves the aircraft running; Engine off from the action menu shuts it down
  (Core 1.4.0).
- The development tools moved to HeliSim Core.

## 0.1.0.1

- Power-on autorotations work: full-down collective unloads the engines in forward flight.
