include ../make_begin

PCH_H	=	stdafx.h
PCH		=	stdafx.h.gch

MODULE			= server
MACD            = -DLUA_API

MODULE			= dll

LIBA_FILE 		= libserver.a
LIBSO_FILE		= 
EXEC_FILE 		=

COMP_OBJ		= ${COMM_OBJ}
INLIB_FILE		= -lpthread -lz -lncurses -liconv -lcharset
INLIB_DIR		= -L/usr/local/lib -L/usr/local/lib64 -L/usr/lib -L/usr/lib64 
INC_PATH 		= -I./ -I${base_dir} -I../protocol -I${3rd_dir}/protobuf/protobuf-2.5.0/src -I${3rd_dir}/mysql/include

# -I${3rd_dir}/glog-0.3.3/src/windows 

include ../make_end
