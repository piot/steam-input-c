#include "clog/clog.h"
#include <atheneum/atheneum.h>
#include <stik/stik.h>








static int stikInitFunctions(StikFunctions* self, Atheneum* atheneum)
{
    self->createInputInstanceV006 = (SteamAPI_SteamInput_v006) atheneumAddress(atheneum, "SteamAPI_SteamInput_v006");;
    self->init  = (SteamAPI_ISteamInput_Init) atheneumAddress(atheneum, "SteamAPI_ISteamInput_Init");
    self->getActionSetHandle = (SteamAPI_ISteamInput_GetActionSetHandle) atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetActionSetHandle");
    self->getDigitalActionHandle  = (SteamAPI_ISteamInput_GetDigitalActionHandle) atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetDigitalActionHandle");
    self->activateActionSet  = (SteamAPI_ISteamInput_ActivateActionSet) atheneumAddress(atheneum, "SteamAPI_ISteamInput_ActivateActionSet");
    self->getConnectedControllers = (SteamAPI_ISteamInput_GetConnectedControllers) atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetConnectedControllers");
    self->runFrame  = (SteamAPI_ISteamInput_RunFrame) atheneumAddress(atheneum, "SteamAPI_ISteamInput_RunFrame");
    self->getDigitalActionOrigins  = (SteamAPI_ISteamInput_GetDigitalActionOrigins) atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetDigitalActionOrigins");
    self->getDigitalActionData = (SteamAPI_ISteamInput_GetDigitalActionData) atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetDigitalActionData");
    self->getCurrentActionSet = (SteamAPI_ISteamInput_GetCurrentActionSet) atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetCurrentActionSet");
    self->getInputTypeForHandle = (SteamAPI_ISteamInput_GetInputTypeForHandle)atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetInputTypeForHandle");
    self->getControllerForGamepadIndex =  (SteamAPI_ISteamInput_GetControllerForGamepadIndex)atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetControllerForGamepadIndex");

    return 0;
}


int stikInit(Stik* self, Atheneum *atheneum)
{
    int functionsResult = stikInitFunctions(&self->functions, atheneum);
    if (functionsResult < 0) {
        return functionsResult;
    }

    self->input = self->functions.createInputInstanceV006();

    CLOG_INFO("stik create %p", self->input);

    SteamBool initWorked = self->functions.init(self->input);
    if (!initWorked) {
        return -6;
    }
//    self->functions.runFrame(self->input);

    CLOG_INFO("stik init %d", initWorked);

    return 0;
}

int stikGetConnectedControllers(Stik* self, InputHandle_t* handles)
{
    return self->functions.getConnectedControllers(self->input, handles);
}

/*
 *

self->functions.runFrame(self->input);
CLOG_INFO("stik runhandle");

InputHandle_t controllerHandles[STEAM_INPUT_MAX_COUNT];
SteamInt controllerCount = self->functions.getConnectedControllers(self->input, controllerHandles);

if (controllerCount <= 0) {
return -3;
}

InputHandle_t controllerHandle = controllerHandles[0];
CLOG_INFO("stik controllerHandle %08X", controllerHandle);

self->functions.runFrame(self->input);
CLOG_INFO("stik runhandle2");

self->functions.activateActionSet(self->input, controllerHandle, setHandle);

self->functions.runFrame(self->input);
CLOG_INFO("stik runhandle2");




EInputActionOrigin origins[MAX_ORIGINS];
SteamInt numberOfOrigins = self->functions.getDigitalActionOrigins(self->input, controllerHandle, setHandle, abilityOneHandle, origins);
CLOG_INFO("stik numberOfOrigins %d", numberOfOrigins);

self->functions.runFrame(self->input);

InputDigitalActionData_t data = self->functions.getDigitalActionData(self->input, controllerHandle, abilityOneHandle);

CLOG_INFO("stik digital action %d %d", data.state, data.active);
if (!data.active) {
return -904;
}


 */