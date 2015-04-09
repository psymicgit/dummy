
#ifndef DB_CONN_H_20100728
#define DB_CONN_H_20100728

#include "sddb.h"

/**
  @desc Connection的接口类。
  之所以设计了IConn和ISDDBConnection两个类，
  是因为，IConn提供了一些不宜暴露给用户的函数。
  另外，内部所有的类和函数，使用的都是IConn而不是ISDDBConnection。
**/
class IConn : public SGDP::ISDDBConnection
{
public:
	/**
	  @desc IConn的析构函数
	**/
	virtual SDAPI ~IConn(){};

	/**
	  @desc 该连接是否被使用
	  @return true代表被使用，false代表未被使用
	**/
	virtual bool IsUsed() = 0;

	/**
	  @param bUsed 设置该连接是否被使用，
	  true代表被使用，false代表没有被使用
	  @desc 设置该连接是否被使用
	**/
	virtual void SetUsed(bool bUsed) = 0;
};

#endif

