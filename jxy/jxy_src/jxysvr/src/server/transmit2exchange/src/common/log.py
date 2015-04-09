#-*- coding: utf-8 -*-
'''
Created on 2013-1-18

@author: zhouliang
'''

import logging
import logging.handlers

import logging
#from logging import handlers
import timerotatingpathfilehandler
import os

def CreateLogger(LogPath, LogName, printToConsole = True, isDebug = True, mode = 'H'):
    logLevel = logging.INFO
    if isDebug:
        logLevel = logging.DEBUG
        
    logger = logging.getLogger(LogName)
    logger.setLevel(logLevel)
    
    hdlr = timerotatingpathfilehandler.timerotatingpathfilehandler(LogPath, LogName, mode)
    fs = '%(asctime)s\t%(levelname)-8s\t%(thread)d\t%(message)s'
    dfs = '%Y-%m-%dT%H:%M:%S'
    fmt = logging.Formatter(fs, dfs)
    hdlr.setLevel(logLevel)
    hdlr.setFormatter(fmt)
    logger.addHandler(hdlr)
    
    if printToConsole:
        console = logging.StreamHandler()
        console.setFormatter(fmt)
        logger.addHandler(console)
        console.setLevel(logLevel)
    return logger


__Logger = None

def InitLog(logPath, logName, isPrintToConsole = True, isDebug = True, mode = 'D'):
    global __Logger
    if __Logger == None:
        __Logger = CreateLogger(logPath, logName, isPrintToConsole, isDebug, mode)
    return __Logger

def GetLogger():
    global __Logger
    #获取日志对象前未创建，则创建默认
#    if __Logger == None:
#        logPath = os.getcwd() + "\\log"
#        if not os.path.exists(logPath):
#            os.mkdir(logPath)
#        InitLog(logPath, "Log")
    return __Logger

#-------------------------------------------------------------------
def test():
    logPath = os.getcwd() + "\\log";
    if not os.path.exists(logPath):
        os.mkdir(logPath)
    log1 = InitLog(logPath, "log1")
    log2 = InitLog(logPath, "log2")
    log1.info("log1 log")
    log2.info("log2 log")
    
if __name__ == '__main__':
    test()