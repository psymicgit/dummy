#ifndef __SDF_UTILS_SERVER_H_
#define __SDF_UTILS_SERVER_H_

#include "sdtype.h"

namespace SGDP {

// ≈‰÷√Œƒµµ∫Í∂®“Â
#define   ELEMENT_MINITOR                     "Monitor"
#define   ELEMENT_ITEM_LISTENER               "Listeners"
#define   ELEMENT_SERVER_NAME				  "Server"
#define   ELEMET_ITEM_SETTING				  "Setting"
#define   ELEMET_ITEM_ITEM                    "Item"
#define   ELEMET_ITEM_OPTION                  "Option"
#define   ELEMET_ITEM_CONFIG                  "config"
#define   ELEMET_ITEM_CONNECTIONS             "connections"
#define   ELEMET_ITEM_CONNECTION              "connection"
#define   ELEMET_ITEM_ITEM_SMALL              "item"
#define   ELEMET_ITEM_LISTEN				  "listen"
#define   ELEMET_ITEM_NETMODULE_COUNT         "NetModuleCount"
#define   ELEMET_ITEM_PIPEMODULE_COUNT        "PipeModuleCount"
#define   ELEMET_ITEM_SHOW_CONNECTION         "ShowConnectionStatus"
#define   ELEM_MAXUSER						  "MaxUsers"
#define   ELEM_PIPE                           "pipeconf"
#define   ELEM_IPLIST                         "iplist"
#define   ELEM_ARROW                          "Allow"

#define   ELEM_NET_LOG_LEVEL                    "NetLogLevel"
#define   ELEM_PIPE_LOG_LEVEL                    "PipeLogLevel"
#define   ELEM_DB_LOG_LEVEL                      "DbLogLevel"


#define   ATRRI_RECVBUFF                      "recvbuf"
#define   ATRRI_SENDBUFF                      "sendbuf"
#define   ATTRI_REMOTE_IP                     "remoteip"
#define   ATTRI_REMOTE_PORT                   "remoteport"
#define   ATTRI_LOCAL_IP                      "localip"
#define   ATTRI_LOCAL_PORT                    "localport"
#define   ATTRI_IP                            "ip"
#define   ATTRI_PORT                          "port"

#define   ATTRI_NAME					      "name"
#define   ATTRI_ID							  "id"
#define   ATTRI_FILE						  "file"
#define   ATTRI_SERVER_NAME			          "servername"
#define   ATTRI_SERVER_ID			          "serverid"
#define   ATTRI_BUSINESS_ID                   "BusinessID"
#define   ATTR_LISTNIP						  "listenip"
#define   ATTR_LISTNPORT					  "listenport"
#define   ATTR_MODE 						  "mode"
#define   ATTR_MODE_CONNECT					  "connect"
#define   ATTR_MODE_LISTEN					  "listen"


#define	MAX_SVRNAME_LEN				64
#define	MAX_IP_LEN					32
#define MAX_LISTEN_INFO				8
#define PACKET_LENGTH				1024000 
#define MAX_USERDATA_LEN            64

#define BUSINESSID_ALL     		    0


#define MSG_MARK	_SDTA2T(__FUNCTION__), __LINE__

}

#endif

