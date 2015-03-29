LIB_DIR=server
EXE_DIR=gameserver gateserver robot
ALL_DIR=$(LIB_DIR) $(EXE_DIR)

ROBOT_DIR=server robot

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

rob:
	for i in $(ROBOT_DIR) ; \
	do \
		echo `pwd` "Make" $$i; \
		cd $$i; make install; cd ..; \
	done

