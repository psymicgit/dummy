#!/usr/bin/python
# -*- coding: utf-8 -*-
'''
Created on 2013-1-20

@author: zhouliang
'''

import commfunc
from log import GetLogger
GetLog = GetLogger

class CPackParse():
    def __init__(self,client):
        self.__PackHeadLen = 2
        self.__PackSignLen = 2
        self.__PackBodyLen = 4
        self.__PackSign = 0xCCDD
        self.__BuffPack = ''
        self.__client=client
        
        #收到封包后的回调，必须要设置
        self.OnPackRecv = None
        
    def __GetPack(self, buf):
        buflen = len(buf)
        if buflen == 0:
            GetLog().debug("buflen == 0")
            return 0
        
        packLen = self.__PackSignLen+self.__PackBodyLen+self.__PackHeadLen
        if buflen < packLen:
            GetLog().debug("buflen < packLen")
            return 0
        pos = 0
        packSign, pos = commfunc.ReadWORD(buf, pos)
        if packSign != self.__PackSign:
            GetLog().warning('封包头错误，接收到的封包头=%x'%packSign)
            return -1
        
        head, pos = commfunc.ReadWORD(buf, pos)
        nBodyLen, pos = commfunc.ReadDWORD(buf, pos)
        if nBodyLen + pos > buflen:
            GetLog().debug("nBodyLen + pos > buflen")
            return 0
        
        curPack, pos = commfunc.ReadString(buf, pos, nBodyLen)
        
        if self.OnPackRecv:
            self.OnPackRecv(curPack, head, self.__client)
        else:
            GetLog().warning("self.__PackDispatcher == None")
        return pos
    
    #传入接收到的原始数据
    def OnRecvOrignPack(self, buf):
        GetLogger().debug( 'orignpack recv data:%s'%( commfunc.BinToStr(buf) ) ) 
        self.__BuffPack += buf
        while len(self.__BuffPack):
            pos = self.__GetPack(self.__BuffPack)
            #封包头错误
            if pos == -1:
                if len(self.__BuffPack) != 0:
                    self.__BuffPack = self.__BuffPack[1:]
                continue
            if pos == 0:
                GetLog().info("封包不完整，等待后续封包")
                break
            self.__BuffPack = self.__BuffPack[pos:]
            
    def Clear(self):
        self.__BuffPack = ''
    
    #打包数据
    def PackData(self, buf, head):
        buffer = ''
        buffer = commfunc.WriteWORD(buffer, self.__PackSign)
        buffer = commfunc.WriteWORD(buffer, head)
        buffer = commfunc.WriteDWORD(buffer, len(buf))
        buffer += buf
        return buffer