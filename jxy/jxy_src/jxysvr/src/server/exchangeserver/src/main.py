#!/usr/bin/python
#-*- coding: utf-8 -*-
'''
Created on 2013-1-18

@author: zhouliang
'''

import globalsetting.globaldefine,globalsetting.config
from common import *
import common
from common.log import GetLogger
import traceback
import os
import ConfigParser
import time

#日志初始化
def InitLog():
    cfg=globalsetting.config.GetMainConfig()
    bDebug=cfg.getboolean( 'debug' , 'IsDebug')
    bPrint=cfg.getboolean( 'debug' , 'IsPrint' )
    logPath = os.getcwd() + "/log";
    if not os.path.exists(logPath):
        os.mkdir(logPath)
    log.InitLog(logPath, "exchange", bPrint, bDebug)

InitLog()


import dblogic.logicprocess
import process.httpcoreprocess
import process.process2gs




#每帧运行
__bStop=False
def Run():
    while not __bStop:
        oldClock=time.clock()
        process.process2gs.Process()
        
        process.process2gs.CheckAndSendAllOrderData()
        process.process2gs.CheckAndSendHeartMsg()
        if( time.clock()-oldClock )<globalsetting.globaldefine.DEF_SELECT_INTERVAL_TIME*1000:
            time.sleep(0.1)
    else:
        UnInitAll()

#全局初始化
def InitAll():
    print 'program begin init'
    
    GetLogger().info( 'program begin init' )
    
    if process.orderdata.Init():
        GetLogger().info( 'process.orderdata初始化成功！' )
    else:
        GetLogger().error( 'process.orderdata初始化失败！' )
        raise 'feedback db init failed'
    
    print 'init success'

#全局逆初始化
def UnInitAll():
    print 'program begin exit'
    GetLogger().info( '全局开始逆初始化' )
    GetLogger().info( '逆初始化步骤0' )
    process.orderdata.UnInit()
    GetLogger().info( '逆初始化步骤1' )
    process.httpcoreprocess.Stop()
    GetLogger().info( '全局逆初始化完毕' )

import thread

def Input():
    global __bStop
    while True:
        strInput=raw_input( "Input 'q' to quit\n" )
        print strInput
        if strInput=='q':
            __bStop=True
            break
        
        
        
#主函数
def Main():
    try:
        InitAll()
        if process.httpcoreprocess.Start():
            thread.start_new( Input, () )
            Run()
        else:
            GetLogger().critical( 'begin httpserver error!' )
            
            
            
    except: 
        GetLogger().info( 'exception:'+traceback.format_exc() )
        print traceback.format_exc()

if __name__=='__main__':
    Main()