#ifndef STIK_H
#define STIK_H

#include <steam-api/steam_api.h>

typedef SteamU64 InputHandle_t;
typedef SteamU64 InputActionSetHandle_t;
typedef SteamU64 InputDigitalActionHandle_t;

typedef struct InputDigitalActionData_t
{
#pragma pack(push, 0)
    uint8_t state;
    uint8_t active;
#pragma pack(pop)
//uint64_t data1;
//uint64_t data2;
} InputDigitalActionData_t;

typedef SteamInt EInputActionOrigin;
typedef SteamInt ESteamInputType;
typedef void* ISteamInput;

typedef ISteamInput* (S_CALLTYPE *SteamAPI_SteamInput_v006)(void);
typedef SteamBool (S_CALLTYPE *SteamAPI_ISteamInput_Init)(ISteamInput* input);
typedef InputActionSetHandle_t (S_CALLTYPE *SteamAPI_ISteamInput_GetActionSetHandle)(ISteamInput* input, SteamConstantString actionSet);
typedef InputDigitalActionHandle_t (S_CALLTYPE *SteamAPI_ISteamInput_GetDigitalActionHandle)(ISteamInput* input, SteamConstantString digitalAction);
typedef InputHandle_t (S_CALLTYPE *SteamAPI_ISteamInput_GetControllerForGamepadIndex)(ISteamInput* input, SteamInt controllerIndex);
typedef void (S_CALLTYPE *SteamAPI_ISteamInput_ActivateActionSet)(ISteamInput* input, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle);
typedef SteamInt (S_CALLTYPE *SteamAPI_ISteamInput_GetConnectedControllers)(ISteamInput* input, InputHandle_t *handlesOut);
typedef void (S_CALLTYPE *SteamAPI_ISteamInput_RunFrame)(ISteamInput* input);
typedef SteamInt (S_CALLTYPE *SteamAPI_ISteamInput_GetDigitalActionOrigins)(ISteamInput* input, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle, InputDigitalActionHandle_t digitalActionHandle, EInputActionOrigin *originsOut);
typedef InputDigitalActionData_t (S_CALLTYPE *SteamAPI_ISteamInput_GetDigitalActionData)(ISteamInput* input, InputHandle_t controllerHandle, SteamU64 digitalActionHandle);
typedef InputActionSetHandle_t (S_CALLTYPE *SteamAPI_ISteamInput_GetCurrentActionSet)(ISteamInput* input, InputHandle_t controllerHandle);
typedef ESteamInputType (S_CALLTYPE *SteamAPI_ISteamInput_GetInputTypeForHandle)(ISteamInput* input,  InputHandle_t inputHandle);

#define STIK_INPUT_MAX_COUNT (16)
#define STIK_MAX_ORIGINS (8)


struct Atheneum;

typedef struct StikFunctions {
    SteamAPI_SteamInput_v006 createInputInstanceV006;
    SteamAPI_ISteamInput_Init init;
    SteamAPI_ISteamInput_GetActionSetHandle getActionSetHandle;
    SteamAPI_ISteamInput_GetDigitalActionHandle getDigitalActionHandle;
    SteamAPI_ISteamInput_ActivateActionSet activateActionSet;
    SteamAPI_ISteamInput_GetConnectedControllers getConnectedControllers;
    SteamAPI_ISteamInput_RunFrame runFrame;
    SteamAPI_ISteamInput_GetDigitalActionOrigins getDigitalActionOrigins;
    SteamAPI_ISteamInput_GetDigitalActionData getDigitalActionData;
    SteamAPI_ISteamInput_GetCurrentActionSet getCurrentActionSet;
    SteamAPI_ISteamInput_GetInputTypeForHandle getInputTypeForHandle;
    SteamAPI_ISteamInput_GetControllerForGamepadIndex getControllerForGamepadIndex;
} StikFunctions;

typedef struct Stik {
    ISteamInput* input;
    StikFunctions functions;
} Stik;

int stikInit(Stik* self, struct Atheneum *atheneum);
int stikGetConnectedControllers(Stik* self, InputHandle_t* handles);

#endif
