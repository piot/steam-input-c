#ifndef STIK_H
#define STIK_H

#include <steam-api/steam_api.h>
#include <stdbool.h>

typedef SteamU64 InputHandle_t;
typedef SteamU64 InputActionSetHandle_t;
typedef SteamU64 InputDigitalActionHandle_t;
typedef SteamU64 InputAnalogActionHandle_t;

typedef struct InputDigitalActionData_t
{
//#pragma pack(push, 0)
    bool state;
    bool active;
//#pragma pack(pop)
//uint64_t data1;
//uint64_t data2;
} InputDigitalActionData_t;

typedef enum EControllerSourceMode {
    k_EInputSourceMode_Dpad = 1,
} EControllerSourceMode;


typedef struct InputAnalogActionData_t {
    EControllerSourceMode sourceMode;
    float x;
    float y;
    bool active;
} InputAnalogActionData_t;


typedef enum ESteamInputTypeEnum {
    k_ESteamInputType_Unknown = 0, //	Catch-all for unrecognized devices
    k_ESteamInputType_SteamController = 1, //	Valve's Steam Controller
    k_ESteamInputType_XBox360Controller	= 2,	//Microsoft's XBox 360 Controller
    k_ESteamInputType_XBoxOneController	= 3, //	Microsoft's XBox One Controller
    k_ESteamInputType_GenericXInput	= 4, //Any generic 3rd-party XInput device
    k_ESteamInputType_PS4Controller	= 5, //	Sony's PlayStation 4 Controller
    k_ESteamInputType_AppleMFiController = 6, //	Unused
    k_ESteamInputType_AndroidController	= 7, //	Unused
    k_ESteamInputType_SwitchJoyConPair	= 8, //	Unused
    k_ESteamInputType_SwitchJoyConSingle = 9, //	Unused
    k_ESteamInputType_SwitchProController = 10, //	Nintendo's Switch Pro Controller
    k_ESteamInputType_MobileTouch	= 11, //	Steam Link App's Mobile Touch Controller
    k_ESteamInputType_PS3Controller	= 12, //Sony's PlayStation 3 Controller or PS3/PS4 compatible fight stick
    k_ESteamInputType_unknown2	= 13, //
    k_ESteamInputType_SteamDeck	= 14, //
    k_ESteamInputType_Count	= 15, //13	Current number of values returned
};

typedef SteamInt EInputActionOrigin;
typedef SteamInt ESteamInputType;
typedef void* ISteamInput;

#include <stdbool.h>

typedef ISteamInput* (S_CALLTYPE *SteamAPI_SteamInput_v006)(void);
typedef SteamBool (S_CALLTYPE *SteamAPI_ISteamInput_Init)(ISteamInput* input, bool explicitlyCallFrame); // bool explicitlyCallFrame is unknown if it exists as a parameter.
typedef InputActionSetHandle_t (S_CALLTYPE *SteamAPI_ISteamInput_GetActionSetHandle)(ISteamInput* input, SteamConstantString actionSet);
typedef InputHandle_t (S_CALLTYPE *SteamAPI_ISteamInput_GetControllerForGamepadIndex)(ISteamInput* input, SteamInt controllerIndex);
typedef void (S_CALLTYPE *SteamAPI_ISteamInput_ActivateActionSet)(ISteamInput* input, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle);
typedef SteamInt (S_CALLTYPE *SteamAPI_ISteamInput_GetConnectedControllers)(ISteamInput* input, InputHandle_t *handlesOut);
typedef void (S_CALLTYPE *SteamAPI_ISteamInput_RunFrame)(ISteamInput* input);
typedef InputActionSetHandle_t (S_CALLTYPE *SteamAPI_ISteamInput_GetCurrentActionSet)(ISteamInput* input, InputHandle_t controllerHandle);
typedef ESteamInputType (S_CALLTYPE *SteamAPI_ISteamInput_GetInputTypeForHandle)(ISteamInput* input,  InputHandle_t inputHandle);

typedef InputDigitalActionHandle_t (S_CALLTYPE *SteamAPI_ISteamInput_GetDigitalActionHandle)(ISteamInput* input, SteamConstantString digitalAction);
typedef SteamInt (S_CALLTYPE *SteamAPI_ISteamInput_GetDigitalActionOrigins)(ISteamInput* input, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle, InputDigitalActionHandle_t digitalActionHandle, EInputActionOrigin *originsOut);
typedef InputDigitalActionData_t (S_CALLTYPE *SteamAPI_ISteamInput_GetDigitalActionData)(ISteamInput* input, InputHandle_t controllerHandle, InputDigitalActionHandle_t digitalActionHandle);


typedef InputAnalogActionHandle_t (S_CALLTYPE *SteamAPI_ISteamInput_GetAnalogActionHandle)(ISteamInput* input, SteamConstantString digitalAction);
typedef SteamInt (S_CALLTYPE *SteamAPI_ISteamInput_GetAnalogActionOrigins)(ISteamInput* input, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle, InputAnalogActionHandle_t analogActionHandle, EInputActionOrigin *originsOut);
typedef InputAnalogActionData_t (S_CALLTYPE *SteamAPI_ISteamInput_GetAnalogActionData)(ISteamInput* input, InputHandle_t controllerHandle, InputAnalogActionHandle_t analogActionHandle);



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

    SteamAPI_ISteamInput_GetAnalogActionHandle getAnalogActionHandle;
    SteamAPI_ISteamInput_GetAnalogActionOrigins getAnalogActionOrigins;
    SteamAPI_ISteamInput_GetAnalogActionData getAnalogActionData;
} StikFunctions;

typedef struct Stik {
    ISteamInput* input;
    StikFunctions functions;
} Stik;

int stikInit(Stik* self, struct Atheneum *atheneum);
int stikGetConnectedControllers(Stik* self, InputHandle_t* handles);
InputActionSetHandle_t stikGetActionSetHandle(Stik* self, const char* actionSetName);
InputDigitalActionHandle_t stikGetDigitalActionHandle(Stik* self, const char* actionSetName);
int stikActivateActionSet(Stik* self, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle);
int stikUpdate(Stik* self);
InputDigitalActionData_t stikGetDigitalActionData(Stik* input, InputHandle_t controllerHandle, InputDigitalActionHandle_t digitalActionHandle);
ESteamInputType stikGetInputTypeForHandle(Stik* self, InputHandle_t inputHandle);
int stikGetDigitalActionOrigins(Stik* self, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle, InputDigitalActionHandle_t digitalActionHandle, EInputActionOrigin *originsOut);
const char* stikGetInputTypeName(ESteamInputType controllerType);

InputAnalogActionData_t stikGetAnalogActionData(Stik* self, InputHandle_t controllerHandle, InputAnalogActionHandle_t analogActionHandle);
int stikGetAnalogActionOrigins(Stik* self, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle, InputAnalogActionHandle_t analogActionHandle, EInputActionOrigin *originsOut);
InputAnalogActionHandle_t stikGetAnalogActionHandle(Stik* self, const char* analogActionName);

#endif
