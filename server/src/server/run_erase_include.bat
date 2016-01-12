rem D:\proj\linux\llvm\build\MinSizeRel\bin\erase_include.exe -extra-arg-before="-I./ -I../../3rd -I../../3rd/glog-0.3.3/src/windows -I../../3rd/protobuf/protobuf-2.5.0/src -I../protocol -I../../3rd/mysql/include -I../../3rd/curl/include -I../../3rd/rapidjson/include" -extra-arg-before="-includestdafx.h" server.cpp --

clang -I./ -I../../3rd -I../../3rd/glog-0.3.3/src/windows -I../../3rd/protobuf/protobuf-2.5.0/src -I../protocol -I../../3rd/mysql/include -I../../3rd/curl/include -I../../3rd/rapidjson/include -includestdafx.h -fexceptions server.cpp

pause


