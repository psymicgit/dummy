#-*- coding: utf-8 -*-
'''
Created on 2013-5-19

@author: zhouliang
'''

import threading,thread
import datetime
import time

#日志文件名
__strFileName='logfile'

#日志文件的日期
__datetimeFile=datetime.datetime.now()

#日志的文件
__file=None

#是否调试
__bDebug=True

#是否输出到屏幕
__bPrintConsole=True

#是否初始化成功
__bInitSuccess=False

#获得当前时间对应的文件
def __GetCurTimeFile():
    global __fileDir, __strFileName
    strFileDate=__datetimeFile.strftime('%Y_%m_%d_%H' )
    strFileFullName='%s/%s_%s.log'%( __fileDir, __strFileName, strFileDate )
    return open( strFileFullName, 'a' )
    

#初始化,返回成功
def Init( strFileDir, strFileName, bPrint, bDebug ):

    global __strFileName,__datetimeFile, __file, __fileDir, __bDebug, __bPrintConsole, __bInitSuccess
    
    if __bInitSuccess:
        return __bInitSuccess
    
    
    __bPrintConsole=bPrint
    __bDebug=bDebug
    __strFileName=strFileName
    __datetimeFile=datetime.datetime.now()
    __fileDir=strFileDir
    __file=__GetCurTimeFile()
    
    __bInitSuccess=(__file != None)
    
    return __bInitSuccess


def __Write( strHead, strData ):
    global __datetimeFile, __file, __strFileName, __bPrintConsole, __bInitSuccess
    
    if not __bInitSuccess:
        print 'warning: '
        return

    #每个小时一个文本
    now=datetime.datetime.now()
    if __datetimeFile.date() != now.date() or __datetimeFile.hour!=now.hour:
        __datetimeFile=__datetimeFile.now()
        
        __file=__GetCurTimeFile()

    logData='%s\t%d\t%s:\t%s'%( __datetimeFile.now(), thread.get_ident(), strHead, strData )
    
    if __bPrintConsole:
        print( logData )
        
    if not __file:
        print 'warning: logfile is none'
        return
    
    __file.write( logData+'\n' )
    
    
    

#输出信息
    
def debug( strData ):
    global __bDebug
    if not __bDebug:
        return
    __Write( 'debug', strData )

def info( strData ):
    __Write( 'info', strData )

def warning( strData ):
    __Write( 'warn', strData )

def error( strData ):
    __Write( 'error', strData )

def critical( strData ):
    __Write( 'critical', strData )

if __name__=='__main__':
    Init('c:/','test', True, True)
    while(True):
        debug('nihao')
        info('nihao2')
        warning('nihao3')
        error('nihao4')
        critical('nihao5')
        time.sleep(1)
    
    
    