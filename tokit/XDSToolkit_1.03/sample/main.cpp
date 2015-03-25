#include <stdio.h>
#include <string.h>
#include "XDSliteAPI.h"

#include "powerups.h"
#define DEFINE_DSD
#include "powerups_dsd.h"

int g_iPowerUpCount = 0;
//unsigned short iElemType;
struct PowerUp_t *g_PowerUps;


void XDS_PROCESSNODE(unsigned short  nodeType,
					 void           *nodeData,
					 unsigned long   nodeSize)
{
	printf("PROCESSNODE: 0x%04x=", nodeType);
	switch(nodeType)
	{
	case XDS_Powerups_Powerup:
		printf("RECORD XDS_Powerups_Powerup\n");
		// record start -- do nothing
		break;

	case XDS_Powerups_g_PowerUps:
		printf("ELEMENT XDS_Powerups_g_Powerups\n");
		// powerup data -- save it
		g_PowerUps = (struct PowerUp_t *)nodeData;
		g_iPowerUpCount = nodeSize / sizeof(struct PowerUp_t);
		break;

	case XDS_RECORD_COMMENT:
		printf("COMMENT '%s'\n", nodeData);
		break;

	case XDS_RECORD_ENDOFDATA:
		printf("END-OF-DATA\n");
		break;

	default:
		printf("<unknown>\n");
		break;
	}
}

void printIf(int iValue, char *szFormat)
{
	if(iValue)
		printf(szFormat, iValue);
}
void printPowerups(void)
{
	printf("%d Power-ups:\n", g_iPowerUpCount);
	for(int ii = 0; ii < g_iPowerUpCount; ++ii)
	{
		printf("%10s  ", g_PowerUps[ii].szName);
		printf("%16s  ", g_PowerUps[ii].szImage);
//		if(g_PowerUps[ii].fHealth != 0.0)
//            printf("Health[%f] ", g_PowerUps[ii].fHealth);
		printIf(g_PowerUps[ii].iHealth, "Health[%d] ");
		printIf(g_PowerUps[ii].iInvulnerability, "Invulnerability[%d] ");
		printIf(g_PowerUps[ii].iFastMove, "FastMove[%d] ");
		printIf(g_PowerUps[ii].iHighJump, "HighJump[%d] ");
		printIf(g_PowerUps[ii].iStunPlayer, "Stun[%d] ");
		printIf(g_PowerUps[ii].iLifeUp, "LifeUp[%d] ");
		printf("\n");
	}
}

char* err()
{
    static char buf[2048];
    strerror_s(buf, errno);
    // strerror(errno)

    return buf;
}

void readPowerups(char *xds)
{
	FILE *pxdsfile = NULL;
	if(fopen_s(&pxdsfile, xds, "rb"))
	{
		fprintf(stderr, "Cannot open '%s' - %s\n", xds, err());
		exit(1);
	}

	printf("Reading powerups from '%s'\n", xds);

/*
    #ifdef XDS_SUPPORT_DEFTYPE
	    struct xdsHandle *h = xdsInit(fp, "Powerups", NULL);
	    regDsd(h);
    #else
	    struct xdsHandle *h = xdsInit(fp, "Powerups", &XDSDSD_Powerups[0]);
    #endif
*/
    FILE *pdsdfile = NULL;
    if(fopen_s(&pdsdfile, "powerups.dsd", "rb"))
    {
        fprintf(stderr, "Cannot open '%s' - %s\n", "powerups.dsd", err());
    }

    // 获取文件大小
    fseek (pdsdfile, 0 , SEEK_END);  
    int dsdlen = ftell (pdsdfile);  
    rewind (pdsdfile);

    static unsigned char buf[10240];

    /* 将文件内容一次性全部拷贝到buffer中 */  
    if (fread (buf, 1, dsdlen, pdsdfile) != dsdlen)  
    {  
        fprintf(stderr, "Reading '%s' error: %s\n", "powerups.dsd", err());
        return;
    }    

    struct xdsHandle *h = xdsInit(pxdsfile, "Powerups", buf);
    if(NULL == h)
    {
        return;
    }

	while(xdsReadRecord(h))
		;
	xdsFini(h);
	fclose(pxdsfile);
}

int main(int argc, char **argv)
{
	readPowerups("powerups.xml");
	printPowerups();

	XDS_FREE(g_PowerUps);
	g_iPowerUpCount = 0;

	readPowerups("powerups.xds");
    printPowerups();

    system("pause");

    return 0;
}