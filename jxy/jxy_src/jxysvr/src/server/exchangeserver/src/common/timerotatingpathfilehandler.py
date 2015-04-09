#!/usr/bin/python
# -*- coding: utf-8 -*-

#根据时间切换目录和文件的文件处理器

from logging.handlers import *

class timerotatingpathfilehandler(TimedRotatingFileHandler):
    def __init__(self, root, filename, when = 'H', interval = 1, backupCount = 0,
                 encoding = None, delay = False, utc = False):
        #创建输出目录数
        t = int(time.time())
        if utc:
            timeTup = time.gmtime(t)
        else:
            timeTup = time.localtime(t)
        year = timeTup[0]
        month = timeTup[1]
        day = timeTup[2]
        self.root = root
        self.filename = filename
        self.baseFilename = self.getBaseFileName(self.root, year, month, day, self.filename)
        filepath = os.path.dirname(self.baseFilename)
        self.ensurePath(filepath)
        TimedRotatingFileHandler.__init__(self, self.baseFilename, when, interval, 
                                          backupCount, encoding, delay, utc)
    
    def getBaseFileName(self, root, year, month, day, filename):
        filenameWithoutExt = filename.split('.')[0]
        return os.path.join(root, '%04d-%02d/%s/%02d/%s'%(year, month, filenameWithoutExt, day, filename))
    
    def ensurePath(self, filepath):
        if not os.path.exists(filepath):
            os.makedirs(filepath)
    
    def doRollover(self):
        lastRolloverTime = self.rolloverAt
        if self.stream:
            self.stream.close()
            self.stream = None
        
        #获取上一次切换时间,认为不需要加上时间差
        #t = self.rolloverAt+self.interval
        if self.utc:
            timeTuple = time.gmtime()
        else:
            timeTuple = time.localtime()

        filenameTuple = self.filename.split('.')
        filename = ''
        if len(filenameTuple) == 1:
            filename = '%s-%s'%(filenameTuple[0], time.strftime(self.suffix, timeTuple))
        else:
            filename = '%s-%s.%s'%(filenameTuple[0], time.strftime(self.suffix, timeTuple), filenameTuple[len(filenameTuple) - 1])
        dfn = os.path.join(self.root, '%04d-%02d/%s/%02d/%s'%(timeTuple[0], timeTuple[1], filenameTuple[0], timeTuple[2], filename))
        dfnDirName=os.path.join(self.root, '%04d-%02d/%s/%02d/'%(timeTuple[0], timeTuple[1], filenameTuple[0], timeTuple[2]))

        self.ensurePath(dfnDirName)
        
        if os.path.exists(dfn):
            os.remove(dfn)
        
        
#        #可能由于改系统时间或其他人为原因造成文件找不到
#        if os.path.exists(self.baseFilename):
#            os.rename(self.baseFilename, dfn)
        #当前文件名字改变
        self.baseFilename=dfn
        
        if self.backupCount > 0:
            for s in self.getFilesToDelete():
                os.remove(s)
                
        currentTime = int(time.time())
        newRolloverAt = self.computeRollover(currentTime)
        while newRolloverAt <= currentTime:
            newRolloverAt = newRolloverAt + self.interval
        #If DST changes and midnight or weekly rollover, adjust for this.
        if (self.when == 'MIDNIGHT' or self.when.startswith('W')) and not self.utc:
            dstNow = time.localtime(currentTime)[-1]
            dstAtRollover = time.localtime(newRolloverAt)[-1]
            if dstNow != dstAtRollover:
                if not dstNow: # DST kicks in before next rollover, so we need to deduct an hour
                    newRolloverAt = newRolloverAt - 3600
                else:          # DST bows out before next rollover, so we need to add an hour
                    newRolloverAt = newRolloverAt + 3600
        self.rolloverAt = newRolloverAt
        #判断是否需要切换文件夹
        nextRolloverTime = self.rolloverAt
        if self.utc:
            lastRolloverTimeTup = time.gmtime(lastRolloverTime)
            nextRolloverTimeTup = time.gmtime(nextRolloverTime)
        else:
            lastRolloverTimeTup = time.localtime(lastRolloverTime)
            nextRolloverTimeTup = time.localtime(nextRolloverTime)
        lastRolloverYear = lastRolloverTimeTup[0]
        lastRolloverMonth = lastRolloverTimeTup[1]
        lastRolloverDay = lastRolloverTimeTup[2]
        nextRolloverYear = nextRolloverTimeTup[0]
        nextRolloverMonth = nextRolloverTimeTup[1]
        nextRolloverDay = nextRolloverTimeTup[2]
        if lastRolloverYear != nextRolloverYear or lastRolloverMonth != nextRolloverMonth or lastRolloverDay != lastRolloverDay:
            #年或月改变，更换文件夹
            self.baseFilename = self.getBaseFileName(self.root, nextRolloverYear, nextRolloverMonth, nextRolloverDay, self.filename)
            filepath = os.path.dirname(self.baseFilename)
            self.ensurePath(filepath)
        else:
            pass
        
        self.mode = 'w'
        self.stream = self._open()
        
def test():
    mylogger = logging.getLogger('test')
    mylogger.setLevel(logging.DEBUG)
    hdlr = timerotatingpathfilehandler('d:\\ServerLog\\', 'PyMongoDBServer.txt')
    fs = '%(asctime)s\t%(levelname)-8s\t%(message)s'
    dfs = '%Y-%m-%dT%H:%M:%S'
    fmt = logging.Formatter(fs, dfs)
    hdlr.setLevel(logging.DEBUG)
    hdlr.setFormatter(fmt)
    mylogger.addHandler(hdlr)
    
    lastTime = time.time()
    while True:
        curTime = time.time()
        if curTime - lastTime >= 5:
            lastTime = curTime
            mylogger.info('test')

if __name__ == '__main__':
    test()