#-*- coding: utf-8 -*-
'''
Created on 2013-1-20

@author: zhouliang
'''


from common import *

from globalsetting import *
import globalsetting
from common.log import *
import dblogic
import process.orderdata
import string


#检查sign
def CheckSign( dicInput ):
    
    
    listInfo=['source=']
    listInfo.append( dicInput['source'] )
    listInfo.append( '&trade_no=' )
    listInfo.append( dicInput['trade_no'] )
    listInfo.append( '&amount=' )
    listInfo.append( dicInput['amount'] )
    listInfo.append( '&partner=' )
    listInfo.append( dicInput['partner'] )
    listInfo.append( '&key=' )
    listInfo.append( globalsetting.globalinfo.ENCRYP_KEY )
    
    strMD5Input=''.join( listInfo )
    GetLogger().debug( '用于MD5的字符串是:%s'%strMD5Input )
    strMd5=commfunc.GetMD5( strMD5Input )
    
    if strMd5==dicInput['sign']:
        return True
    else:
        GetLogger().error( '校验md5错误，传入的md5是%s,自己算出的md5是%s'%( dicInput['sign'], strMd5 ) )
        return False
    
#处理流程
def Process( dicInput ):
    
    
    GetLogger().info( '传来的参数是:%s'%( str( dicInput ) ) )
    
    if dicInput['trade_no']=='':
        return "{'status':'empty trade_no'}"
    
    if not CheckSign( dicInput ):
        return "{'status':'signerror'}"
    
    
    dbObj=dblogic.logicprocess.GetDBObj()
    
    dicInput['trade_no']=string.replace( dicInput['trade_no'], '\'', '' )
    strSql="select * from %s where OrderID='%s' and DateExchange IS NOT NULL"%( globalinfo.DB_EXCHANGE_TABLE_NAME, dicInput['trade_no'] )
    retList=dbObj.RunSql(strSql)
    if retList:
        GetLogger().info( '重复提交充值' )
        return "{'status':'success'}"
    
    if not dblogic.logicprocess.SaveExchangeInfo( 
            dicInput['trade_no'] , dicInput['source'], dicInput['amount'], dicInput['partner'] ):
        return "{'status':'error_1'}"
    
    
    #添加已经充值的账号记录
    
    retSelectData=dblogic.logicprocess.GetDataByOrderID( dicInput['trade_no'] )
    
    
    GetLogger().debug( '获得充值信息:%s'%( str(retSelectData ) ) )
    
    if not retSelectData:
        return "{'status':'error_2'}"
    
    #根据数据库的列顺序
    strPlayerID=retSelectData[0]['PlayerID']
    
    
    
    if not process.orderdata.AddData( int( strPlayerID ) , dicInput['trade_no'], str(dicInput['amount']), dbObj ):
        return "{'status':'error_3'}"
    
    
    return "{'status':'success'}"
    
    