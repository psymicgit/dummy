#-*- coding: utf-8 -*-
'''
Created on 2013-1-20

@author: zhouliang
'''
import asyncore, socket
import ConfigParser
from globalsetting import *
import common
from common.log import GetLogger
import traceback
import common.mnpackparse
import dblogic
import uuid,copy
import process
import time
import globalsetting
from common import commfunc
import urllib
import threading

#接收到数据的处理
class CPackDispatcher:
    def __init__(self, client):
        self.__client=client
        
    ## //请求生成GUID,成功返回0
    def  _OnRecv_0094(self, lpData):
        pos=0
        nLen, pos=common.commfunc.ReadWORD( lpData , pos)
        GetLogger().debug( 'pos=%d nLen=%d Data:%s'%( 
                                pos, nLen, common.commfunc.BinToStr( lpData[pos:] ) ) )
        strInfo, pos=common.commfunc.ReadString(lpData, pos, nLen)
        
        
        GetLogger().debug( '接收到请求GUID的信息是:%s'%strInfo )
        try:
            dictParam=eval( strInfo )
        except:
            GetLogger().error( traceback.format_exc() )
            GetLogger().error( '接收到请求GUID的信息是:%s'%( strInfo ) )
            return
        
        strGuid=str( uuid.uuid1() )
        strGuid=globalsetting.config.GetMainConfig().get('platform', 'ServerName')+'_'+strGuid
        dictResult={}
        dictResult['OrderID']=strGuid
        if dblogic.logicprocess.CreateOrderInfo( strGuid, dictParam['PlayerID'], 
                                                 dictParam['PlayerLevel'], dictParam['ZoneID'], int(dictParam['UserID']) ):
            #成功返回结果给gs
            dictResult['Result']=1
            
        else:
            #返回失败的信息给gs
            dictResult['Result']=0
            
        dictResult['PlayerID']=dictParam['PlayerID']
        
        strResult=str( dictResult )
        
        strResult=strResult.replace( '\'' , '"')
 
        buffSend=''
        nDataLen=len(strResult)
        buffSend=common.commfunc.WriteWORD( buffSend, nDataLen )
        buffSend=common.commfunc.WriteString( buffSend , nDataLen, strResult)
        buffSend=self.__client.packParse.PackData( buffSend, globaldefine.RECGS_GEN_GUID_ACK )
        GetLogger().info( '创建订单,返回数据:%s'%buffSend )
        self.__client.sendBuffer+=buffSend
        
    #收到订单处理完毕
    def _OnRecv_0097(self, lpData):
        pos=0
        nLen, pos=common.commfunc.ReadWORD( lpData , pos)
        strInfo, pos=common.commfunc.ReadString(lpData, pos, nLen)
        
        GetLogger().info( '订单处理GS返回的内容是:%s'%( strInfo ) )
        try:
            dictParam=eval( strInfo )
        except:
            GetLogger().error( traceback.format_exc() )
            GetLogger().error( '接收到订单处理结果的信息是:%s'%( strInfo ) )
            return
        
        if ( dictParam["ResultCode"]==globaldefine.PROCESS_EXCHANGE_SUCCESS or 
            dictParam["ResultCode"]==globaldefine.PROCESS_EXCHANGE_ORDER_ID_REPEAT ):
            dblogic.logicprocess.SetExchangeProcessed(dictParam["OrderID"], 
                        dictParam["ResultCode"]==globaldefine.PROCESS_EXCHANGE_ORDER_ID_REPEAT)
            process.orderdata.DelData( dictParam["PlayerID"], dictParam["OrderID"] )
            
            return
                    
        GetLogger().error( '订单处理错误，GS返回的内容是:%s'%( strInfo ) )
        
    #收到心跳
    def _OnRecv_0093(self, lpData):
        GetLogger().debug( 'recv heart' )
       
    def OnRecvPacket(self, lpData, nhead ):
        GetLogger().debug( 'recv data:%s'%( common.commfunc.BinToStr(lpData) ) )  
        strFuncName="_OnRecv_%04d"%(nhead)      
        if hasattr(self, strFuncName):
            try:
                GetLogger().info("%s, PackLen=%d"%(strFuncName, len(lpData)))
                getattr(self, strFuncName)(lpData)
            except:
                GetLogger().error("%s fail! %s"%( strFuncName, traceback.format_exc() ) )
            return
        GetLogger().warning("未定义封包的处理函数:%s"%(strFuncName))
        

#与gameserver连接的处理类
class Client(asyncore.dispatcher):
    __MAX_RECV_LEN=8192*5
    def __init__(self, socket):
        asyncore.dispatcher.__init__(self, socket)
        self.sendBuffer = ''
        self.packParse=common.mnpackparse.CPackParse( self )
        self.packParse.OnPackRecv=self.OnRecvPacket
        self.packDispatcher=CPackDispatcher( self )
        self.lastSendHeartClock=0
        self.__lock=threading.Lock()
        
    def handle_error(self):
        GetLogger().error( 'client error' )

    def handle_close(self):
        self.close()
        self.sendBuffer=''
        self.packParse.Clear()
        
        GetLogger().error( 'disconnect from gs,'+'Addr:'+str(self.addr) )
        
        global _server
        _server.DelClient()

    def handle_read(self):
        strRecv=self.recv( Client.__MAX_RECV_LEN )
        if not strRecv:
            #select连接断开标记
            #self.close()
            pass
        
        self.packParse.OnRecvOrignPack( strRecv )
        
    def writable(self):
        #如果第一次连接，则直接返回成功,以便回调connect
        if not self.connected:
            return True
        
        return len(self.sendBuffer) > 0

    def handle_write(self):
        sent = self.send(self.sendBuffer)
        self.sendBuffer = self.sendBuffer[sent:]
        GetLogger().debug( '发送了数据,长度:%s'%sent )
        
    def OnRecvPacket(self, curPack, head, client ):
        try:
            self.packDispatcher.OnRecvPacket( curPack, head )
        except:
            GetLogger().error( '收到封包失败:%s'%traceback.format_exc() )
            
    def Send(self, strBuff):
        commfunc.ScopeLock(self.__lock)
        self.sendBuffer+=strBuff
        
            
class ListenServer( asyncore.dispatcher ):
    def __init__(self, nPort):
        asyncore.dispatcher.__init__(self)
        self.client = None  
        #建立等待的socket  
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)  
        self.bind(('', nPort))  
        self.listen(5)
        
    def handle_accept(self):  
        #接受client socket的連線  
        sock, address = self.accept()  
        GetLogger().info( 'New client from : ' + str(address) )
        
        if( self.client ):
            GetLogger().info( 'already connect address:%s refuse connect'%( str(self.client.addr) ) )
            sock.close()
            return
            
        #自定义的客户端类
        self.client = Client(sock)
        
    def DelClient(self):
        self.client=None
    

cfg=config.GetMainConfig()
strHost=cfg.get( 'gameserver' , 'Host')
nPort=cfg.getint( 'gameserver' , 'Port')
_server=ListenServer( nPort )
    
__lastSendTime=time.time()

def CheckAndSendAllOrderData():
    global __lastSendTime,_server
    
    if( _server.client==None ):
        return
    
    curTime=time.time()
    
    if ( curTime - __lastSendTime ) < globalsetting.globaldefine.DEF_SEND_ALL_INTERVAL:
        return
    
    __lastSendTime=time.time()
    
    sendInfo=process.orderdata.GetAllFirstOneDesInfo()
    
    if sendInfo=='{}':
        return
    GetLogger().debug( '发送所有订单数据,len:%d'%len( sendInfo ) )
    
    buffSend=''
    nDataLen=len(sendInfo)
    buffSend=common.commfunc.WriteWORD( buffSend, nDataLen )
    buffSend=common.commfunc.WriteString( buffSend , nDataLen, sendInfo)
    buffSend=_server.client.packParse.PackData( buffSend, globaldefine.RECGS_PROCESS_ORDER_ID_REQ )
    
    _server.client.Send( buffSend )
    GetLogger().debug( 'send to gameserver:%s'%buffSend )
    
def CheckAndSendHeartMsg():
    global __lastSendTime,_server
    
    if( _server.client==None ):
        return
    
    if time.clock()-_server.client.lastSendHeartClock>globaldefine.DEF_HEART_INTERVAL:
        buffSend=''
        buffSend=common.commfunc.WriteBYTE( buffSend, 0 )
        buffSend=_server.client.packParse.PackData( buffSend, globaldefine.RECGS_HEART_REQ )
        _server.client.Send( buffSend )
        
        GetLogger().debug( '发送心跳' )
        _server.client.lastSendHeartClock=time.clock()

#处理每一帧的逻辑，与gs进行交互
def Process():
    try:       
        
        #数据发送和接收
        asyncore.loop( globalsetting.globaldefine.DEF_SELECT_INTERVAL_TIME, False, None, 1)
        
#        if not IsConnected():
#            Connect()
#        else:
#            CheckAndSendHeartMsg()
        
    except:
        GetLogger().error( traceback.format_exc() )

#def IsConnected():
#    return __client.connected


#进行连接,如果已经连接则直接返回True
#def Connect():
#    try:
#        if __client.connected:
#            return
#        
#        if __client.isConnecting:
#            return
#        
#        __client.Connect()
#        
#    except:
#        __client.close()
#        GetLogger().error( traceback.format_exc() )


