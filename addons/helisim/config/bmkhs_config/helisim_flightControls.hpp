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
//Field reference: \bmkhs_helisim\fmc.hpp. PID gains {kp, ki, kd, limit}. These are the
//tuning that defines how the aircraft handles - one set does not carry across airframes.
//No systems (useSystems = 0), so nothing gates them.
    class FMC {
        class Sas {
            gate[]      = {};
            pitch[]     = {0.0044, 0.0000, 0.000060, 0.00};    //pitch x1.7, roll x1.4 - restore after the lift coef cuts
            roll[]      = {0.0024, 0.0000, 0.000049, 0.00};
            yaw[]       = {0.0026, 0.00044, 0.00010, 0.001276};
        };
        class AttitudeHold {
            gate[]      = {};
            subModes[]  = {"pos", "vel", "att"};
            posPitch[]  = {0.131, 0.0061, 0.105, 0.1};     //position / velocity hold
            posRoll[]   = {0.039, 0.0050, 0.064, 0.1};
            attPitch[]  = {0.157, 0.0043, 0.0765, 0.1};
            attRoll[]   = {0.056, 0.0021, 0.0252, 0.1};
        };
        class AltitudeHold {
            gate[]      = {};
            rad[]       = {0.015, 0.00003, 0.0015, 0.05};
            bar[]       = {0.0003, 0.0000, 0.00024, 0.00};
        };
        class HeadingHold {
            gate[]      = {};
            subModes[]  = {"hdg", "yaw", "trn"};
            hdg[]       = {0.0750, 0.0200, 0.0050, 0.1};
            //Turn coordination. Error is lateral g and the output is clamped to the hold's
            //authority (0.1), so size these against that rather than the +-1 gauge.
            trn[]       = {0.2500, 0.0600, 0.3000, 0.1};
        };
    };

    //Auto-pedal
    pidAutoAttPitch[]   = {0.0925, 0.0025, 0.0220, 0.1};
    pidAutoAttRoll[]    = {0.0400, 0.0015, 0.0250, 0.1};
    pidAutoPedalHdg[]   = {0.0250, 0.00125, 0.00625, 0.075}; //halved - yaw swung in the hover
    pidAutoPedalNtt[]   = {0.0075, 0.0040, 0.0025, 0.075};  //kp halved - overshot into NTT
    pidAutoPedalAero[]  = {0.3000, 0.2500, 0.1000, 0.05};
