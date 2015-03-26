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
#include "base64.h"

#include "rsa.h"

#define PAS(L, n) (lua_tostring(L, n))

extern "C" int rsalib_helloLua(lua_State* L)
{
    printf("Hello Lua, I'm in Cpp! Lua's message is: %s/n", PAS(L, 1));
    return 0;
}

extern "C" int lua_public_key_decrypt(lua_State* L)
{
    const char* encrypted_text = lua_tostring(L, 1);
    const char* public_key   = lua_tostring(L, 2);

    char *errmsg = NULL;

    int encrypted_len = strlen(encrypted_text);
    int decrypt_len = 0;

	char *decrypt_text = NULL;
    size_t decode_len = 0;

    const unsigned char *decoded_text = base64_decode((unsigned char*)encrypted_text, encrypted_len, &decode_len);
    if(NULL == decoded_text || decode_len == 0)
    {
        goto ret;
    }

    decrypt_text = public_key_decrypt((char*)decoded_text, decode_len, public_key, &decrypt_len, &errmsg);
    if(NULL == decrypt_text || decrypt_len == 0)
    {
        goto ret;
    }

ret:    
    lua_pushstring(L, decrypt_text);
    lua_pushinteger(L, decrypt_len);
    lua_pushstring(L, errmsg);

    return 3;
}

extern "C" int lua_private_key_sign(lua_State* L)
{
    const char* plain_text  = lua_tostring(L, 1);
    const char* private_key = lua_tostring(L, 2);

    char *errmsg = NULL;

    unsigned int sign_len = 0;
    char *sign_text = private_key_sign(plain_text, strlen(plain_text), private_key, &sign_len, &errmsg);
    if(NULL == sign_text || sign_len == 0)
    {
        goto ret;
    }

ret:
    lua_pushlstring (L, sign_text, sign_len);
    lua_pushinteger(L, sign_len);
    lua_pushstring(L, errmsg);

    return 3;
}

extern "C" int lua_public_key_verify(lua_State* L)
{
    const char* plain_text  = lua_tostring(L, 1);
    const char* base64_sign_text  = lua_tostring(L, 2);
    const char* public_key = lua_tostring(L, 3);

    char *errmsg = NULL;

    int base64_sign_len = strlen(base64_sign_text);

    size_t sign_len = 0;
    const unsigned char *sign_text = base64_decode((unsigned char*)base64_sign_text, base64_sign_len, &sign_len);

    bool ok = public_key_verify(plain_text, strlen(plain_text), (char*)sign_text, sign_len, public_key, &errmsg);
    lua_pushboolean(L, ok);
    lua_pushstring(L, errmsg);

    return 2;
}

extern "C" int lua_base64_encode(lua_State* L)
{
    const char* plain_text  = lua_tostring(L, 1);
    int plain_len = lua_tointeger(L, 2);

    size_t base64_len = 0;
    const unsigned char *base64_text = base64_encode((unsigned char*)plain_text, plain_len, &base64_len);

    lua_pushstring(L, (char*)base64_text);
    return 1;
}

extern "C" int lua_base64_decode(lua_State* L)
{
    const char* base64_text  = lua_tostring(L, 1);

    int base64_len = strlen(base64_text);

    size_t plain_len = 0;
    const unsigned char *plain_text = base64_decode((unsigned char*)base64_text, base64_len, &plain_len);

    lua_pushstring(L, (char*)plain_text);
    return 1;
}

static luaL_Reg rsa_funcs[] = { 
    {"public_key_decrypt", lua_public_key_decrypt},
    {"private_key_sign"  , lua_private_key_sign},
    {"public_key_verify" , lua_public_key_verify},
    {"base64_encode"     , lua_base64_encode},
    {"base64_decode"     , lua_base64_decode},
    
    {NULL, NULL} 
}; 

extern "C" int luaopen_rsa(lua_State* L)
{
    luaL_register(L, "rsa", rsa_funcs);
    return 1;
}
