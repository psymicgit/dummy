libtinyxml.a:tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o
	ar rv $@ $^

tinystr.o:tinystr.cpp
	g++ -c $^

tinyxml.o:tinyxml.cpp
	g++ -c $^

tinyxmlerror.o:tinyxmlerror.cpp
	g++ -c $^

tinyxmlparser.o:tinyxmlparser.cpp
	g++ -c $^

clean:
	rm -rf tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o libtinyxml.a

install:
	cp -rf libtinyxml.a ${MAXNET}/bin/dll/linux64/debug/


