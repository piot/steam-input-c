#include "clog/clog.h"
#include <atheneum/atheneum.h>
#include <stik/stik.h>

typedef ISteamInput* (S_CALLTYPE *SteamAPI_SteamInput_v006)(void);
typedef SteamBool (S_CALLTYPE *SteamAPI_ISteamInput_Init)(ISteamInput* input, SteamBool explicitRunFrame);
typedef SteamU64 (S_CALLTYPE *SteamAPI_ISteamInput_GetActionSetHandle)(ISteamInput* input, SteamConstantString actionSet);
typedef SteamU64 (S_CALLTYPE *SteamAPI_ISteamInput_GetDigitalActionHandle)(ISteamInput* input, SteamConstantString digitalAction);
typedef SteamU64 (S_CALLTYPE *SteamAPI_ISteamInput_GetControllerForGamepadIndex)(ISteamInput* input, SteamInt controllerIndex);
typedef void (S_CALLTYPE *SteamAPI_ISteamInput_ActivateActionSet)(ISteamInput* input, SteamU64 inputHandle, SteamU64 actionSetHandle);
typedef SteamInt (S_CALLTYPE *SteamAPI_ISteamInput_GetConnectedControllers)(ISteamInput* input, SteamU64 *handlesOut);
typedef void (S_CALLTYPE *SteamAPI_ISteamInput_RunFrame)(ISteamInput* input);
typedef SteamInt EInputActionOrigin;

typedef  SteamInt (S_CALLTYPE *SteamAPI_ISteamInput_GetDigitalActionOrigins)(ISteamInput* input, SteamU64 inputHandle, SteamU64 actionSetHandle, SteamU64 digitalActionHandle, EInputActionOrigin *originsOut );



typedef struct InputDigitalActionData_t
{
#pragma pack(push, 0)
    SteamBool state;
    SteamBool active;
#pragma pack(pop)
} InputDigitalActionData_t;

#define STEAM_INPUT_MAX_COUNT (16)

typedef InputDigitalActionData_t (S_CALLTYPE *SteamAPI_ISteamInput_GetDigitalActionData)(ISteamInput* input, SteamU64 controllerHandle, SteamU64 digitalActionHandle);

int stikInit(Stik* self, Atheneum *atheneum)
{
    SteamAPI_SteamInput_v006 createInput = (SteamAPI_SteamInput_v006) atheneumAddress(atheneum, "SteamAPI_SteamInput_v006");
    ISteamInput inputInstance = createInput();

    CLOG_INFO("stik create %p", inputInstance);

    SteamAPI_ISteamInput_Init initInput = (SteamAPI_ISteamInput_Init) atheneumAddress(atheneum, "SteamAPI_ISteamInput_Init");
    SteamBool initWorked = initInput(inputInstance, 0);

    CLOG_INFO("stik init %d", initWorked);


    SteamAPI_ISteamInput_GetActionSetHandle getActionSetHandle = (SteamAPI_ISteamInput_GetActionSetHandle) atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetActionSetHandle");
    SteamU64 setHandle = getActionSetHandle(inputInstance, "Gamepad");
    CLOG_INFO("stik getHandle %p", setHandle);
    if (setHandle == 0) {
        return -4;
    }

    SteamAPI_ISteamInput_RunFrame runFrame = (SteamAPI_ISteamInput_RunFrame) atheneumAddress(atheneum, "SteamAPI_ISteamInput_RunFrame");
    runFrame(inputInstance);
    CLOG_INFO("stik runhandle");

    SteamAPI_ISteamInput_GetConnectedControllers getConnectedControllers = (SteamAPI_ISteamInput_GetConnectedControllers) atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetConnectedControllers");
    SteamU64 controllerHandles[STEAM_INPUT_MAX_COUNT];
    SteamInt controllerCount = getConnectedControllers(inputInstance, controllerHandles);

    if (controllerCount <= 0) {
        return -3;
    }

    SteamU64 controllerHandle = controllerHandles[0];
    CLOG_INFO("stik controllerHandle %08X", controllerHandle);

    runFrame(inputInstance);
    CLOG_INFO("stik runhandle2");

    SteamAPI_ISteamInput_ActivateActionSet activateActionSet = (SteamAPI_ISteamInput_ActivateActionSet) atheneumAddress(atheneum, "SteamAPI_ISteamInput_ActivateActionSet");
    activateActionSet(inputInstance, controllerHandle, setHandle);

    runFrame(inputInstance);
    CLOG_INFO("stik runhandle2");



    SteamAPI_ISteamInput_GetDigitalActionHandle getDigitalActionHandle = (SteamAPI_ISteamInput_GetDigitalActionHandle) atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetDigitalActionHandle");
    SteamU64 abilityOneHandle = getDigitalActionHandle(inputInstance, "Interact");
    CLOG_INFO("stik abilityOneHandle %d", abilityOneHandle);
    if (abilityOneHandle == 0) {
    //    return -5;
    }

#define MAX_ORIGINS (8)

    EInputActionOrigin origins[MAX_ORIGINS];
    SteamAPI_ISteamInput_GetDigitalActionOrigins getDigitalActionOrigins = (SteamAPI_ISteamInput_GetDigitalActionOrigins) atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetDigitalActionOrigins");
    SteamInt numberOfOrigins = getDigitalActionOrigins(inputInstance, controllerHandle, setHandle, abilityOneHandle, origins);
    CLOG_INFO("stik numberOfOrigins %d", numberOfOrigins);

    SteamAPI_ISteamInput_GetDigitalActionData getDigitalActionData = (SteamAPI_ISteamInput_GetDigitalActionData) atheneumAddress(atheneum, "SteamAPI_ISteamInput_GetDigitalActionData");
    InputDigitalActionData_t data = getDigitalActionData(inputInstance, controllerHandle, abilityOneHandle);

    CLOG_INFO("stik digital action %d %d", data.state, data.active);
    if (!data.active) {
        return -904;
    }

    return 0;
}
