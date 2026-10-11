# Bo 105 - HeliSim

## Requires

| mod | why |
|---|---|
| BradMick's HeliSim 1.2.0.0 or later | the flight model this pack drives |
| Global Mobilization - Cold War Germany | the Bo 105 itself (`gm_vehicles_air_helicopters_bo105`) |
| CBA_A3 | HeliSim's dependency |

Load all three in game. This pack loads after HeliSim and after Global Mobilization's Bo 105.
Every Bo 105 variant inherits from `gm_bo105_base`, so all of them fly this model.

## Flight model

How every value was derived, and the commands that regenerate it: [docs/BO105_FM.md](docs/BO105_FM.md).
Tools in [tools/](tools/).
