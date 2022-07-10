#ifndef STIK_H
#define STIK_H

#include <steam-api/steam_api.h>

typedef void* ISteamInput;


struct Atheneum;

typedef struct Stik {
    ISteamInput* input;
} Stik;

int stikInit(Stik* self, struct Atheneum *atheneum);

#endif
