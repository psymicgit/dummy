#include "GateLogic.h"
#include "game_to_gate.pb.h"

void GateLogic::SendToGameByClient(int clientId, int msgId, const char* msg, int msgSize)
{
	RouteFromClientMsg routeMsg;
	routeMsg.set_client_id(clientId);
	routeMsg.set_msg_id(msgId);
	routeMsg.set_msg(msg, msgSize);

	GateServer::Instance().SendToGameServer(GateToGame_RouteFromClient, routeMsg);
}