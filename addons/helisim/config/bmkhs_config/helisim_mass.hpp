/////////////////////////////////////////////////////////////////////////////////////////////
// Mass and Balance /////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

    //Empty airframe. No EmptyMassVariants - no fitted equipment changes it.
    emptyMass         = 1675; //kg
    emptyMom          = 5468.875;

    //Maximum gross mass - bounds the fixed test weight
    maxGrossMass      = 2500; //kg

    //Fuselage station datum and CG limits
    fsDatum             = 5.445;      //m, station 0 reference
    fwdCgLimit          = 2.370;    //m
    aftCgLimit          = 2.050;    //m
    comCorrection[]     = {0.000, 0.000, -0.326};
    //Casual mode center of mass
    casualModeCom[]     = {0.000, 0.810, 0.975};

/////////////////////////////////////////////////////////////////////////////////////////////
// Indexed mass items ///////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
    //Every mass item is {arm, mass}: arm[] is {lateral, longitudinal, vertical} in metres,
    //right-positive / nose-positive, in the same surveyed frame as fsDatum.

    //SEATS. Occupancy is resolved against fullCrew, so an empty seat adds no mass.
    //  role       - "driver" | "gunner" | "commander" | "turret" | "cargo"
    //  turret[]   - turret path for gunner/commander/turret seats; {} for the driver
    //  cargoIndex - cargo slot for role = "cargo"; -1 otherwise
#ifdef BO105_SWOOPER
    numSeats = 10;
    class Seats {
        class Seat01 {  //pilot
            arm[]      = {0.364, 3.432, -1.022};
            mass       = 100.0;
            role       = "driver";
            turret[]   = {};
            cargoIndex = -1;
        };
        class Seat02 {  //copilot
            arm[]      = {-0.375, 3.468, -1.018};
            mass       = 100.0;
            role       = "turret";
            turret[]   = {0};
            cargoIndex = -1;
        };
        class Seat03 {  //left bench 1
            arm[]      = {-1.062, 2.254, -1.147};
            mass       = 100.0;
            role       = "turret";
            turret[]   = {1};
            cargoIndex = -1;
        };
        class Seat04 {  //right bench 1
            arm[]      = {1.049, 2.252, -1.138};
            mass       = 100.0;
            role       = "turret";
            turret[]   = {2};
            cargoIndex = -1;
        };
        class Seat05 {  //left bench 2
            arm[]      = {-1.028, 1.614, -1.142};
            mass       = 100.0;
            role       = "turret";
            turret[]   = {3};
            cargoIndex = -1;
        };
        class Seat06 {  //right bench 2
            arm[]      = {1.051, 1.622, -1.144};
            mass       = 100.0;
            role       = "turret";
            turret[]   = {4};
            cargoIndex = -1;
        };
        class Seat07 {  //rear left
            arm[]      = {-0.393, 2.537, -0.899};
            mass       = 100.0;
            role       = "turret";
            turret[]   = {5};
            cargoIndex = -1;
        };
        class Seat08 {  //rear right
            arm[]      = {0.354, 2.542, -0.937};
            mass       = 100.0;
            role       = "turret";
            turret[]   = {6};
            cargoIndex = -1;
        };
        class Seat09 {  //rear left outside
            arm[]      = {-0.742, 2.816, -1.161};
            mass       = 100.0;
            role       = "turret";
            turret[]   = {7};
            cargoIndex = -1;
        };
        class Seat10 {  //rear right outside
            arm[]      = {0.757, 2.851, -1.166};
            mass       = 100.0;
            role       = "turret";
            turret[]   = {8};
            cargoIndex = -1;
        };
    };
#else
    numSeats = 6;
    class Seats {
        class Seat01 {  //pilot
            arm[]      = {0.364, 3.433, -1.022};
            mass       = 100.0;
            role       = "driver";
            turret[]   = {};
            cargoIndex = -1;
        };
        class Seat02 {  //copilot / gunner
            arm[]      = {-0.375, 3.400, -1.020};
            mass       = 100.0;
            role       = "turret";
            turret[]   = {0};
            cargoIndex = -1;
        };
        class Seat03 {  //rear left
            arm[]      = {-0.394, 2.538, -0.899};
            mass       = 100.0;
            role       = "turret";
            turret[]   = {1};
            cargoIndex = -1;
        };
        class Seat04 {  //rear right
            arm[]      = {0.354, 2.544, -0.937};
            mass       = 100.0;
            role       = "turret";
            turret[]   = {2};
            cargoIndex = -1;
        };
        class Seat05 {  //rear left outside
            arm[]      = {-0.742, 2.816, -1.161};
            mass       = 100.0;
            role       = "turret";
            turret[]   = {3};
            cargoIndex = -1;
        };
        class Seat06 {  //rear right outside
            arm[]      = {0.756, 2.851, -1.166};
            mass       = 100.0;
            role       = "turret";
            turret[]   = {4};
            cargoIndex = -1;
        };
    };
#endif

    //STATIONS - the HOT launchers
    numStations = 2;
    class Stations {
        class Station01 {  //left
            arm[]    = {-1.614, 2.015, -1.153};
            pylons[] = {1, 3, 5};
        };
        class Station02 {  //right
            arm[]    = {1.614, 2.015, -1.153};
            pylons[] = {2, 4, 6};
        };
    };

    numMagazines = 0;

    //TODO(BO105): launcherMass is the three tubes only - add the launcher frame when known.
    numStores = 1;
    class Stores {
        class Store01 {  //HOT 1 / HOT 2
            match        = "hot_heat";
            launcherMass = 25.5;    //kg, 3 x 8.5 kg tube
            massPerRound = 23.5;    //kg
            isTank       = 0;
        };
    };
