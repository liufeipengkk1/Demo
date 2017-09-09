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
    /*!
     *@brief 默认构造函数
     */
    Referenced()
    {
		m_RefCount = 0;
    }
    
    /*!
     *@brief 拷贝构造函数
     */
    Referenced(const Referenced&)
    {
		m_RefCount = 0;
    }
    
    /*!
     *@brief 重载赋值符号
     *@return 当前对象的引用
     */
    Referenced& operator = (const Referenced&) { return *this; }
    
    /*!
     *@brief 增加引用计数，增加引用该对象的引用计数。
     *@see void Unref()
     */
    virtual void Ref()
    {
        m_RefCount++;
    }
    
    /*!
     *@brief 减少引用计数，当引用计数的数目为零时，引用对象将被自动删除。
     *@see void Ref()
     */
    virtual void Unref()
    {
        m_RefCount--;
        if (m_RefCount == 0)
        {
            delete this;
        }
    }
    
    /*!
     *@brief 减少对象的引用计数，但不执行删除操作
     *@remark 调用者需要清除的知道谁负责该引用对象的删除，否则会造成内存泄露。
     */
    void UnrefNoDelete() const { --m_RefCount; }
    
    /*!
     *@brief 获得对象被引用的数目。
     *@return 引用计数个数
     */
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
