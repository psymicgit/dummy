include ${MAXNET}/project/linux/makebase/make_begin


MODULE			= server

LIBA_FILE 		= 
LIBSO_FILE		= 
EXEC_FILE 		= gateserver

OTHER_SRC_C	= 
OTHER_SRC_CPP   =       ${MAXNET_SRC_DIR}/server/common/server/utility.cpp \
						${MAXNET_SRC_DIR}/server/common/server/packetparser.cpp \
                        ${MAXNET_SRC_DIR}/server/protocol/server/protocligs.cpp \
                        ${MAXNET_SRC_DIR}/server/protocol/server/protocligt.cpp \
                        ${MAXNET_SRC_DIR}/server/protocol/server/protoclils.cpp \
                        ${MAXNET_SRC_DIR}/server/protocol/server/protoclizs.cpp \
                        ${MAXNET_SRC_DIR}/server/protocol/server/protocommondata.cpp \
                        ${MAXNET_SRC_DIR}/server/protocol/server/protogsdb.cpp \
                        ${MAXNET_SRC_DIR}/server/protocol/server/protogsls.cpp \
                        ${MAXNET_SRC_DIR}/server/protocol/server/protogtcommon.cpp \
                        ${MAXNET_SRC_DIR}/server/protocol/server/protogtgs.cpp \
                        ${MAXNET_SRC_DIR}/server/protocol/server/protogtls.cpp
INLIB_FILE		= -lpthread -lz -lncurses -liconv -lcharset -lsqlite3 -lsdframework -lsdnet -lsdlogger -lsdpipe -lsdconsole -lsdu -ltinyxml -lsddb -lcurl -ljson_linux-gcc-4.7.0_libmt
INLIB_DIR		= -L${THIRDP_DIR}/tinyxml/lib/linux64/debug -L/usr/local/lib -L/usr/local/lib64 
INC_PATH 		= -I${MAXNET_INC_DIR} \
                  -I${MAXNET_INC_DIR}/dll \
				  -I${MAXNET_INC_DIR}/dll/sdu \
				  -I${MAXNET_INC_DIR}/dll/sdlogger \
				  -I${MAXNET_INC_DIR}/dll/sdframework \
				  -I${MAXNET_INC_DIR}/dll/sdconsole \
				  -I${MAXNET_INC_DIR}/dll/sdnet \
				  -I${MAXNET_INC_DIR}/dll/sddb \
				  -I${MAXNET_INC_DIR}/dll/sdpipe \
				  -I${MAXNET_SRC_DIR}/server \
				  -I${MAXNET_SRC_DIR}/server/common \
				  -I${MAXNET_SRC_DIR}/server/common/server \
				  -I${MAXNET_SRC_DIR}/server/common/client \
				  -I${MAXNET_SRC_DIR}/server/protocol/server \
				  -I${MAXNET_SRC_DIR}/server/gateserver \
				  -I${THIRDP_DIR}/tinyxml/include \
				  -I/usr/share/gettext/intl \
				  -I${THIRDP_DIR}/ \
				  -I${THIRDP_DIR}/jsoncpp-src/include



include ${MAXNET}/project/linux/makebase/make_end
