#-*- coding: utf-8 -*-
'''
Created on 2013-1-20

@author: zhouliang
'''
import config
import globaldefine

cfg=config.GetMainConfig()
strServerName=cfg.get( 'platform' , 'ServerName')

#充值数据库的表名
DB_EXCHANGE_TABLE_NAME=globaldefine.DEF_EXCHANGE_TABLE_PREFIX + strServerName

#充值反馈数据库的内容
DB_EXCHANGE_FEEDBACK_TABLE_NAME=globaldefine.DEF_EXCHANGE_FEEDBACK_TABLE_PREFIX + strServerName

#全区的充值表
DB_GLOBAL_AMOUNT_TABLE_NAME='global_amount'

#加密用的key
ENCRYP_KEY=cfg.get( 'platform' , 'Key')

#兑换函数
EXCHANGE_FUNC_NAME=cfg.get( 'platform', 'ExchangeFuncName' )