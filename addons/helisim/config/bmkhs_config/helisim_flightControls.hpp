/////////////////////////////////////////////////////////////////////////////////////////////
// Flight Controls //////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//TODO(BO105): the EC665 Tiger's gains as a placeholder - retune once the Bo 105 flies.
    inputLagValue     = 0.95;
    //Casual mode auto pitch target
    autoAttLevelPitch = -5.0;     //deg
    autoAttRollLimit  = 30.0;     //deg, bank commanded by the roll key

/////////////////////////////////////////////////////////////////////////////////////////////
// FMC              /////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//Field reference: \bmkhs_helisim\fmc.hpp. PID gains {kp, ki, kd, ki_clamp}. These are the
//tuning that defines how the aircraft handles - one set does not carry across airframes.
//No systems (useSystems = 0), so nothing gates them.
    class FMC {
        class Sas {
            gate[]      = {};
            pitch[]     = {0.0026, 0.0000, 0.000035, 0.00};
            roll[]      = {0.0017, 0.0000, 0.000035, 0.00};
            yaw[]       = {0.0026, 0.00044, 0.00010, 2.9};
        };
        class AttitudeHold {
            gate[]      = {};
            posPitch[]  = {0.077, 0.0036, 0.062, 0.014};   //position / velocity hold
            posRoll[]   = {0.028, 0.0036, 0.046, 0.014};
            attPitch[]  = {0.0925, 0.0025, 0.0450, 0.0025};
            attRoll[]   = {0.0400, 0.0015, 0.0180, 0.0015};
        };
        class AltitudeHold {
            gate[]      = {};
            rad[]       = {0.015, 0.00003, 0.0015, 0.00033};
            bar[]       = {0.0003, 0.0000, 0.00024, 0.00};
        };
        class HeadingHold {
            gate[]      = {};
            hdg[]       = {0.0750, 0.0200, 0.0050, 0.0200};
            //Turn coordination. Error is lateral g and the output is clamped to the hold's
            //authority (0.1), so size these against that rather than the +-1 gauge.
            trn[]       = {0.2500, 0.0600, 0.3000, 0.1500};
        };
    };

    //Auto-pedal
    pidAutoAttPitch[]   = {0.0925, 0.0025, 0.0220, 0.0025};
    pidAutoAttRoll[]    = {0.0400, 0.0015, 0.0250, 0.0015};
    pidAutoPedalHdg[]   = {0.0500, 0.0025, 0.0125, 30.000};
    pidAutoPedalNtt[]   = {0.0150, 0.0040, 0.0025, 18.750};
    pidAutoPedalAero[]  = {0.3000, 0.2500, 0.1000, 0.2000};
