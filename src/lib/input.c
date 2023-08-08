/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "clog/clog.h"
#include <atheneum/atheneum.h>
#include <stik/stik.h>
#include <inttypes.h>

static int stikInitFunctions(StikFunctions* self, Atheneum* atheneum)
{
    self->createInputInstanceV006 = (SteamAPI_SteamInput_v006) atheneumAddress(atheneum, "SteamAPI_SteamInput_v006");
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



    self->getAnalogActionHandle  = (SteamAPI_ISteamInput_GetAnalogActionHandle) atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetAnalogActionHandle");
    self->getAnalogActionOrigins  = (SteamAPI_ISteamInput_GetAnalogActionOrigins) atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetAnalogActionOrigins");
    self->getAnalogActionData = (SteamAPI_ISteamInput_GetAnalogActionData) atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetAnalogActionData");


    return 0;
}


int stikInit(Stik* self, Atheneum *atheneum)
{
    int functionsResult = stikInitFunctions(&self->functions, atheneum);
    if (functionsResult < 0) {
        return functionsResult;
    }

    self->input = self->functions.createInputInstanceV006();

    CLOG_INFO("stik create %p", (void*) self->input)

    SteamBool initWorked = self->functions.init(self->input, 1);
    if (!initWorked) {
        return -6;
    }
    //self->functions.runFrame(self->input);

    CLOG_INFO("stik init %d", initWorked)

    return 0;
}

int stikGetConnectedControllers(Stik* self, InputHandle_t* handles)
{
    return self->functions.getConnectedControllers(self->input, handles);
}


InputActionSetHandle_t stikGetActionSetHandle(Stik* self, const char* actionSetName)
{
    return self->functions.getActionSetHandle(self->input, actionSetName);
}

InputDigitalActionHandle_t stikGetDigitalActionHandle(Stik* self, const char* digitalActionName)
{
    return self->functions.getDigitalActionHandle(self->input, digitalActionName);
}

InputAnalogActionHandle_t stikGetAnalogActionHandle(Stik* self, const char* analogActionName)
{
    return self->functions.getAnalogActionHandle(self->input, analogActionName);
}

int stikActivateActionSet(Stik* self, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle)
{
    self->functions.activateActionSet(self->input, inputHandle, actionSetHandle);
    self->functions.runFrame(self->input);
    InputActionSetHandle_t detectedActionSet = self->functions.getCurrentActionSet(self->input, inputHandle);
    if (detectedActionSet != actionSetHandle) {
        CLOG_SOFT_ERROR("could not set action set %" PRIX64 " (%" PRIX64 ")", actionSetHandle, detectedActionSet)
        return -1;
    }
    return 0;
}

int stikUpdate(Stik* self)
{
    self->functions.runFrame(self->input);

    return 0;
}

InputDigitalActionData_t stikGetDigitalActionData(Stik* self, InputHandle_t controllerHandle, InputDigitalActionHandle_t digitalActionHandle)
{
    return self->functions.getDigitalActionData(self->input, controllerHandle, digitalActionHandle);
}

InputAnalogActionData_t stikGetAnalogActionData(Stik* self, InputHandle_t controllerHandle, InputAnalogActionHandle_t analogActionHandle)
{
    return self->functions.getAnalogActionData(self->input, controllerHandle, analogActionHandle);
}

ESteamInputType stikGetInputTypeForHandle(Stik* self, InputHandle_t inputHandle)
{
    return self->functions.getInputTypeForHandle(self->input, inputHandle);
}

const char* stikGetInputTypeName(ESteamInputType controllerType)
{
    static const char* values[] = {
        "Unknown",
        "Steam Controller",
        "Xbox 360 Controller",
        "Xbox One Controller",
        "Generic XInput",
        "PS4 Controller",
        "Apple MFi Controller",
        "Android Controller",
        "SwithJoyConPAir",
        "SwitchJoyConSingle",
        "SwitchProController",
        "MobileTouch",
        "Ps3 controller",
        "Unknown2",
        "Steam Deck"};

    if (controllerType > 0 && controllerType < k_ESteamInputType_Count) {
        return values[controllerType];
    }

    return "Controller Unknown. Outside of values";
}

int stikGetDigitalActionOrigins(Stik* self, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle, InputDigitalActionHandle_t digitalActionHandle, EInputActionOrigin *originsOut)
{
    return self->functions.getDigitalActionOrigins(self->input, inputHandle, actionSetHandle, digitalActionHandle, originsOut);
}



int stikGetAnalogActionOrigins(Stik* self, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle, InputAnalogActionHandle_t analogActionHandle, EInputActionOrigin *originsOut)
{
    return self->functions.getAnalogActionOrigins(self->input, inputHandle, actionSetHandle, analogActionHandle, originsOut);
}
