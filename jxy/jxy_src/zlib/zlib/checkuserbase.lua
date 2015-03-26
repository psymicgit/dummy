accesscodemanager = require "accesscodemanager"
rechargemanager = require"rechargemanager"
local  ngx_helpers = require "ngxs.helpers"
local mysqlmanager=require "mysqlmanager"
local logs=require "logs"
local utils=require "utils"
local zlib= require "zlib"
local cjson=require "cjson"
local capture=ngx_helpers.capture
local exec=mysqlmanager.exec
local query=mysqlmanager.query
local query_pay=mysqlmanager.query_pay
local len=string.len
module(...,package.seeall)

visitemethod={
get=1,
post=2,
post_header=3
}

function init(self)
	--注册验证用户方法
	accesscodemanager.registverifyuserFunc(self.platformid,function(...)
		return self:checkuser(...)
	end
	)
	
	--注册创建订单方法（安卓与ios）
	
	rechargemanager.registcreateordFunc(self.channelid_and,function(...)
		return  self:createorder_android(...)
	end
	)
	
		rechargemanager.registcreateordFunc(self.channelid_ios,function(...)
		return  self:createorder_ios(...)
	end
	)
	
end

local appid="5847AF86BD806F0B355539E3812EE98C"

local function getMsgID()
	
	return os.time()..utils.serialnumber()
	
end

 _Status={
request="request",
success="success"
}

local function callback_wrong(senddata)
	
end

local function callback_success()
	
end

function posttalkingdata(Status,Os,playerid,orderid,rmb,gold,zoneid,level,gameVersion,currencyType,chargeTime,iapID,paymentType,mission)
	
	local url="http://api.talkinggame.com/api/charge/"..appid
	
	if not currencyType then
		currencyType="CNY"
	end
	local _data={}
	local data={
		msgID=getMsgID(),
		status=_Status[Status],
		OS=Os,
		accountID=playerid,
		orderID=orderid,
		currencyAmount=rmb,
		currencyType=currencyType,
		virtualCurrencyAmount=gold,
		chargeTime=chargeTime,
		iapID=iapID,
		paymentType=paymentType,
		gameServer=tostring(zoneid),
		gameVersion=tostring(gameVersion),
		level=level,
		mission=mission
	}
	table.insert(_data,data)
	local data_txt=cjson.encode(_data)
	--data_txt=utils.To_utf8(data_txt)
	logs.logTalkingData_s(data_txt)
	local data_gziptxt=zlib.gzcompress(data_txt,len(data_txt))
	logs.logTalkingData_s(data_gziptxt)
	local res=capture.Post(url,data_gziptxt)
	
	if res and res.body then
		local body=res.body
	
		local respone_body,length,errmsg=zlib.gzdecompress(body,len(body))
		if length>0 then
			
			if Status==_Status.success then
				logs.logTalkingData(body)
				respone_body=utils.To_utf8(respone_body)
				logs.logappstore_recharge(_Status[Status].."："..respone_body)
			end
			local callback_data=cjson.decode(respone_body)
			
			if callback_data.code==100 then
				callback_success()
			end
		else
			if Status==_Status.success then
				logs.logTalkingData_err(body)	
			end
			if not errmsg then
				errmsg=""
			end
	
			logs.logappstore_recharge("源："..data_txt.."经过gzip加密后："..data_gziptxt.._Status[Status].."："..body.."错误："..errmsg)
		end
	end
	
	callback_wrong(data_txt)
	
end

-- 记录每个充值回调的数据，参数：写日志的方法，请求方式，内容
function logRequestData(logFunc,v_method,data)
	
	if logFunc and type(logFunc)=="function" and data and v_method then
		local r_type="get"
		
		if v_method==visitemethod.post then
			r_type="post"
		end
		
		logFunc(r_type.."："..data)
	end
	
end
--[[第三方账号认证（走http协议的）基类

	必须实现方法 getcheckuserurl(uid,sessionid,sdkname,usertype) 返回的参数为 url（第三方验证地址）,visitemethod（请求方式）,postdata（若为post方式则必须有值）
	与方法processingresult(res,uid,usertype),返回 bool,errmsg(可选)

]]



function checkuser(self,uid,sessionid,sdkname,usertype,access_token_secret)
	local url,method,postdata=self.getcheckuserurl(uid,sessionid,sdkname,usertype)
	
	if not url then
		return false
	end
	local before=ngx.now()
	local res=nil 
		if method==visitemethod.get then
		res=capture.Get(url)
	elseif  method==visitemethod.post then
		res=capture.Post(url,postdata)
	else
		res=capture.Post_header(url,postdata)
	end
	local _end=ngx.now()
	logs.logcheckuser(string.format("验证用户所花费的时间为%d",_end-before))
	
	if not res then
		return false,"网络连接出现错误"
	end
	
	return self.processingresult(res,uid,usertype)
end

--posttalkingdata(Status,Os,playerid,orderid,rmb,gold,zoneid,level,gameVersion,currencyType,chargeTime,iapID,paymentType,mission)

function createorder_ios(self, db,orderid,zoneid,playerid,productid,rmb,gold,istest,quantity,deviceid,clientip,accesstoken)
		local sql="select orderid from {1} where orderid='{2}';"

		local res=query(db,sql,nil,self.orders_tablename_ios,orderid)
		
		if not res or #res>0 then -- 已存在该订单直接返回失败
			return false
		end

		sql="INSERT INTO {12}(orderid,zoneid,playerid,productid,rmb,gold,type,quantity,createtime,deviceid,createip)VALUES('{1}',{2},{3},'{4}',{5},{6},{7},{8},'{9}','{10}','{11}');"
		
		res=exec(db,sql
				,orderid
				,zoneid
				,playerid
				,productid
				,rmb
				,gold
				,istest
				,quantity
				,mysqlmanager.GetCurrentDateTime()
				,deviceid
				,clientip
				,self.orders_tablename_ios
			)
		
		if res.state==1 then
			sql="select level,lastgameversion from globalplayers where playerid={1};"
			res=query(db,sql,nil,playerid)
			
			if not res or #res==0 then
				return false
			end
			
			posttalkingdata(_Status.request,"ios",playerid,orderid,rmb,gold,zoneid,res[1]["level"],tostring(res[1]["lastgameversion"]))
			return true
		end
		
		return false
end


--posttalkingdata(Status,Os,playerid,orderid,rmb,gold,zoneid,level,gameVersion,currencyType,chargeTime,iapID,paymentType,mission)
function createorder_android(self, db,orderid,zoneid,playerid,productid,rmb,gold,istest,quantity,deviceid,clientip,accesstoken)
		local sql="select orderid from {1} where orderid='{2}';"

		local res=query(db,sql,nil,self.orders_tablename_android,orderid)
		
		if not res or #res>0 then -- 已存在该订单直接返回失败
			return false
		end

		sql="INSERT INTO {12}(orderid,zoneid,playerid,productid,rmb,gold,type,quantity,createtime,deviceid,createip)VALUES('{1}',{2},{3},'{4}',{5},{6},{7},{8},'{9}','{10}','{11}');"
		
		res=exec(db,sql
				,orderid
				,zoneid
				,playerid
				,productid
				,rmb
				,gold
				,istest
				,quantity
				,mysqlmanager.GetCurrentDateTime()
				,deviceid
				,clientip
				,self.orders_tablename_android
			)
		
		if res.state==1 then
			sql="select level,lastgameversion from globalplayers where playerid={1};"
			res=query(db,sql,nil,playerid)
			
			if not res or #res==0 then
				return false
			end
			
			posttalkingdata(_Status.request,"android",playerid,orderid,rmb,gold,zoneid,res[1]["level"],tostring(res[1]["lastgameversion"]))
			return true
		end
		
		return false
end

function order_finished(orderid,playerid,zoneid,gold,rmb,type,createip,deviceid,channel,source)
	
	if not source then
		source=0
	end
	
	if not createip  then
		createip=""
	end
	
	if not deviceid  then
		deviceid=""
	end
	
	local sql=[[
	 call order_finished('{1}'
	,{2}
	,{3}
	,{4}
	,{5}
	,{6}
	,'{7}'
	,'{8}'
	,'{9}'
	,{10}
	,{11}
	,@payid
	);
	select @payid;
	]]
	local db=mysqlmanager.CreateDB()
	local res=query_pay(db,sql,2
	,orderid
	,playerid
	,zoneid
	,gold
	,rmb
	,type
	,createip
	,deviceid
	,utils.GetCurrentDateTime()
	,channel
	,source
	)

	local result=true
	
	if not res or #res==0 then
		mysqlmanager.close(db)
		 result=false
	end
	
	
	
	if result then
		sql="select level,lastgameversion from globalplayers where playerid={1};"
		res=query(db,sql,nil,playerid)
		
		if not res or #res==0 then
			return false
		end
		
		local platform="android" 
		
		if channel<=100 then
			platform="ios"
		end
		
		posttalkingdata(_Status.success,platform,playerid,orderid,rmb,gold,zoneid,res[1]["level"],tostring(res[1]["lastgameversion"]))
		mysqlmanager.close(db)
		
		return true,res[1]["@payid"]
	end
	
	return false
end