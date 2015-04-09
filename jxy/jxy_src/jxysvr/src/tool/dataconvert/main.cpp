#include <iostream>
#include <sddb.h>
#include <dll/sdframework/sdloggerimpl.h>
#include <dll/sdnet/sdnet.h>
#include "convert.h"
using namespace std;
using namespace SGDP;

void Exit()
{
	std::system("pause");
	exit(0);
}

int main()
{
	string strCfgName("loggercfg.xml");
	if( !CSDLoggerImpl::Init( strCfgName.c_str() ) )
	{
		cout<<"日志初始化失败，请检查配置文件："<<strCfgName<<endl;
		Exit();
	}

// 	CSDComLoggerImpl netLogger;
// 	netLogger.Init(_SDT("[SDNET] "));
	//SGDP::SDNetSetLogger(&netLogger, 0);


	USR_INFO( _SDT( "开始转换" ) );
	Convert conv;
	if( !conv.Init() )
	{
		cout<<"初始化失败，转换失败"<<endl;
		Exit();
	}

	cout<<"开始执行"<<endl;
	if( !conv.Process() )
	{
		cout<<"处理失败！处理详细信息请查看log"<<endl;
		SYS_CRITICAL( _SDT( "处理失败!" ) );
	}
	else
	{
		cout<<"执行完毕:"<<endl<<conv.GetResultDes();
	}
	if( !conv.UnInit() )
	{
		cout<<"释放时失败"<<endl;
		Exit();
	}
	USR_INFO( "执行完毕:%s", conv.GetResultDes().c_str() );
	CSDLoggerImpl::UnInit();
	Exit();
	return 0;
}