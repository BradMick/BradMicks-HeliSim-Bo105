/////////////////////////////////////////////////////////////////////////////////////////////
// Fuel //////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
// Fuel tanks ///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//  variableName - the aircraft names its own tank variables. Core prefixes bmkhs_ and appends
//              the field: "fwdTank" publishes bmkhs_fwdTankMass, bmkhs_fwdTankMax,
//              bmkhs_fwdTankLow. Names must be unique. Do not include the bmkhs_ prefix.
//  arm[]     - {lateral, longitudinal, vertical} in m, right-positive / nose-positive
//  capacity  - kg of usable fuel
//  lowFuelKg - low-level caution threshold in kg; 0 for no caution on this tank
//  removable - 1 if the tank can be taken out; 0 is always fitted
//  role      - "main" (an engine can draw from it) or "xfer" (feeds the mains only)

    //Both engines draw from the supply tank. The main tank tops it up - fn_setup sets the
    //XFER pump to SUPPLY.
    numCrossfeedModes = 1;
    class CrossfeedModes {
        class Norm { position = "NORM"; engSources[] = {"supplyTank", "supplyTank"}; };
    };

    xferDestinations[] = {"SUPPLY", "MAIN"};

    numFuelTanks = 2;
    class FuelTanks {
        class FuelTank01 {
            variableName = "supplyTank";
            arm[]     = {0.000, 3.170, 0.000};
            capacity  = 74.0;
            lowFuelKg = 74.0;
            removable = 0;
            role      = "main";
        };
        class FuelTank02 {
            variableName = "mainTank";
            arm[]     = {0.000, 2.210, 0.000};
            capacity  = 382.0;
            lowFuelKg = 0.0;
            removable = 0;
            role      = "main";
        };
    };

    numAuxTanks = 0;
