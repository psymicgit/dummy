///<------------------------------------------------------------------------------
//< @file:   lua_func.cpp
//< @author: hongkunan
//< @date:   2014Äê1ÔÂ7ÈÕ 22:40:30
//< @brief:
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "lua_func.h"
#include <stdio.h>
#include <string.h>
#include <string>

#include <zlib.h>
#include <iostream>
#include "zlibpkg.h"
#include <stdlib.h>

#define PAS(L, n) (lua_tostring(L, n))

#define BUF_SIZE 2048

using namespace std;

extern "C" int lua_zcompress(lua_State* L)
{
    const char* uncompr = lua_tostring(L, 1);;
    uLong uncomprLen = strlen(uncompr);

    Byte compr[BUF_SIZE] = {0};    // big enough
    uLong comprLen = sizeof(compr) / sizeof(compr[0]);

    std::string errmsg;

    int err = compress(compr, &comprLen, (const Bytef*)uncompr, uncomprLen);
    if (err != Z_OK) {
        lua_pushstring(L, NULL);

        switch(err)
        {
        case Z_MEM_ERROR:
            errmsg = "Z_MEM_ERROR";
            break;

        case Z_BUF_ERROR:
            errmsg = "Z_BUF_ERROR";
            break;
        }

        goto ret;
    }
    else
    {
        lua_pushlstring (L, (char*)compr, comprLen);
    }

ret:
    lua_pushinteger(L, comprLen);
    lua_pushstring(L, errmsg.c_str());

    return 3;
}

extern "C" int lua_zdecompress(lua_State* L)
{
    const char* compr = lua_tostring(L, 1);
    uLong comprLen = lua_tointeger(L, 2); //strlen(compr) + 1;//lua_tointeger(L, 2);

    Bytef uncompr[BUF_SIZE] = {0};    // big enough
    uLongf uncomprLen = sizeof(uncompr) / sizeof(uncompr[0]);

    std::string errmsg;

    int err = uncompress(uncompr, &uncomprLen, (const Bytef*)compr, comprLen);
    if (err != Z_OK) {
        lua_pushstring(L, NULL);

        switch(err)
        {
        case Z_MEM_ERROR:
            errmsg = "Z_MEM_ERROR";
            break;

        case Z_BUF_ERROR:
            errmsg = "Z_BUF_ERROR";
            break;

        case Z_DATA_ERROR:
            errmsg = "Z_DATA_ERROR";
            break;
        }

        goto ret;
    }
    else
    {
        lua_pushlstring (L, (char*)uncompr, uncomprLen);
    }

ret:    
    lua_pushinteger(L, uncomprLen);
    lua_pushstring(L, errmsg.c_str());

    return 3;
}

extern "C" int lua_gzcompress(lua_State* L)
{
    const char* uncompr      = lua_tostring(L, 1);
    unsigned int uncompr_len = strlen(uncompr) + 1;

    static char compr[BUF_SIZE] = {0};
    unsigned int compr_len = BUF_SIZE;
    char *errmsg = NULL;

    int ret = gzcompress(uncompr, uncompr_len, compr, &compr_len, &errmsg);
    if(ret || 0 == compr_len)
    {
        lua_pushstring(L, NULL);
        goto ret;
    }
    else
    {
        lua_pushlstring (L, compr, compr_len);
    }

ret:    
    lua_pushinteger(L, compr_len);
    lua_pushstring(L, errmsg);

    return 3;
}

extern "C" int lua_gzdecompress(lua_State* L)
{
    const char* compr       = lua_tostring(L, 1);
    unsigned int compr_len  = lua_tointeger(L, 2);

    static char uncompr[BUF_SIZE] = {0};
    unsigned int uncompr_len = sizeof(uncompr);
    char *errmsg = NULL;

    int ret = gzdecompress(compr, compr_len, uncompr, &uncompr_len, &errmsg);
    if(ret || 0 == uncompr_len)
    {
        lua_pushstring(L, NULL);
        goto ret;
    }
    else
    {
        lua_pushlstring (L, uncompr, uncompr_len);
    }

ret:    
    lua_pushinteger(L, uncompr_len);
    lua_pushstring(L, errmsg);

    return 3;
}

extern "C" int lua_httpgzdecompress(lua_State* L)
{
    const char* compr = lua_tostring(L, 1);
    unsigned int compr_len     = lua_tointeger(L, 2);

    static char uncompr[BUF_SIZE] = {0};
    unsigned int uncompr_Len = sizeof(uncompr);
    char *errmsg = NULL;

    int ret = httpgzdecompress(compr, compr_len, uncompr, &uncompr_Len, &errmsg);
    if(ret || 0 == uncompr_Len)
    {
        lua_pushstring(L, NULL);
        goto ret;
    }
    else
    {
        lua_pushlstring (L, (char*)uncompr, uncompr_Len);
    }

ret:    
    lua_pushinteger(L, uncompr_Len);
    lua_pushstring(L, errmsg);

    return 3;
}

static luaL_Reg zlib_funcs[] = { 
    {"zcompress",        lua_zcompress},
    {"gzcompress",       lua_gzcompress},
    {"zdecompress",      lua_zdecompress},
    {"gzdecompress",     lua_gzdecompress},
    {"httpgzdecompress", lua_httpgzdecompress},
    
    {NULL, NULL} 
}; 

extern "C" int luaopen_zlib(lua_State* L)
{
    luaL_register(L, "zlib", zlib_funcs);
    return 1;
}
