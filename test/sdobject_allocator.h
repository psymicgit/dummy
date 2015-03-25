#ifndef __OBJECT_ALLOCATOR_H__
#define __OBJECT_ALLOCATOR_H__

#include <deque>
#include <vector>

typedef int INT32;
typedef unsigned int UINT32;

namespace SGDP
{
template<typename T>
class ObjectAllocator
{
    enum
    {
        POOL_INFLATE_SIZE = 100,
    };
public:
    typedef size_t   size_type;
    typedef T*       pointer;
    typedef T&       reference;
    typedef const T* const_pointer;
    typedef const T& const_reference;
    typedef T        value_type;

    typedef std::deque<pointer> ObjectQueue;

    typedef typename ObjectQueue::iterator ObjectQueueItr;

    ObjectAllocator(INT32 initSize = 0, INT32 growSize = 1 ) throw()
    {
        m_initSize	= initSize;
        m_totalSize	= 0;
        m_growSize	= growSize;
        inflate(initSize);
    }


    ~ObjectAllocator() throw()
    {
        for(typename ObjectQueue::iterator itr = m_objects.begin(); itr != m_objects.end(); ++itr)
        {
            ::operator delete (*itr);
        }
        m_objects.clear();
    }

    pointer address(reference __x) const
    {
        return &__x;
    }

    const_pointer address(const_reference __x) const
    {
        return &__x;
    }

    T* allocate()
    {
        if (m_objects.empty())
        {
            this->inflate(m_growSize);
        }

        T * ret = m_objects.front();
        m_objects.pop_front();
        return ret;
    }

    // __p is not permitted to be a null pointer.
    void deallocate(pointer __p)
    {
        for(typename ObjectQueue::iterator itr = m_objects.begin(); itr != m_objects.end(); ++itr)
        {
            // find pointer in queue.
            if (*itr == __p)
            {
                return;
            }
        }
        m_objects.push_back(__p);

        //对于在内存池中的对象数据，当超过一定空闲个数时释放，防止内存出现暴涨
        if (m_objects.size() > (UINT32)(m_initSize + m_growSize + POOL_INFLATE_SIZE))
        {
            T * ret = m_objects.front();
            m_objects.pop_front();
            ::operator delete (ret);
            m_totalSize--;
        }
    }

    inline size_type read_size()
    {
        return m_objects.size();
    }

    inline size_type write_size() const throw()
    {
        return m_objects.size() ;
    }

private:
    void inflate(INT32 count = POOL_INFLATE_SIZE)
    {
        //NOTICE: locked outside
        for (INT32 i = 0; i < count ; i++)
        {
            pointer  _p = static_cast<pointer>(::operator new (sizeof(value_type)));
            m_objects.push_back(_p);
        }
        m_totalSize += count;
    }

    ObjectAllocator(const ObjectAllocator&) throw()
    {
    }

    // not allow copy
    template<typename T1>
    ObjectAllocator(const ObjectAllocator<T1>&) throw()
    {
    }

    template<typename T1>
    void operator = (const ObjectAllocator<T1>&) throw()
    {

    }

    ObjectQueue m_objects;
    INT32	m_totalSize ;
    INT32	m_growSize;
    INT32	m_initSize;
};
}
#endif // 



