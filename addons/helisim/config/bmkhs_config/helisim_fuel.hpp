/////////////////////////////////////////////////////////////////////////////////////////////
// Fuel //////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//TODO(BO105): the EC665 Tiger's two tanks as a placeholder - tanks, arms and capacities from
//the Bo 105.
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

    //CROSSFEED positions - which tank each engine feeds from in each valve position, by
    //tank variableName. The first entry is the default.
    numCrossfeedModes = 3;
    class CrossfeedModes {
        class Norm { position = "NORM"; engSources[] = {"fwdTank", "aftTank"}; };
        class Fwd  { position = "FWD";  engSources[] = {"fwdTank", "fwdTank"}; };
        class Aft  { position = "AFT";  engSources[] = {"aftTank", "aftTank"}; };
    };

    //XFER pump destinations, in main order.
    xferDestinations[] = {"FWD", "AFT"};

    numFuelTanks = 2;
    class FuelTanks {
        class FuelTank01 {
            variableName = "fwdTank";
            arm[]     = {0.000, 1.900, 0.000};
            capacity  = 590.0;
            lowFuelKg = 92.0;
            removable = 0;
            role      = "main";
        };
        class FuelTank02 {
            variableName = "aftTank";
            arm[]     = {0.000, 1.200, 0.000};
            capacity  = 575.0;
            lowFuelKg = 92.0;
            removable = 0;
            role      = "main";
        };
    };

    numAuxTanks = 0;
