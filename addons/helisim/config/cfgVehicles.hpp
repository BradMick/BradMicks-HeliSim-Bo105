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

    //GM sets the force coefficients again below the base, so each variant overrides them too.
    #define BO105_VARIANT(NAME) \
        class NAME##_base; \
        class NAME : NAME##_base { BMKHS_FM_OVERRIDE };

    BO105_VARIANT(gm_ge_army_bo105p_pah1)
    BO105_VARIANT(gm_ge_army_bo105p_pah1a1)
    BO105_VARIANT(gm_ge_army_bo105m_vbh)
    BO105_VARIANT(gm_ge_army_bo105p1m_vbh)
    BO105_VARIANT(gm_ge_bgs_bo105m_vbh)
    BO105_VARIANT(gm_ge_pol_bo105m_vbh)
    BO105_VARIANT(gm_ge_adak_bo105m_vbh)

    class gm_ge_army_bo105p1m_vbh_swooper_base;
    class gm_ge_army_bo105p1m_vbh_swooper : gm_ge_army_bo105p1m_vbh_swooper_base {
        BMKHS_FM_OVERRIDE
        #define BO105_SWOOPER
        #include "bmkhs_bo105_config.hpp"
        #undef BO105_SWOOPER
    };
};
