#-*- coding: utf-8 -*-
'''
Created on 2013-1-18

@author: zhouliang
'''

import struct
import string
import math
import datetime
import os
import smtplib
import email
from email import utils
from email import header
from email.mime import text
from email.mime import multipart
import hashlib
import ConfigParser
import traceback

#获得http请求的参数以字典形式返回，正常的链接地址是：/?asd=1&asd2=2&asd3=3
def GetHttpParam(strPath):
    splitResult=strPath.split('?')
    
    #正常的路径中只有一个?
    if len(splitResult)!=2:
        return ''
    
    paramList=splitResult[1].split('&')
    
    if len( paramList )==0:
        return ''
    
    paramDic={}
    
    #填充详细的参数信息
    for strParamDes in paramList:
        paramInfo=strParamDes.split('=')
        
        if len(paramInfo)!=2:
            continue
        
        paramDic[ paramInfo[0] ]=paramInfo[1]
        
    return paramDic


def ReadBYTE(buf, pos):
    curValue = struct.unpack_from('B', buf, pos)
    pos += 1
    return curValue[0], pos

def ReadWORD(buf, pos):
    curValue = struct.unpack_from('!H', buf, pos)
    pos += 2  
    return curValue[0], pos

def ReadDWORD(buf, pos):
    curValue = struct.unpack_from('!I', buf, pos)  
    pos += 4
    return curValue[0], pos

def ReadFloat(buf, pos):
    curValue = struct.unpack_from('!f', buf, pos)  
    pos += 4
    return curValue[0], pos

def ReadDouble(buf, pos):
    curValue = struct.unpack_from('!d', buf, pos)  
    pos += 8
    return curValue[0], pos

def ReadString(buf, pos, _len):
    curValue = struct.unpack_from('!%ds'%_len, buf, pos)
    pos += _len  
    return curValue[0], pos


#----------------------写入
def WriteBYTE(buf, value):
    buf += struct.pack('B', value)
    return buf

def WriteWORD(buf, value):
    buf += struct.pack('!H', value)
    return buf

def WriteDWORD(buf, value):
    buf += struct.pack('!I', value)
    return buf

def WriteFloat(buf, value):
    buf += struct.pack('!f', value)
    return buf

def WriteDouble(buf, value):
    buf += struct.pack('!d', value)
    return buf

def WriteString(buf, len, value):
    buf += struct.pack('!%ds'%len, value)
    return buf

def GetDistance(srcX, srcY, destX, destY):
    return math.sqrt(pow(srcX - destX, 2) + pow(srcY - destY, 2))
    
def MovePos(srcX, srcY, destX, destY, curMoveDist):
    if curMoveDist == 0:
        return  srcX, srcY
    
    totalDist = GetDistance(srcX, srcY, destX, destY)
    if totalDist == 0:
        return  srcX, srcY
    
    resultX = curMoveDist / float(totalDist) * (destX - srcX) + srcX
    resultY = curMoveDist / float(totalDist) * (destY - srcY) + srcY
    return resultX, resultY 
    

##测试代码:
#strs = '美香是猪'
#buf = ''
#buf = WriteString(buf, len(strs), strs)
#value, pos = ReadString(buf, 0, len(strs))
#print value

#获得当前系统时间
def GetCurrentDataTimeStr():
    curTime = datetime.datetime.today()
    curTimeStr = str(curTime)
    curTimeStr = curTimeStr.split(".")[0]
    return curTimeStr

#获得系统时间(参数 -> 时间列表)
def GetDateTimeByStr(timeStr):
    timeStr = timeStr.split(".")[0]
    return  datetime.datetime.strptime(timeStr, "%Y-%m-%d %H:%M:%S")
    
    
    
#字符串转换为整型, 如果不能转换, 返回默认值
def ToIntDef(input, defValue = 0):
    try:
        result = int(input)
        return result
    except ValueError:
        return defValue
    
    
#字符串转换为整型, 如果不能转换, 返回False,原值
def StrToInt(input):
    try:
        result = int(input)
        return True,result
    except ValueError:
        return False,input
    
    
#16进制颜色转换
#"#FFFFFF"--"255,255,255"
def HcToSc(h):
    h="0x"+h[1:7]
    red=string.atoi(h[:2]+h[2:4], base=16)
    green=string.atoi(h[:2]+h[4:6], base=16)
    blue=string.atoi(h[:2]+h[6:8], base=16)
    cStr=str(red)+","+str(green)+","+str(blue)
    return cStr

#"255,255,255"--"#FFFFFF"
def ScToHc(s):
    red=hex(string.atoi(s.split(",")[0]))[2:]
    green=hex(string.atoi(s.split(",")[1]))[2:]
    blue=hex(string.atoi(s.split(",")[2]))[2:]
    hStr="#"+str(red+green+blue)
    return hStr

#16进制转换
#"0xFFFFFF"--"255,255,255"
def HdToSd(h):
    red=string.atoi(h[0:2]+h[2:4], base=16)
    green=string.atoi(h[0:2]+h[4:6], base=16)
    blue=string.atoi(h[0:2]+h[6:8], base=16)
    cStr=str(red)+","+str(green)+","+str(blue)
    return cStr

#"255,255,255"--"0xFFFFFF"
def SdToHd(s):
    red=hex(string.atoi(s.split(",")[0]))[2:]
    green=hex(string.atoi(s.split(",")[1]))[2:]
    blue=hex(string.atoi(s.split(",")[2]))[2:]
    hStr="0x"+str(red+green+blue)
    return hStr

def GetPercent(value1, value2):
    if value2 == 0:
        return 0
    return int(float(value1) / float(value2) * 100)



def OpenFileForWrite(fileName):
    dirName = os.path.dirname(fileName)
    if not os.path.isdir(dirName):
        os.makedirs(dirName)
    
    if not os.path.isfile(fileName):
        return file(fileName, 'w')
    return  file(fileName, 'a')


#函数调用
def ParseNameGetObj(curCallObj, callName):
    callList = callName.split(".")
    if len(callList) <= 1:
        return None
    
    for curCallName in callList:
        if hasattr(curCallObj, curCallName) != True:
            #无此属性
            return None
        curCallObj = getattr(curCallObj, curCallName)
    return curCallObj

#获得执行函数
def GetExecFunc(curCallObj, callName):
    curCallObj = ParseNameGetObj(curCallObj, callName)
    if curCallObj == None:
        return None
        
    if callable(curCallObj) != True:
        #不可调用
        return None
    
    return curCallObj

#字符串异或处理
def str_xor(astring, xornum):
    a=[]
#    b=[]
    for x in astring:
#        b.append(chr(ord(x)^0x96))
        a.append(chr(ord(x)^xornum))
    return ''.join(a)


#读取配置文件
def LoadDataFromCfg(cfgFilePath, title, sign):
    cfg = ConfigParser.ConfigParser()
    cfg.read(cfgFilePath)
    return cfg.get(title, sign)

    
#发送邮件，包含附件
def SendEmail(user, psw, subject, AnnexList, toaddr, content, emailsrv):
#    print subject
    try:
        #创建一个带附件的实例
        msg = multipart.MIMEMultipart()
        
        #构造附件        
        for Annex in AnnexList:
            annexfile = open(Annex, 'rb')
            annexdata = annexfile.read()
            annexfile.close()
            att = text.MIMEText(annexdata, 'base64', 'gb2312')
            att['Content-Type'] = 'application/octet-stream'
            att["Content-Disposition"] = 'attachment; filename="%s"'%(os.path.basename(Annex))
            msg.attach(att)  
        
        #构造正文
        att = text.MIMEText(content, 'base64', 'gb2312')
        att['Content-Type'] = 'text/plain'
        att["Content-Transfer-Encoding"] = '7bit'
        msg.attach(att)  
            
        #加邮件头
        msg['to'] = toaddr
        msg['from'] = user
        msg['subject'] = header.Header(subject, 'gb2312')
        
        s = smtplib.SMTP(emailsrv)
        s.login(user, psw)
        s.sendmail(msg['from'], toaddr, msg.as_string())
        s.quit()
    except:
        return False, "%s"%traceback.format_exc()
    return True, "" 

#写文件
def SaveToFile(info, filepath):
    file = OpenFileForWrite(filepath)
    file.write(info)
    file.close()
    
    
def ReadFromFile(filepath):
    file = open(filepath, 'r')
    content = file.read()
    file.close()
    return content

def utf8togbk(data):
    result = ""
    try:
        result = data.encode("gbk")
    except:
        result = data
    return result   
    
#保存附件
def SaveAnnexs(dataPath, annexlist = []): 
    filelist = []
    try:  
        if not os.path.isdir(dataPath):
            os.makedirs(dataPath)
        
        for annexinfo in annexlist:
            filename = annexinfo[0]
            filename = utf8togbk(filename)
            filename = os.path.join(dataPath, filename)
            filedata = annexinfo[1]
            file = open(filename,'wb')
            file.write(filedata)
            file.close()
            filelist.append(filename)
    except Exception, e:
        return False, '%s'%e
    return True, filelist
  
    
def GetDataList(data, cte):
    datalist=[]
    alist=[]
    if cte in data:
        alist = data.split(cte)
    else:
        return data
    for i in range(1, len(alist)):
        datalist.append(alist[i].split("?=")[0])
    return datalist

## 全局函数简要说明：对文件名进行解码
#  @param data 要解码的数据
#  @param cte 编码格式
#  @return 解码后的数据
#  @remarks 对文件名进行解码
def DecodeData(data, cte):
    if data == None:
        return data
    if "?gb2312?b?" in data:
        records = GetDataList(data, "?gb2312?b?")
        ret = ""
        for record in records:
            ret += utils._bdecode(record)
        return ret
    if "?gb2312?B?" in data:
        records = GetDataList(data, "?gb2312?B?")
        ret = ""
        for record in records:
            ret += utils._bdecode(record)
        return ret
    else:
        return data
    
    
#    if data == None:
#        return data
#    if "?gb2312?b?" in data:
#        data = data.split("?gb2312?b?")[1][:-2]
#        return utils._bdecode(data)
#    if "?gb2312?B?" in data:
#        data = data.split("?gb2312?B?")[1][:-2]
#        return utils._bdecode(data)
#    else:
#        return data     
#    return header.decode_header(data)[0][0]

def ParseEmail(rawmsg):
    annexlist = []
    content = ""
    try:
        msg = email.message_from_string("\n".join(rawmsg))
        
        #获取正文信息，和所有的附件信息
        for part in msg.walk():
            
            annexinfolist = []
            contenttype = part.get_content_type()  #文件内容类型
            attachment = part.get("Content-Disposition", "").lower() #判断是否是附件类型
            filename = part.get_filename()         #文件名
            cte = part.get('content-transfer-encoding', '').lower()
            
            #获取附件信息
            if ("attachment" in attachment) and filename:
                filename = DecodeData(filename, cte)
                annexinfolist.append(filename)
                annexinfolist.append(part.get_payload(None, True))
                annexlist.append(annexinfolist)
            #获取正文信息
            elif contenttype in ['text/plain','text/base64']:
                content = part.get_payload(None, True)
    except:
        return False, [], traceback.format_exc()
    return True, annexlist, content

#解压指定文件夹下的zip文件，返回解压过的zip文件列表
def ExtractAllZipFile(path):
    flist = os.listdir(path)
    zlist = []
    for file in flist:
        alist = os.path.splitext(file)
        suffix = alist[1].lower()
#        name = alist[0]
        if '.zip' == suffix:
            zfile = os.path.join(path, file)
            zlist.append(zfile)
#            extractto = os.path.join(path, name)
#            ZipFileOper.extract(zfile, extractto)
            ZipFileOper.extract(zfile, path)
    return zlist

def DelDir(dir):
    for root, dirs, files in os.walk(dir, topdown=False):
        for name in files:
            os.remove(os.path.join(root, name))
        for name in dirs:
            os.rmdir(os.path.join(root, name))
    if os.path.isdir(dir):
        os.rmdir(dir)
        
def DelFile(dir, txtSign=['.*']):
    for root, dirs, files in os.walk(dir, topdown=False):
        for name in files:
            if '.*' in txtSign or os.path.splitext(name)[1] in txtSign:
                os.remove(os.path.join(root, name))

def CopyDir(srcDir, targetDir):
    if not os.path.exists(targetDir):
        os.makedirs(targetDir)
    for root, dirs, files in os.walk(srcDir, topdown=False):
        dirname = ""
        copyto = targetDir
        if root != srcDir:
            dirname = root.split("%s\\"%srcDir)[1]
            copyto = os.path.join(copyto, dirname)
        if not os.path.exists(copyto):
            os.makedirs(copyto)
        for name in files:
            srcFile = os.path.join(root, name)
            targetFile = os.path.join(copyto, name)
            file(targetFile, "wb").write(open(srcFile,"rb").read())
            
def CopyFile(srcFile, targetDir):
    if not os.path.exists(targetDir):
        os.makedirs(targetDir)
    filename = os.path.split(srcFile)[1]
    targetFile = os.path.join(targetDir, filename)
    file(targetFile, "wb").write(open(srcFile,"rb").read())
    
def BinToStr( strInput ):
    strRet=''
    for a in strInput:
        strRet+=hex( ord( a ) )+' '
    
    return strRet

#传入字符串，对其进行md5
def GetMD5( strInput ):
    m = hashlib.md5()
    m.update( strInput )
    
    return m.hexdigest()

#局部锁
class ScopeLock:
    def __init__(self, lock):
        self.__lock=lock
        self.__lock.acquire()
        
    def __del__(self):
        self.__lock.release()

if __name__=='__main__':
    print GetHttpParam('/?xx=1&xx2=2&xx3=3&xx4=5')