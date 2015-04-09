VERSION =	1.9.3
PREFIX =	/usr/devel
INCLUDE =	-I$(PREFIX)/include
INSTALLDIR =	$(PREFIX)/bin

CPPFLAGS = 	-Wall -O2 -g -MD -DSQL2CLASS_VERSION='"$(VERSION)"' $(INCLUDE)
LIBS =		-L$(PREFIX)/lib 

INSTALL =	/usr/bin/install --strip
TARDIR =	/usr/local/apache/www.alhem.net/htdocs/project/sql2class
TARFILE =	sql2class-$(VERSION).tar.gz

PROGS =		sql2class 

all:		$(PROGS)

t:		t.o 
		g++ -o $@ $^ $(LIBS) -lsql2class -lmysqlwrapped -L/usr/local/lib/mysql \
			-lmysqlclient -lz
		
clean:
		@touch x.o x~
		rm -f *.o *~ $(PROGS) *.d slask

install:	all
		$(INSTALL) $(PROGS) $(INSTALLDIR)

SQL2CLASS =	sql2class.o utils.o gclass.o gclasslib.o Parse.o gclassjava.o
sql2class:	$(SQL2CLASS) 
		g++ -o sql2class $(SQL2CLASS) $(LIBS)

tar:
		mkdir -p $(TARDIR)
		tar czf $(TARFILE) \
			sql2class.h sql2class.cpp \
			utils.h utils.cpp \
			gclass.h gclass.cpp \
			gclasslib.h gclasslib.cpp \
			Parse.h Parse.cpp \
			Makefile README str-sql2class \
			gclassjava.h gclassjava.cpp \
			gpl.txt t.cpp Project/sql2class.dsp
		/usr/local/bin/tarfix.sh sql2class-$(VERSION)
		cp $(TARFILE) $(TARDIR)
		cp sql2class-$(VERSION).zip $(TARDIR)

db:		sql2class
		mysqldump sql2class > str-sql2class
		./sql2class -build -getset -stl -lib libsql2class str-sql2class -overwrite -namespace db

-include	*.d
