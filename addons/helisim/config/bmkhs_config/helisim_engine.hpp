/////////////////////////////////////////////////////////////////////////////////////////////
// Engine ////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////
    // Engines - the gas turbine model  /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////
    //numEngines is declared at the top of bmkhs_bo105_config.hpp, beside useSystems - it is
    //the airframe's count, not an engine's property. No hitpoints here, so it IS the count.
    //TODO(BO105): the EC665 Tiger's MTR390 as a placeholder. Replace with the Bo 105's Allison
    //250-C20B and build its compressor map - AIRCRAFT_GUIDE.md, Step 3, "The engines".
    class Engines {
        class Engine01 {
            name            = "eng01";
            damageRole      = "engines";
            damageRoleIndex = 0;

            //Whole-assembly properties - not owned by any one section.
            engineType  = "turboShaftEngine";   //dispatches to bmkhs_fnc_turboShaftEngine
            designRpm   = 8000;                 //100% Np, the shaft reference
            npFly       = 1.00;                 //governed Np in FLY, as a fraction of designRpm
            maxFuelFlow = 0.033;                //kg/s per unit of fuel - the gauge boundary
            powerKw     = 957;                  //100% torque, 1143 Nm at 8000 rpm - the torque reference, refTq

            //Hard shutdowns - both CUT FUEL rather than restricting it.
            maxNg = 1.10;                       //mechanical fly weights
            maxNp = 1.175;                      //engine overspeed

            //Engine Limits
            oilPsiLimits[] = {0.203, 1.015};
            ngMin          = 0.63;
            ngLimits[]     = {{1.022, 12, 10}, {1.051, 0, 20}};
            npLimits[]     = {{1.057, 30, 10}, {1.121, 0, 20}};
            tqLimits[]     = {{0.94, 300, 5}, {1.006, 0, 10}};
            tgtLimits[]    = {{894, 300, 1000}, {928, 0, 2000}};
            tqLimitsSe[]   = {{0.94, 1800, 10}, {1.006, 150, 2}, {1.085, 30, 2}, {1.339, 0, 4}};
            tgtLimitsSe[]  = {{894, 1800, 1000}, {928, 150, 0}, {962, 30, 0}, {1036, 0, 2000}};

            //Compressor - stations 2 -> 3.
            class Compressor {
                pressureRatio = 13.0;    //at Ng 1.0
                massFlow      = 3.2;     //kg/s at Ng 1.0, standard day
                inletDiameter = 0.396;   //m - for inlet losses, not yet modelled
                ramRecovery   = 1.0;     //share of the ram pressure rise the inlet keeps

                //Spooling down only - an unfired compressor is pure load, and that stops it.
                compDrag      = 3.4;     //as compDrag * ng^2
                compDragFloor = 0.10;    //finishes the stop - ng^2 alone only asymptotes

                //Airflow trim by FAT, {FAT, multiplier} - dials the engine onto its charts.
                airflowTable[] = {
                     {-40, 1.0230}
                    ,{-30, 0.9700}
                    ,{-20, 0.9170}
                    ,{-10, 0.9770}
                    ,{  0, 1.0377}
                    ,{ 10, 1.0126}
                    ,{ 15, 1.0000}
                    ,{ 20, 1.0567}
                    ,{ 30, 1.1028}
                    ,{ 40, 1.1488}
                };

                //Compressor map - the MTR390's own, replacing Core's T700-701C default.
                compressorMap[] = {
                     {0.0000, 0.0588, 0.0000, 0.544, 0.4335}
                    ,{0.0500, 0.0595, 0.0116, 0.544, 0.4335}
                    ,{0.1000, 0.0618, 0.0329, 0.544, 0.4335}
                    ,{0.1500, 0.0656, 0.0604, 0.544, 0.4335}
                    ,{0.2000, 0.0712, 0.0930, 0.544, 0.4335}
                    ,{0.2500, 0.0789, 0.1300, 0.544, 0.4335}
                    ,{0.3000, 0.0891, 0.1709, 0.544, 0.4335}
                    ,{0.3500, 0.1024, 0.2153, 0.544, 0.4335}
                    ,{0.4000, 0.1194, 0.2631, 0.544, 0.4335}
                    ,{0.4500, 0.1410, 0.3139, 0.544, 0.4335}
                    ,{0.5000, 0.1683, 0.3676, 0.544, 0.4335}
                    ,{0.5500, 0.2026, 0.4241, 0.544, 0.4335}
                    ,{0.6000, 0.2456, 0.4833, 0.544, 0.4335}
                    ,{0.6173, 0.2629, 0.5043, 0.544, 0.4335}
                    ,{0.6768, 0.3329, 0.5891, 0.595, 0.4462}
                    ,{0.7546, 0.4559, 0.7130, 0.642, 0.4686}
                    ,{0.8125, 0.5971, 0.8152, 0.657, 0.5028}
                    ,{0.8790, 0.8651, 1.0310, 0.650, 0.5061}
                    ,{0.8957, 0.9383, 1.0225, 0.651, 0.5130}
                    ,{0.9133, 1.0155, 1.0261, 0.653, 0.5194}
                    ,{0.9392, 1.1291, 1.1114, 0.655, 0.5174}
                    ,{0.9500, 1.1766, 1.1471, 0.656, 0.5165}
                    ,{1.0000, 1.3959, 1.3116, 0.659, 0.5126}
                };

                //Start thresholds - discrete events the model branches on.
                lightOffNg = 0.15;       //fuel introduced
                selfSustNg = 0.52;       //starter cuts out
                //Where the start fuel ramp reaches full and residual heat has faded. Raise it
                //to hold fuel lean longer and peak cooler.
                idleNg     = 0.679;

                //Ng limiter - ngLimitMax min (ngLimitBase + ngLimitSlope * FAT).
                ngLimitMax   = 1.022;
                ngLimitBase  = 1.01436;
                ngLimitSlope = 0.0019091;
            };

            //Combustor - stations 3 -> 4.
            class Combustor {
                fuelLhv             = 43000;    //kJ/kg - JP-8
                combustorEfficiency = 0.99;

                maxTgt      = 1036;      //deg C - TGT limiter, twin engine
                maxTgtSe    = 1036;      //deg C - TGT limiter, single engine
                startTgt    = 750;       //deg C - the transient START limit, not the peak
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
                fuelIdle = 0.539;        //settles Ng at 0.679
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
