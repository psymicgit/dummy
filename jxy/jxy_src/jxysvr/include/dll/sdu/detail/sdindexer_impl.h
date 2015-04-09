#ifndef SDINDEXER_IMPL_H
#define SDINDEXER_IMPL_H

namespace SGDP
{

#ifndef SDINVALID_INDEX
#define SDINVALID_INDEX 0  //定义无效的索引为0 
#endif

	template <class T>
	struct Container
	{
		Container()
		{
			bUsed = FALSE;
			mask = 0;
		}
		UINT32 mask;
		volatile UINT32 bUsed;
		T data;
	};

	template <class T> 
	struct DefaultValue
	{
		void operator() (T & data)
		{
			data = T();
		}
	};

	template <class T> 
	struct DefaultValue<T * > 
	{
		void operator() (T & data)
		{
			data = NULL;
		}
	};

	template <class T>
	struct DefaultEqual
	{
		bool operator ()( const T& left, const T& right)
		{
			return left == right;
		}
	};


	template < class T, UINT32 maxSize = 1024, class ValueEqual = DefaultEqual<T> >
	class SDIndexerImpl
	{
	public:
		SDIndexerImpl()
		{
			DefaultValue<T> defValue;
			for (UINT32 i = 0; i < maxSize; i ++)
			{
				defValue(m_array[i].data);
				m_array[i].bUsed = FALSE;
			}
			m_curIndex = maxSize >> 1;
			m_freeCount = maxSize - 1;
		}

		UINT32 Alloc(T & value)
		{
			while (m_freeCount > 0)
			{
				for (UINT32 curIdx = m_curIndex; curIdx < maxSize; curIdx++)
				{
					//当此Index未被使用时
					if ( !SDAtomicCas32((volatile INT32*)&(m_array[curIdx].bUsed), TRUE, FALSE) )
					{
						m_array[curIdx].data = value;
						//减一操作,如果多线程同时减,会将此操作数置0
						SDAtomicDec32((volatile INT32*)&m_freeCount);
						SDAtomicSet32((volatile INT32 *)&m_curIndex, (m_curIndex)%(maxSize-2) + 1);
						return curIdx;
					}
				}

				for (UINT32 curIdx = 1; curIdx < m_curIndex; curIdx++)
				{
					//当此Index未被使用时
					if ( !SDAtomicCas32((volatile INT32*)&(m_array[curIdx].bUsed), TRUE, FALSE) )
					{
						m_array[curIdx].data = value;
						//减一操作,如果多线程同时减,会将此操作数置0
						SDAtomicDec32((volatile INT32*)&m_freeCount);
						SDAtomicSet32((volatile INT32 *)&m_curIndex, (m_curIndex)%(maxSize - 2) + 1);
						return curIdx;
					}
				}
			}
			return SDINVALID_INDEX;
		}

		//根据数据查找对应的ID 
		UINT32 Find(const T & data)
		{
			for (UINT32 i = 1; i < maxSize; i++)
			{
				if (m_equaler(m_array[i].data, data))
				{
					return i;
				}
			}
			return SDINVALID_INDEX;
		}

		BOOL Exist(UINT32 idx)
		{
			if (idx < maxSize)
			{
				return m_array[idx].bUsed;
			}
			return FALSE;
		}

		T & Get(UINT32 idx)
		{
			static T defValue;
			if (idx < maxSize && m_array[idx].bUsed)
			{
				return m_array[idx].data;
			}
			return defValue;
		}

		T& Free(UINT32 idx)
		{
			static T defValue;
			if (idx < maxSize && idx > 0)
			{
				SDAtomicInc32((volatile INT32 *)&m_freeCount);
				SDAtomicCas32((volatile INT32*)&(m_array[idx].bUsed), FALSE, TRUE);
				return m_array[idx].data;
			}
			return defValue;
		}

	private:
		ValueEqual  m_equaler;
		UINT32 m_curIndex;
		UINT32 m_indexMask;
		UINT32 m_freeCount;
		Container<T> m_array[maxSize];
	};

}

#endif // 


