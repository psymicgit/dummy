LIB_DIR=server
EXE_DIR=gameserver gateserver robot
ALL_DIR=$(LIB_DIR) $(EXE_DIR)

all:
	for i in $(ALL_DIR) ; \
	do \
		echo `pwd` "Make" $$i; \
		cd $$i; make install; cd ..; \
	done
clean:
	for i in $(ALL_DIR) ; \
	do \
		echo `pwd` "Make clean" $$i ; \
		cd $$i; make clean ; cd .. ; \
	done