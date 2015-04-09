#-*- coding: utf-8 -*-
'''
Created on 2013-1-18

@author: zhouliang
'''

#配置文件目录
DEF_CONFIG_DIR='config/'

#主配文件路径
DEF_MAIN_CONFIG=DEF_CONFIG_DIR+'main.ini'

#兑换数据表的前缀
DEF_EXCHANGE_TABLE_PREFIX='exchange_'

#兑换记录的数据表
DEF_EXCHANGE_FEEDBACK_TABLE_PREFIX='exchange_feedback_'

#每一帧的时间间隔
DEF_SELECT_INTERVAL_TIME=0.1

#心跳间隔
DEF_HEART_INTERVAL=30

#处理间隔
DEF_SEND_ALL_INTERVAL=0.1


#主动发出的封包定义
RECGS_RECHARGE_REQ=90
RECGS_HEART_REQ=92
RECGS_GEN_GUID_ACK=95
RECGS_PROCESS_ORDER_ID_REQ=96



#处理充值结果
PROCESS_EXCHANGE_SUCCESS=0
PROCESS_EXCHANGE_ORDER_ID_REPEAT=1