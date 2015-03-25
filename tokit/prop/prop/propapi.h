#ifndef _propapi_h_
#define _propapi_h_

#include <string>

using namespace std;

namespace propapi
{
    void loadprop(char *xds, char *dsd, void **prop, int &len);

    void load_gamesvr();
}

#endif // _propapi_h_
