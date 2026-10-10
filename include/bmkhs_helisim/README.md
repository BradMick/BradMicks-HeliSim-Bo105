# HeliSim Core headers - vendored copy

A copy of the HeliSim Core header this pack includes, committed so the pack
builds with nothing but `hemtt build` - no submodule, no junction, no setup step.

**Matches HeliSim Core 1.2.1.0.**

**Do not edit these files here.** Change them in HeliSim Core, then copy them
back over. The procedure is in HeliSim Core's `docs/AIRCRAFT_GUIDE.md` under
"Building your mod against Core's headers".

| Header | Included by |
|---|---|
| `fmOverride.hpp` | `addons/helisim/config/cfgVehicles.hpp` - the flight model override block |

A no-systems aircraft needs only this one. Add the others (`hitPoints.hpp`,
`controlMacros.hpp`, `functions/.../*.hpp`) when the pack starts including them.
