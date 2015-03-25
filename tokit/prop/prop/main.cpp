#include <xdsapi.h>
#include <stddef.h>

#include "propapi.h"
#include "powerups.h"

struct PowerUp_t *g_PowerUps = NULL;
int g_PowerUpCnt = 0;

int main(int argc, char **argv)
{
    // propapi::loadprop("powerups.xds", "powerups.dsd", (void**)&g_PowerUps, g_PowerUpCnt);

    propapi::load_gamesvr();

    system("pause");
    return 0;
}
