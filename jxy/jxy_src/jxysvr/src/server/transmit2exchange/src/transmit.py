#-*- coding: utf-8 -*-
'''
Created on 2013-4-8

@author: zhouliang
'''

from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler
from SocketServer import ThreadingMixIn
from common import *
import config
import httplib
import traceback
from common.log import GetLogger

DEF_PARAM_ERROR=11
DEF_EXE_ERROR=12

def GetErrorStr( nErrorCode ):
    return ("{'status':'error_%d'}"%nErrorCode).replace( '\'' , '"')

#http请求处理函数
class RequestHandler( BaseHTTPRequestHandler ):
    
    def do_GET(self):

        #不处理浏览器的logo请求
        if( self.path=='/favicon.ico' ):
            return
        
        dicParam=commfunc.GetHttpParam( self.path )
        
        if dicParam=='':
            self.wfile.write( GetErrorStr(DEF_PARAM_ERROR) )
            return
        
        try:
            strServerName=str.split( dicParam['trade_no'], '_' )[0]
            strServerPath=config.ServerList[strServerName][0]
            nPort=config.ServerList[strServerName][1]
            conn=httplib.HTTPConnection( strServerPath, nPort )
            conn.request('GET', self.path)    
            strRet=conn.getresponse().read()
            
            self.wfile.write( strRet )
            
        except:
            GetLogger().warning( '发生了错误:%s path:%s'%( traceback.format_exc(), self.path ) )
            self.wfile.write( ("{'status':'error_12'}").replace( '\'' , '"') )
        	self.close_connection();
        	
        #self.wfile.write( strRet )
        
        
class ThreadingHTTPServer( ThreadingMixIn, HTTPServer ):
    pass

#处理http请求的服务器
__serverAddr=('',config.ListenPort)
_srv=ThreadingHTTPServer( __serverAddr, RequestHandler )

import os
def Begin():
    global _srv
    #日志初始化
    logPath = os.getcwd() + "/log";
    if not os.path.exists(logPath):
        os.mkdir(logPath)
    log.InitLog(logPath, "exchange", config.PrintLog, config.Debug)
    
    GetLogger().info( 'Server start...' )
    #服务器启动
    _srv.serve_forever()
    
    
if __name__=='__main__':
    Begin()