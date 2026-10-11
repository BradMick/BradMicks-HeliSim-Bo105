/////////////////////////////////////////////////////////////////////////////////////////////
// Engine ////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////
    // Engines - the gas turbine model  /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////
    //numEngines is declared at the top of bmkhs_bo105_config.hpp, beside useSystems - it is
    //the airframe's count, not an engine's property. No hitpoints here, so it IS the count.
    //Allison 250-C20B. TODO(BO105): starter and spool values are the Tiger's.
    class Engines {
        class Engine01 {
            name            = "eng01";
            damageRole      = "engines";
            damageRoleIndex = 0;

            //Whole-assembly properties - not owned by any one section.
            engineType  = "turboShaftEngine";   //dispatches to bmkhs_fnc_turboShaftEngine
            designRpm   = 6000;                 //100% Np, the shaft reference
            npFly       = 1.00;                 //governed Np in FLY, as a fraction of designRpm
            maxFuelFlow = 0.0161;               //kg/s per unit of fuel - the gauge boundary
            powerKw     = 313;                  //100% torque - the torque reference, refTq

            //Hard shutdowns - both CUT FUEL rather than restricting it.
            maxNg = 1.10;                       //mechanical fly weights
            maxNp = 1.175;                      //engine overspeed

            //Engine Limits
            oilPsiLimits[] = {0.385, 1.000};    //3.5 / 9.1 kp/cm2
            ngMin          = 0.63;
            ngLimits[]     = {{1.05, 15, 10}, {1.06, 0, 20}};
            npLimits[]     = {{1.02, 15, 10}, {1.05, 0, 20}};
            tqLimits[]     = {{0.86, 0, 5}};
            tgtLimits[]    = {{779, 300, 1000}, {810, 6, 2000}, {843, 0, 2000}};
            tqLimitsSe[]   = {{0.95, 0, 5}};
            tgtLimitsSe[]  = {{779, 1800, 1000}, {810, 6, 2000}, {843, 0, 2000}};

            //Compressor - stations 2 -> 3.
            class Compressor {
                pressureRatio = 7.2;     //at Ng 1.0
                massFlow      = 1.56;    //kg/s at Ng 1.0, standard day
                inletDiameter = 0.396;   //m - for inlet losses, not yet modelled
                ramRecovery   = 1.0;     //share of the ram pressure rise the inlet keeps

                //Spooling down only - an unfired compressor is pure load, and that stops it.
                compDrag      = 3.4;     //as compDrag * ng^2
                compDragFloor = 0.10;    //finishes the stop - ng^2 alone only asymptotes

                //Airflow trim by FAT, {FAT, multiplier} - the OGE chart's engine-limited cells. 15 C held.
                airflowTable[] = {
                     {-40, 1.1400}
                    ,{-30, 1.0930}
                    ,{-20, 1.0460}
                    ,{-10, 1.0090}
                    ,{  0, 0.9710}
                    ,{ 10, 0.9900}
                    ,{ 15, 1.0000}
                    ,{ 20, 1.1360}
                    ,{ 30, 1.2120}
                    ,{ 40, 1.2880}
                };

                //Compressor map - compressor_map.py: MCP 15 C 0.950 779 0.86, TOP 15 C 0.968 810 0.95.
                compressorMap[] = {
                     {0.0000, 0.0588, 0.0000, 0.544, 0.2924}
                    ,{0.0500, 0.0595, 0.0116, 0.544, 0.2924}
                    ,{0.1000, 0.0618, 0.0329, 0.544, 0.2924}
                    ,{0.1500, 0.0656, 0.0604, 0.544, 0.2924}
                    ,{0.2000, 0.0712, 0.0930, 0.544, 0.2924}
                    ,{0.2500, 0.0789, 0.1300, 0.544, 0.2924}
                    ,{0.3000, 0.0891, 0.1709, 0.544, 0.2924}
                    ,{0.3500, 0.1024, 0.2153, 0.544, 0.2924}
                    ,{0.4000, 0.1194, 0.2631, 0.544, 0.2924}
                    ,{0.4500, 0.1410, 0.3139, 0.544, 0.2924}
                    ,{0.5000, 0.1683, 0.3676, 0.544, 0.2924}
                    ,{0.5500, 0.2026, 0.4241, 0.544, 0.2924}
                    ,{0.6000, 0.2456, 0.4833, 0.544, 0.2924}
                    ,{0.6173, 0.2629, 0.5043, 0.544, 0.2924}
                    ,{0.6768, 0.3329, 0.5891, 0.595, 0.3459}
                    ,{0.7546, 0.4559, 0.7130, 0.642, 0.4067}
                    ,{0.8125, 0.5971, 0.8152, 0.657, 0.4649}
                    ,{0.8636, 0.7925, 0.9502, 0.655, 0.4997}
                    ,{0.8800, 0.8694, 0.9595, 0.650, 0.5201}
                    ,{0.9167, 1.0306, 0.9804, 0.653, 0.5658}
                    ,{0.9500, 1.1766, 0.9993, 0.656, 0.6074}
                    ,{1.0000, 1.3959, 1.0278, 0.659, 0.6697}
                };

                //Start thresholds - discrete events the model branches on.
                lightOffNg = 0.15;       //fuel introduced
                selfSustNg = 0.52;       //starter cuts out
                //Where the start fuel ramp reaches full and residual heat has faded. Raise it
                //to hold fuel lean longer and peak cooler.
                idleNg     = 0.679;

                //Ng limiter - ngLimitMax min (ngLimitBase + ngLimitSlope * FAT).
                ngLimitMax   = 1.05;
                ngLimitBase  = 1.05;
                ngLimitSlope = 0.0;
            };

            //Combustor - stations 3 -> 4.
            class Combustor {
                fuelLhv             = 43000;    //kJ/kg - JP-8
                combustorEfficiency = 0.99;

                maxTgt      = 843;       //deg C - TGT limiter, twin engine
                maxTgtSe    = 843;       //deg C - TGT limiter, single engine
                startTgt    = 927;       //deg C - the transient START limit, not the peak
                startMinTgt = 80;        //deg C - below this before the power lever is moved

                //How violently an un-purged engine runs away, latched from TGT at the lever.
                residualHeatGain = 0.003;
            };

            //Compressor turbine - stations 4 -> 4.5. Drives the compressor, steps Ng; TGT is read here.
            class CompressorTurbine {
                turbineEfficiency = 0.88;
                spoolInertia      = 5.0; //how fast Ng answers a torque change
            };

            //The free turbine - stations 4.5 -> 5. Np is state with its own torque balance.
            class PowerTurbine {
                ptEfficiency  = 0.88;    //isentropic efficiency
                ptInertia     = 0.60;    //the free turbine's own inertia
                ptDrag        = 0.06;    //drag on a released turbine, as ptDrag * np^2
                ptDragFloor   = 0.05;    //finishes the stop - windmilling only
            };

            //The ECU - what it schedules, and the ceilings it will not pass.
            class Governor {
                //Minimum fuel the power lever schedules.
                fuelIdle = 0.521;        //settles Ng at 0.679
                fuelFly  = 3.136;        //WIDE OPEN - the governor cuts back from here

                //Fuel metered at light-off as a fraction of idle fuel. Sets the start PEAK.
                startFuelBase = 0.50;

                ffwdGain = 1.00;         //collective anticipation - the load demand spindle

                leverTravelTime = 15.0;   //seconds, idle to fly - the fuel ramp, Np and the lever animation
                loadShareGain   = 8.0;   //how hard an engine below its matched partners trims up to them

                //Np governor, {kp, ki, kd, ki_clamp}.
                pid[] = {80.0000, 40.0000, 0.0000, 0.0750};

                gate[] = {};             //nothing gates it - no systems modelled here
            };

            //Air turbine or electric, and what it needs available before the spool turns.
            //No systems are modelled here, so nothing gates it.
            class Starter {
                type   = "pneumatic";
                torque    = 0.45;               //stalled, on the spool, normalised
                runawayNg = 0.25;               //no torque left - where motoring settles
                gate[] = {};
            };
        };
        class Engine02 : Engine01 {
            name            = "eng02";
            damageRoleIndex = 1;
        };
    };
