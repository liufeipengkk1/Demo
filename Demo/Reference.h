#ifndef _RENDER_CORE_REFERENCE_H
#define _RENDER_CORE_REFERENCE_H

#include <iostream>
#include <atomic>
using namespace std;

/*!
 *@class Referenced
 *@brief 具有引用计数的实现类，需要引用计数的类，需要从此类继承
 * 该接口主要为了统一对象的引用计数操作，给出了默认的实现。
 */
class Referenced
{
public:
    Referenced()
    {
		m_RefCount = 0;
    }
    
    Referenced(const Referenced&)
    {
		m_RefCount = 0;
    }

    Referenced& operator = (const Referenced&) { return *this; }

    virtual void Ref()
    {
        m_RefCount++;
    }

    virtual void Unref()
    {
        m_RefCount--;
        if (m_RefCount == 0)
        {
            delete this;
        }
    }

    void UnrefNoDelete() const { --m_RefCount; }

    long ReferenceCount() const { return m_RefCount; }
    
    // IntrusivePtrAddRef and intrusive_ptr_release allow
    // use of osg Referenced classes with boost::intrusive_ptr
    void IntrusivePtrAddRef(Referenced* p) { p->Ref(); }
    void IntrusivePtrRelease(Referenced* p) { p->Unref(); }
    
protected:
    virtual ~Referenced() { /*assert(m_RefCount == 0);*/}
    mutable std::atomic_ulong m_RefCount;
};


#endif /* Reference.h */
