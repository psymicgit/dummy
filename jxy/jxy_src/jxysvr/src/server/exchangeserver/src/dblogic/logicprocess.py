#-*- coding: utf-8 -*-
'''
Created on 2013-1-18

@author: zhouliang
'''
import ConfigParser
from globalsetting import *
import traceback
import common
import string,base64
import dbobj
from common.log import GetLogger
from common import commfunc
from string import replace
import datetime


DEF_SAVE_ORDER_SUCCESS=0 #保存数据成功
DEF_SAVE_ORDER_ERROR_CANNOT_FIND_ORDER=1 #找不到订单
DEF_SAVE_ORDER_ERROR_DB_ERROR=2 #数据库处理错误

__dbObj=dbobj.DBObj()


def GetDBObj():
    return __dbObj

#设置充值信息已经被处理
def SetExchangeProcessed( strOrderID, bRepeat ):
    strSql="update %s set DateProcess=now(), IsProcess=1 where OrderID='%s' and IsProcess=0"%( 
                globalinfo.DB_EXCHANGE_TABLE_NAME, strOrderID )
    if __dbObj.ExecSql(strSql) > 0:
        GetLogger().info( 'SetExchangeProcessed succees!' )
        return True
    else:
        if not bRepeat:
            GetLogger().error( 'SetExchangeProcessed保存数据失败,执行SQL语句:%s'%strSql )
        else:
            GetLogger().warning( 'SetExchangeProcessed保存数据失败,可能是重复设置数据，执行SQL语句:%s'%strSql )
        return False
        
#保存数据到DB,只要保存成功了，即可返回成功，即使统计数据操作失败也不用返回失败
def SaveExchangeInfo( strOrderID, strSource, strAmount, strPartner ):
    strSql="update %s set Source='%s', Amount='%d', Partner='%s', DateExchange=now() where OrderID='%s' and Amount IS NULL"%( 
                globalinfo.DB_EXCHANGE_TABLE_NAME, strSource, int(strAmount), strPartner, strOrderID )
    if __dbObj.ExecSql(strSql) <= 0:
        GetLogger().error( 'save data error,exec SQL,may be repeat insert:%s'%strSql )
        return False
    
    strSql="select * from %s where OrderID='%s'"%( globalinfo.DB_EXCHANGE_TABLE_NAME, strOrderID )
    
    retList=__dbObj.RunSql(strSql)
    if not retList or len( retList )<=0 :
        GetLogger().warning( "can't find data, sql:%s"%strSql )
        return True
    
    nUserID=retList[0]['UserID']
    nPlayerID=retList[0]['PlayerID']
    nZoneID=retList[0]['ZoneID']
    
    #如果global充值表中不存在该角色，那么创建
    strSql="select * from %s where UserID='%d' and PlayerID='%d'"%( globalinfo.DB_GLOBAL_AMOUNT_TABLE_NAME, nUserID, nPlayerID )
    retList=__dbObj.RunSql(strSql)
    if not retList:
        strSql="Insert into %s( UserID, PlayerID, TotalAmount, ZoneID, LastExchangeDay, RecentDayAmount ) values( '%d', '%d', 0, '%d', '%s', 0 )"%(
                    globalinfo.DB_GLOBAL_AMOUNT_TABLE_NAME, nUserID, nPlayerID, nZoneID, datetime.datetime.today().date() )
        if __dbObj.ExecSql(strSql) <= 0:
            GetLogger().warning( 'generate global record exchange error,PlayerID:%d UserID:%d'%( nPlayerID, nUserID ) )
    
    #创建完后再查找，如果不存在则报错
    strFindGlobalSql="select * from %s where UserID=%d and PlayerID=%d"%( globalinfo.DB_GLOBAL_AMOUNT_TABLE_NAME, nUserID, nPlayerID )
    retFindGlobalList=__dbObj.RunSql(strFindGlobalSql)
    if not retFindGlobalList or len( retFindGlobalList )<=0 :
        GetLogger().warning( "can't find data, sql:%s"%strSql )
        return True
    
    
    
    lastTime=retFindGlobalList[0]['LastExchangeDay']
    todayDate=datetime.datetime.today().date()
    nDayAmount=retFindGlobalList[0]['RecentDayAmount']
    if( lastTime !=todayDate ):
        nDayAmount=int(strAmount)
        strSetDateSql="update %s set LastExchangeDay='%s' where UserID='%d' and PlayerID=%d"%( 
                            globalinfo.DB_GLOBAL_AMOUNT_TABLE_NAME, str(todayDate), nUserID, nPlayerID )
    #strSql="update %s set TotalAmount=TotalAmount+'%d' "%( globalinfo.DB_GLOBAL_AMOUNT_TABLE_NAME, int(strAmount) )
        if __dbObj.ExecSql(strSetDateSql) <= 0:
            GetLogger().warning( "can't update data, sql:%s"%strSetDateSql )
            return True
    else:
        nDayAmount+=int(strAmount)
    #RecentDayAmount
    strUpdateGlobalSql="update %s set TotalAmount=TotalAmount+'%d',RecentDayAmount='%d' where UserID='%d' and PlayerID='%d'"%( 
                            globalinfo.DB_GLOBAL_AMOUNT_TABLE_NAME, int(strAmount), nDayAmount, nUserID, nPlayerID )
    if __dbObj.ExecSql(strUpdateGlobalSql) <= 0:
        GetLogger().error( 'update error,SQL:%s'%strUpdateGlobalSql )
        return True
    GetLogger().info( 'table %s set Player:%d User:%d add amount:%d todayDate:%s'%( 
                            globalinfo.DB_GLOBAL_AMOUNT_TABLE_NAME, nPlayerID, nUserID, int(strAmount), str(todayDate) ) )
    return True
    
    
    
#根据订单号，获得数据库段
def GetDataByOrderID( strOrderID ):
    strSql="select * from %s where OrderID='%s'"%( globalinfo.DB_EXCHANGE_TABLE_NAME, strOrderID )
    retList=__dbObj.RunSql(strSql)
    if not retList:
        GetLogger().error( '根据订单号获得数据失败，sql:%s 结果:%s'%( strSql, str(retList) ) )
        return False
    
    if len(retList)>0:
        return retList
    else:
        GetLogger().error( '根据订单号获得数据失败，sql:%s'%strSql )
        return False

#创建订单号,成功返回订单号，失败返回None
def CreateOrderInfo( strOrderID, nPlayerID, nPlayerLevel, nZoneID, nUserID ):
    
    strSql="Insert into %s ( OrderID, PlayerID, DateCreate, IsProcess, PlayerLevel, ZoneID, UserID ) \
                values( '%s', '%d', now(), 0, '%d', '%d', '%d' )"%( 
                    globalinfo.DB_EXCHANGE_TABLE_NAME, strOrderID, nPlayerID, nPlayerLevel, nZoneID, nUserID )
    if __dbObj.ExecSql(strSql) == 0:
        return False
        
    return True
