#ifndef _FILE_SINGLETON_H_
#define _FILE_SINGLETON_H_ 

#include "syncobjs.h"

namespace sbase
{
	template
		<typename T,
	class ThreadingModel = sbase::SingleThreaded<T>
		>
	class  Singleton
	{
	public:
		static T& Instance()
		{
			if ( NULL == S_instance )
			{
				Guard();
				if( NULL == S_instance )
				{		 
					S_instance = new T;
				}
			}

			return *S_instance;
		}
	protected:
		Singleton(){};
		~Singleton(){};

	private:
		static T *S_instance;
		typedef typename ThreadingModel::Lock Guard;
	};

}

#define INSTANTIATE_SINGLETON_1(TYPE) \
	template class  sbase::Singleton<TYPE,sbase::SingleThreaded<TYPE> >; \
	template<> TYPE* sbase::Singleton<TYPE,sbase::SingleThreaded<TYPE> >::S_instance = NULL

#define INSTANTIATE_SINGLETON_MULTITHREADING(TYPE,THREADINLOCK) \
	template class  sbase::Singleton<TYPE,THREADINLOCK>; \
	template<> TYPE* sbase::Singleton<TYPE,THREADINLOCK >::S_instance = NULL

#endif


