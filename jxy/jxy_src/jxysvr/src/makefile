DLLSRCDIR=dll/sdu dll/sdlogger dll/sdframework dll/sddb
SERSRCDIR1=server/gameserver server/gateserver server/loginserver server/zoneserver
SERSRCDIR2=server/gmserver server/monitorserver server/noticeserver 
ALLSRCDIR=$(DLLSRCDIR) $(SERSRCDIR1) $(SERSRCDIR2)

all:
	for i in $(ALLSRCDIR) ; \
	do \
		echo `pwd` "Make" $$i; \
		cd $$i; make install; cd ..; \
	done
allclean:
	for i in $(ALLSRCDIR) ; \
	do \
		echo `pwd` "Make clean" $$i ; \
		cd $$i; make clean ; cd .. ; \
	done
	
ser1:
	for i in $(SERSRCDIR1) ; \
	do \
		echo `pwd` "Make" $$i; \
		cd $$i; make install; cd ..; \
	done
ser1clean:
	for i in $(SERSRCDIR1) ; \
	do \
		echo `pwd` "Make clean" $$i ; \
		cd $$i; make clean ; cd .. ; \
	done
		
ser2:
	for i in $(SERSRCDIR2) ; \
	do \
		echo `pwd` "Make" $$i; \
		cd $$i; make install; cd ..; \
	done
ser2clean:
	for i in $(SERSRCDIR2) ; \
	do \
		echo `pwd` "Make clean" $$i ; \
		cd $$i; make clean ; cd .. ; \
	done
