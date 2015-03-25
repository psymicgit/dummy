#include "random.h"
#ifdef WIN32
#define _CRT_RAND_S
#include <assert.h>
#endif
#include <stdlib.h>
#include <time.h>

namespace nsTool
{
	//const static float S_PI = 3.14159265358979323846;
	RandomGen::RandomGen() {
		srand((unsigned int)time(NULL));
		//srand(GetTickCount());
	}

	RandomGen::~RandomGen() {
	}

	unsigned int RandomGen::getRand() {
#ifdef WIN32
		static unsigned int uiRet;
		if (0 != rand_s(&uiRet)) {
			return (unsigned int)-1;
		}

		return uiRet;
#endif
#ifdef __LINUX__
		return rand();
#endif // WIN32

	}


	unsigned int RandomGen::getRandMax() {
#ifdef WIN32
		return UINT_MAX;
#endif
#ifdef __LINUX__
		return (unsigned int)RAND_MAX + 1;
#endif // WIN32

	}

	unsigned int RandomGen::getRand(unsigned int uiMax) {
		if (0 == uiMax) {
			return 0;
		}
		return (unsigned int)((double)getRand() / (double) getRandMax() * uiMax) ;
	}

	float RandomGen::getRandFloat(float fRangle) {
		//static	float fToRand;
		//fToRand = fRangle;
		//if (0 > fToRand) {
		//	fToRand = -fToRand;
		//}
		//static const float MAXRANGLE = 1000000;
		//unsigned int uiRand = getRand((unsigned int)(fToRand * MAXRANGLE));
		//float fRet = (float)uiRand/MAXRANGLE;
		//if (0 > fRangle) {
		//	fRet = -fRet;
		//}

		return (float) getRand() / (float) getRandMax() * fRangle ;
	}

	int RandomGen::getRandInt(int iMax) {
		if (0 == iMax) {
			return 0;
		}
		return (int)((float)getRand() / (float) getRandMax() * iMax);
	}

	double RandomGen::gaussian(double mu, double sigma) {
		if(sigma <= 0.0) { 
			//printf("Sigma<=0.0 in _sta!");
			//assert(0);
			return 0;
		} 

		double sum = 0.0; 
		for(int i = 1; i <= 12;i++) {
			sum = sum + getRandFloat(1); 
		}
		return (sum - 6.00) * sigma + mu; 
	}

	int RandomGen::gaussian(int mu, unsigned int sigma, unsigned int rangle) {
		int iaussian = (int)gaussian((double)mu, (double)sigma);
		if (iaussian < mu - (int)rangle) {
			iaussian = mu - (int)rangle;
		} else if (iaussian > mu + (int)rangle) {
			iaussian = mu + (int)rangle;
		} 
		return iaussian;
	}

	int RandomGen::gaussianHalf(int mu, unsigned int sigma, unsigned int rangle) {
		int iaussian = (int)gaussian((double)mu, (double)sigma);
		int iDe = abs(iaussian - mu);
		if (iaussian > int(mu + rangle)) {
			iaussian = mu + rangle;
		} 
		return iDe <= (int)rangle ? mu + iDe : mu + (int)rangle;
	}
}
