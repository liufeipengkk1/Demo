
#ifndef _BYTED_RENDER_CORE_SHAREPTR_H
#define _BYTED_RENDER_CORE_SHAREPTR_H


#include <iostream>
using namespace std;

template<class T>
class SharePtr
{
public:
    typedef T element_type;
   
    SharePtr() : _ptr(0) {}
    
    SharePtr(T* ptr) : _ptr(ptr) { if (_ptr) _ptr->Ref(); }

    SharePtr(const SharePtr& rp) : _ptr(rp._ptr) { if (_ptr) _ptr->Ref(); }
  
    template<class Other> SharePtr(const SharePtr<Other>& rp) : _ptr(rp._ptr) { if (_ptr) _ptr->Ref(); }
  
    ~SharePtr() { if (_ptr) _ptr->Unref();  _ptr = 0; }
    
    SharePtr& operator = (const SharePtr& rp)
    {
        assign(rp);
        return *this;
    }
    
    template<class Other> SharePtr& operator = (const SharePtr<Other>& rp)
    {
        assign(rp);
        return *this;
    }

    inline SharePtr& operator = (T* ptr)
    {
        if (_ptr==ptr) return *this;
        T* tmp_ptr = _ptr;
        _ptr = ptr;
        if (_ptr) _ptr->Ref();
        // Unref second to prevent any deletion of any object which might
        // be referenced by the other object. i.e rp is child of the
        // original _ptr.
        if (tmp_ptr) tmp_ptr->Unref();
        return *this;
    }
    
    bool operator == (const SharePtr& rp) const { return (_ptr==rp._ptr); }
    bool operator == (const T* ptr) const { return (_ptr==ptr); }
    friend bool operator == (const T* ptr, const SharePtr& rp) { return (ptr==rp._ptr); }

    bool operator != (const SharePtr& rp) const { return (_ptr!=rp._ptr); }
    bool operator != (const T* ptr) const { return (_ptr!=ptr); }
    friend bool operator != (const T* ptr, const SharePtr& rp) { return (ptr!=rp._ptr); }

    bool operator < (const SharePtr& rp) const { return (_ptr<rp._ptr); }
    
private:
    typedef T* SharePtr::*unspecified_bool_type;
    
public:
    // safe bool conversion
    operator unspecified_bool_type() const { return valid()? &SharePtr::_ptr : 0; }
    
    T& operator*() const { return *_ptr; }
    
    T* operator->() const { return _ptr; }

    T* get() const { return _ptr; }

    //operator T* () { return _ptr; }
    
    bool operator!() const   { return _ptr == 0; } // not required
    
    bool valid() const       { return _ptr != 0; }

    T* release() { T* tmp=_ptr; if (_ptr) _ptr->UnrefNoDelete(); _ptr = 0; return tmp; }
    
    void swap(SharePtr& rp) { T* tmp=_ptr; _ptr=rp._ptr; rp._ptr=tmp; }
    
private:
    
    template<class Other> void assign(const SharePtr<Other>& rp)
    {
        if (_ptr==rp._ptr) return;
        T* tmp_ptr = _ptr;
        _ptr = rp._ptr;
        if (_ptr) _ptr->Ref();
        // Unref second to prevent any deletion of any object which might
        // be referenced by the other object. i.e rp is child of the
        // original _ptr.
        if (tmp_ptr) tmp_ptr->Unref();
    }
    
    template<class Other> friend class SharePtr;
    
    T* _ptr;
};


template<class T> inline
void swap(SharePtr<T>& rp1, SharePtr<T>& rp2) { rp1.swap(rp2); }

template<class T> inline
T* get_pointer(const SharePtr<T>& rp) { return rp.get(); }

template<class T, class Y> inline
SharePtr<T> static_pointer_cast(const SharePtr<Y>& rp) { return static_cast<T*>(rp.get()); }

template<class T, class Y> inline
SharePtr<T> dynamic_pointer_cast(const SharePtr<Y>& rp) { return dynamic_cast<T*>(rp.get()); }

template<class T, class Y> inline
SharePtr<T> const_pointer_cast(const SharePtr<Y>& rp) { return const_cast<T*>(rp.get()); }

#endif /* SharePtr_h */
