/*
** Lua binding: toluainterface
** Generated automatically by tolua++-1.0.92 on 07/17/13 21:49:07.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_toluainterface_open (lua_State* tolua_S);

#include <sdtype.h>
#include <logic/player/player.h>
#include <logic/record/recordmgr.h>
#include <protocol/server/protocommondata.h>
#include <logic/base/basedefine.h>
#include <logic/lua/toluafunc.h>
#include <logic/lua/luamgr.h>

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_UINT32 (lua_State* tolua_S)
{
 UINT32* self = (UINT32*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_UINT64 (lua_State* tolua_S)
{
 UINT64* self = (UINT64*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CHAR");
 tolua_usertype(tolua_S,"UINT32");
 tolua_usertype(tolua_S,"CPlayer");
 tolua_usertype(tolua_S,"UINT64");
}

/* function: GetTimeValue */
#ifndef TOLUA_DISABLE_tolua_toluainterface_GetTimeValue00
static int tolua_toluainterface_GetTimeValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CHAR",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CHAR* pszDateTime = ((const CHAR*)  tolua_tousertype(tolua_S,1,0));
  {
   UINT64 tolua_ret = (UINT64)  GetTimeValue(pszDateTime);
   {
#ifdef __cplusplus
    void* tolua_obj = new UINT64(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"UINT64");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(UINT64));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"UINT64");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTimeValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetRank */
#ifndef TOLUA_DISABLE_tolua_toluainterface_GetRank00
static int tolua_toluainterface_GetRank00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CPlayer* poPlayer = ((CPlayer*)  tolua_tousertype(tolua_S,1,0));
  {
   UINT32 tolua_ret = (UINT32)  GetRank(poPlayer);
   {
#ifdef __cplusplus
    void* tolua_obj = new UINT32(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"UINT32");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(UINT32));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"UINT32");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRank'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: LuaRandomReseed */
#ifndef TOLUA_DISABLE_tolua_toluainterface_LuaRandomReseed00
static int tolua_toluainterface_LuaRandomReseed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int dwRange = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  {
   int tolua_ret = (int)  LuaRandomReseed(dwRange);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LuaRandomReseed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_toluainterface_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_function(tolua_S,"GetTimeValue",tolua_toluainterface_GetTimeValue00);
  tolua_function(tolua_S,"GetRank",tolua_toluainterface_GetRank00);
  tolua_function(tolua_S,"LuaRandomReseed",tolua_toluainterface_LuaRandomReseed00);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_toluainterface (lua_State* tolua_S) {
 return tolua_toluainterface_open(tolua_S);
};
#endif

