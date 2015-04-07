ALL_DIR=server gameserver gateserver robot

all:
	@for i in $(ALL_DIR) ; \
	do \
		echo ; echo ; echo -e "\e[34;1m ----------------------------------`pwd`/$$i "make" $$i----------------------------------\033[0m"; \
		cd $$i; make; make install; cd ..; \
	done; echo ; echo ; 

clean:
	@for i in $(ALL_DIR) ; \
	do \
		echo ; echo ; echo -e "\e[34;1m ----------------------------------`pwd`/$$i "make" clean $$i----------------------------------\033[0m"; \
		cd $$i; make clean ; cd .. ; \
	done; echo ; echo ; 