#-*- coding: utf-8 -*-
'''
Created on 2013-1-18

@author: zhouliang
'''
from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler
from SocketServer import ThreadingMixIn
import io,shutil,urllib
import time, threading
import ConfigParser
from globalsetting import *
import string
from common import *
import common
from common.log import GetLogger
import traceback
from platform import *
from platform import exchange


cfg=config.GetMainConfig()
__nPort=string.atoi( cfg.get( 'http' , 'Listenport' ) )

#http请求处理函数
class RequestHandler( BaseHTTPRequestHandler ):
    
    def do_GET(self):

        #不处理浏览器的logo请求
        if( self.path=='/favicon.ico' ):
            return
        
        dicParam=commfunc.GetHttpParam( self.path )
        GetLogger().info( 'http recv client addr:%s path:%s '%( self.client_address, self.path ) )
        
        if dicParam=='':
            self.wfile.write( ("{'status':'error_11'}").replace( '\'' , '"') )
            return
        
        try:
            #进入处理流程
            strRet=exchange.Process( dicParam )
        except:
            GetLogger().error( 'http recv client traceback:%s'%( traceback.format_exc() ) )
        
        #json数据字符串是用双引号"而不是单引号'
        strRet=strRet.replace( '\'' , '"')
        GetLogger().info( '返回:%s'%( strRet ) )
        
        self.wfile.write( strRet )
        
        
class ThreadingHTTPServer( ThreadingMixIn, HTTPServer ):
    pass

#处理http请求的服务器
__serverAddr=('',__nPort)
_srv=ThreadingHTTPServer( __serverAddr, RequestHandler )


#http处理服务器
class ServerListenThread(threading.Thread):
    def __init__(self ):
        global _srv
        threading.Thread.__init__(self)
        
    def run(self):
        global _srv
        try:
            _srv.serve_forever()
        except:
            GetLogger().critical( traceback.format_exc() )
            raise


#执行http监听的进程
__ThreadRun=ServerListenThread()


#开始http处理
def Start():
    global __ThreadRun
    __ThreadRun.start()
    
    if( __ThreadRun.isAlive() ):
        print 'begin...'
        return True
    else:
        print 'begin http server error!'
        return False


#结束http处理
def Stop():
    global _srv
    _srv.shutdown()
    
    
if __name__=='__main__':
    _srv.serve_forever()