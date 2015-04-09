#-*- coding: utf-8 -*-
'''
Created on 2013-1-20

@author: zhouliang
'''


import ConfigParser
import globalsetting.globaldefine

#主要配置
__mainConfig=ConfigParser.ConfigParser()

__mainConfig.read( globalsetting.globaldefine.DEF_MAIN_CONFIG )

def GetMainConfig():
    return __mainConfig