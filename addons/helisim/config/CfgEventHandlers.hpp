class Extended_PreInit_EventHandlers {
    class bmkhs_bo105_helisim_preInit {
        init = "call compile preprocessFileLineNumbers 'bmkhs_bo105_helisim\XEH_preInit.sqf';";
    };
};

//The pack starts itself. Nothing outside it calls in.
class Extended_Init_EventHandlers {
    class gm_bo105_base {
        class bmkhs_bo105_helisim_init_eh {
            init = "_this call bmkhs_bo105_helisim_fnc_setup";
        };
    };
};

class Extended_GetIn_EventHandlers {
    class gm_bo105_base {
        class bmkhs_bo105_helisim_getin_eh {
            getIn = "_this call bmkhs_fnc_eventGetIn";
        };
    };
};
