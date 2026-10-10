#include "\bmkhs_helisim\fmOverride.hpp"

class CfgVehicles {
    class gm_helicopter_base;
    //The base Bo 105, reopened with its own parent so its config is extended, not replaced.
    //Everything else about the aircraft - model, crew, weapons, fuel capacity - stays as it is.
    class gm_bo105_base : gm_helicopter_base {
        //Core owns the force coefficients - packs cannot tune them
        BMKHS_FM_OVERRIDE

        #include "bmkhs_bo105_config.hpp"
    };
};
