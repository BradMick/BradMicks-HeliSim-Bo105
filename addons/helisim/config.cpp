class CfgPatches
{
    class bmkhs_bo105_helisim
    {
        units[] = {};
        author = "BradMick";
        weapons[] = {};
        requiredVersion = 2.10;
        //HeliSim Core, and the Bo 105 - this pack modifies its class, so it must load after it.
        requiredAddons[] = {"bmkhs_helisim", "gm_vehicles_air_helicopters_bo105"};
        //The airframe this pack drives. Every Bo 105 variant inherits from it.
        bmkhsBaseClass   = "gm_bo105_base";
        #include "version.hpp"
    };
};

#include "CfgFunctions.hpp"
#include "config\CfgEventHandlers.hpp"
#include "config\cfgVehicles.hpp"
