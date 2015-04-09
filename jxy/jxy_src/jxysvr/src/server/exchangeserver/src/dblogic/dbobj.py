#-*- coding: utf-8 -*-
'''
Created on 2013-1-24

@author: zhouliang
'''

import threading
from globalsetting import *
import traceback
import common
from common.log import GetLogger
from common import commfunc
import string
import MySQLdb

class DBObj:
    def __init__(self):
        self.__dbLock=threading.Lock()
        self.__con=None
        self.__ConnectDB()
        
    #连接到数据库
    def __ConnectDB(self):
        try:
            cfg=config.GetMainConfig()
            strUser=cfg.get('db', 'User', True)
            strPsw=cfg.get( 'db', 'Psw', True )
            strHost=cfg.get( 'db', 'Host' , True)
            nPort=string.atoi( cfg.get( 'db', 'Port', True ), 10 )
            strDBName=cfg.get( 'db', 'Name', True )
            GetLogger().info( 'connect db Host:%s,port:%d'%( strHost, nPort ) )
            
            self.__con=MySQLdb.connect(host=strHost,user=strUser,passwd=strPsw,db=strDBName,charset="utf8", port=nPort)
            return True
        except:
            GetLogger().error('connect db failed, error:'+traceback.format_exc() )
            raise
    
#    def __CheckAndReconnect(self):
#        bNeedConnect=False
#        try:
#            if self.__con.ping(True)!=None:
#                GetLogger().error( 'ping数据库失败，无法操作' )
#                return False
#        except:
#            bNeedConnect=True
#            GetLogger().error( '数据库连接检测不通过，需要重连。%s'%traceback.format_exc() )
#        try:
#            if bNeedConnect:
#                self.__ConnectDB()
#        except:
#            GetLogger().error( '重连数据库失败:%s'%traceback.format_exc() )

#只返回数量
    def ExecSql( self, strSql ):
        common.commfunc.ScopeLock( self.__dbLock )
        #self.__CheckAndReconnect()
        cursor=self.__con.cursor()
        
        try:
            cursor.execute( strSql )
            ret=cursor.rowcount
        except MySQLdb.OperationalError, error:
            GetLogger().error(  'exec sql:%s failed:%s'%( strSql, str(error) ) )
            self.__ConnectDB()
            cursor.execute( strSql )
        except:
            GetLogger().error(  'exec sql failed:'+strSql+traceback.format_exc() )
            cursor.close()
            raise
        cursor.close()
        
        self.__con.commit()
        return ret
    
    
    #查询语句,返回查询到的内容
    def RunSql( self, strSql ):
        commfunc.ScopeLock( self.__dbLock )
        #self.__CheckAndReconnect()
        cursor=self.__con.cursor((MySQLdb.cursors.DictCursor))
            
        try:
            cursor.execute( strSql )
        except MySQLdb.OperationalError, error:
            GetLogger().error(  'exec sql:%s failed:%s'%( strSql, str(error) ) )
            self.__ConnectDB()
            cursor.execute( strSql )
        except:
            GetLogger().error(  'exec sql failed:'+strSql+traceback.format_exc() )
            cursor.close()
            return False
        results = cursor.fetchall()
        cursor.close()
        self.__con.commit()
        return results
    
    def __del__(self):
        if( self.__con ):
            self.__con.close()