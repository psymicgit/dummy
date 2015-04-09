#-*- coding: utf-8 -*-
'''
Created on 2013-1-20

@author: zhouliang
'''
from common.log import GetLogger
import dblogic
from globalsetting import *
import threading
import traceback
import copy
from common import commfunc
import globalsetting



#玩家数据
class PlayerData:
    def __init__(self):
        self.nPlayerID=-1 #玩家ID
        
        self.listOrderInfo=[] #订单号列表，内容为元组,1为订单，2为钱
        
    def DelOrderInfo( self, strOrderID ):
        for orderInfo in self.listOrderInfo:
            if orderInfo[0]==strOrderID:
                GetLogger().debug( '删除订单:%s'%( str( orderInfo ) ) )
                self.listOrderInfo.remove( orderInfo )
                
                
        
__dicOrder={}

def Init():
    try:
        #查看所有有充值到但是没有被gs处理过的
        strSql="select * from %s where IsProcess=0 and DateExchange is not NULL order by DateExchange"%globalsetting.globalinfo.DB_EXCHANGE_TABLE_NAME
        
        result=dblogic.logicprocess.GetDBObj().RunSql( strSql )
        
        for feedbackInfo in result:
            #遍历，并且插入
            
            if not __dicOrder.has_key( feedbackInfo['PlayerID'] ):
                data=PlayerData()
                data.nPlayerID=feedbackInfo['PlayerID']
                __dicOrder[ feedbackInfo['PlayerID'] ]=data
                
            __dicOrder[ feedbackInfo['PlayerID'] ].listOrderInfo.append( (str(feedbackInfo['OrderID']), feedbackInfo['Amount']) )
            try:                
    
                GetLogger().info( "读取到玩家:%d 未处理的订单数据是:%s"%( data.nPlayerID, str( feedbackInfo['OrderID'] ) ) )
            except:
                GetLogger().error( "获得的反馈数据是:%s"%str(feedbackInfo) )
                GetLogger().error( traceback.format_exc() )
                 
            
            
        return True
    except:
        GetLogger().error( traceback.format_exc() )
        
        return False

def UnInit():
    pass

__lock=threading.Lock()


#添加数据,用于通知gameserver有人充值
def __AddData( nPlayerID, strOrderID, nAmount, dbObj ):
    if not __dicOrder.has_key( nPlayerID ):
        data=PlayerData()
        data.nPlayerID=nPlayerID
        __dicOrder[nPlayerID]=data
        
    __dicOrder[nPlayerID].listOrderInfo.append( (strOrderID, nAmount) )
    return True
    

#添加数据,用于通知gameserver有人充值
def AddData( nPlayerID, strOrderID, nAmount, dbObj ):
    commfunc.ScopeLock(__lock)
    try:
        ret=False
        ret=__AddData(nPlayerID, strOrderID, nAmount, dbObj)
        
    except:
        GetLogger().error( traceback.format_exc() )

    
    return ret
        
#返回到实际内容是PlayerData
def __GetDataByPlayerID( nPlayerID ):
    if not __dicOrder.has_key( nPlayerID ):
        GetLogger().warning( 'GetDataByPlayerID无法获得数据通过PlayerID:'+str(nPlayerID) )
        return None
    
    return __dicOrder[nPlayerID] 

#返回到实际内容是PlayerData
def GetDataByPlayerID( nPlayerID ):
    commfunc.ScopeLock(__lock)
    try:
        ret=None
        ret=__GetDataByPlayerID( nPlayerID )
    except:
        GetLogger().error( traceback.format_exc() )
        
    
    return ret

#返回全部首个的充值信息的描述,可直接用于网络传输
def GetAllFirstOneDesInfo():
    commfunc.ScopeLock(__lock)
    try:
        dicRet={}
        for key,value in __dicOrder.items():
            if len( value.listOrderInfo )==0:
                GetLogger().warning( '无法得到玩家:%s 的任何充值信息描述'%str(key) )
                continue
            
            allOrderData=value.listOrderInfo[0]
            
            dicRet[ '%s'%key ]={ 'OrderID':'%s'%allOrderData[0], 'Amount':int(allOrderData[1]) }
            
    except:
        GetLogger().error( traceback.format_exc() )
    
    return str( dicRet ).replace('\'', '"')

#删除已经处理的PlayerID的订单
def __DelData( nPlayerID, strOrderID ):
    if not __dicOrder.has_key( nPlayerID ):
        GetLogger().warning( 'DelData无法获得数据通过PlayerID:'+str(nPlayerID) )
        return None
    
    GetLogger().debug( '删除已处理订单前的数据长度:%d'%len( __dicOrder[nPlayerID].listOrderInfo ) )
    
    __dicOrder[nPlayerID].DelOrderInfo( strOrderID )
    
    GetLogger().debug( '删除已处理订单后的数据长度:%d'%len( __dicOrder[nPlayerID].listOrderInfo) )
    
    if len( __dicOrder[nPlayerID].listOrderInfo )>0:
        return True
    
    else:        
        __dicOrder.pop( nPlayerID )
        GetLogger().debug( '删除玩家充值反馈成功, nPlayerID:%d'%nPlayerID )
        return True

#删除已经处理的PlayerID的订单,返回真假
def DelData( nPlayerID, strOrderID ):
    commfunc.ScopeLock(__lock)
    try:
        ret=False
        ret=__DelData( nPlayerID, strOrderID )
    except:
        GetLogger().error( traceback.format_exc() )
        
    
    return ret