/////////////////////////////////////////////////////////////////////////////////////////////
// Mass and Balance /////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
    //TODO(BO105): every value here is the EC665 Tiger's placeholder, in the Tiger's model
    //space. Replace from the Bo 105 manual and the GM model - AIRCRAFT_GUIDE.md, Step 3,
    //"Mass and balance - converting the CG".

    //Empty airframe. No EmptyMassVariants - no fitted equipment changes it.
    emptyMass         = 3060; //kg
    emptyMom          = 21626.550;

    //Maximum gross mass - bounds the fixed test weight
    maxGrossMass      = 6000; //kg

    //Fuselage station datum and CG limits
    fsDatum             = 8.595;      //m, station 0 reference
    fwdCgLimit          = 1.615;    //m
    aftCgLimit          = 1.440;    //m
    comCorrection[]     = {0.0, 0.0, 0.224};
    //Casual mode center of mass
    casualModeCom[]     = {0.0, 1.56, 1.225};

/////////////////////////////////////////////////////////////////////////////////////////////
// Indexed mass items ///////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
    //Every mass item is {arm, mass}: arm[] is {lateral, longitudinal, vertical} in metres,
    //right-positive / nose-positive, in the same surveyed frame as fsDatum.

    //SEATS. Occupancy is resolved against fullCrew, so an empty seat adds no mass.
    //  role       - "driver" | "gunner" | "commander" | "turret" | "cargo"
    //  turret[]   - turret path for gunner/commander/turret seats; {} for the driver
    //  cargoIndex - cargo slot for role = "cargo"; -1 otherwise
    //TODO(BO105): side-by-side crew - arms, roles and turret paths from the GM model, and the
    //rear bench as cargo seats.
    numSeats = 2;
    class Seats {
        class Seat01 {  //gunner
            arm[]      = {0.000, 4.145, 0.000};
            mass       = 100.0;
            role       = "gunner";
            turret[]   = {0};
            cargoIndex = -1;
        };
        class Seat02 {  //pilot
            arm[]      = {0.000, 2.695, 0.000};
            mass       = 100.0;
            role       = "driver";
            turret[]   = {};
            cargoIndex = -1;
        };
    };

    //TODO(BO105): the PAH-1's HOT launchers - stations and stores once it is known whether GM
    //carries them as pylons or as fixed magazines.
    numStations  = 0;
    numMagazines = 0;
    numStores    = 0;
