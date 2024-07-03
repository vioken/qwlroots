// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwconfig.h>
#include <QtCore/qglobal.h>

#include <concepts>

#define QW_NAMESPACE QWLRoots

#define QW_BEGIN_NAMESPACE namespace QW_NAMESPACE {
#define QW_END_NAMESPACE }
#define QW_USE_NAMESPACE using namespace QW_NAMESPACE;

#if defined(QW_STATIC_LIB)
#  define QW_EXPORT
#else
#if defined(QW_LIBRARY)
#  define QW_EXPORT Q_DECL_EXPORT
#else
#  define QW_EXPORT Q_DECL_IMPORT
#endif
#endif

#define QW_DECLARE_PRIVATE(Class) Q_DECLARE_PRIVATE_D(qGetPtrHelper(qw_d_ptr), Class)
#define QW_DECLARE_PUBLIC(Class) Q_DECLARE_PUBLIC(Class)
#define QW_PRIVATE_SLOT(Func) Q_PRIVATE_SLOT(d_func(), Func)


/*
 *  We disallow some class's destructor, they are binding of a wlroots struct
 *  which will be free in c language
 *  We use destroying delete to avoid call destructor after delete, but clang++
 *  still check destructor must not deleted nor private, we call abort() to ensure
 *  destructor never called in runtime
 *
 *  More info https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0722r3.html
 */
#ifdef __clang__
    #define QW_DISALLOW_DESTRUCTOR(Class) ~Class() { std::abort(); };
#else
    #define QW_DISALLOW_DESTRUCTOR(Class) ~Class() = delete;
#endif

#ifdef QT_NO_DEBUG
#define QW_ALWAYS_INLINE Q_ALWAYS_INLINE
#else
#define QW_ALWAYS_INLINE
#endif

#ifndef WLR_USE_UNSTABLE
#define WLR_USE_UNSTABLE
#endif

#include <QScopedPointer>
#include <QObject>

QW_BEGIN_NAMESPACE

class QWObject;
class QWObjectPrivate
{
public:
    virtual ~QWObjectPrivate();

protected:
    QWObjectPrivate(void *handle, bool isOwner, QWObject *qq);

    QWObject *q_ptr;
    void *m_handle;
    bool isHandleOwner;

    Q_DECLARE_PUBLIC(QWObject)
};

class QW_EXPORT QWObject
{
public:
    template<typename Handle>
    inline Handle *handle() const {
        if (!isValid()) {
            return nullptr;
        }
        return reinterpret_cast<Handle*>(qw_d_ptr->m_handle);
    }

    virtual ~QWObject();
    inline bool isValid() const {
        // NOTE(lxz): Some functions of wlroots allow null pointer parameters. In order to reduce repeated verification code fragments, If this ptr is nullptr, return nullptr.
        // WARNING(lxz): Check this in the member function, it is UB. Under some compilers it is necessary to use `volatile` to prevent compiler optimizations.
        //               In the derived class, if the object address is nullptr, the address of this is not necessarily 0x0, it may be 0x01. The correct memory address starting position will not be lower than 0x1000, so it is considered that addresses lower than 0x1000 are nullptr.
        volatile auto thisPtr = reinterpret_cast<qintptr>(this);
        return thisPtr > 0x1000 && qw_d_ptr->m_handle;
    }

    void setData(void* owner, void* data);

    template<typename T>
    T* getData() const {
        return reinterpret_cast<T*>(m_data.second);
    }

protected:
    QWObject(QWObjectPrivate &dd);
    QScopedPointer<QWObjectPrivate> qw_d_ptr;

    Q_DISABLE_COPY(QWObject)
    QW_DECLARE_PRIVATE(QWObject)

private:
    std::pair<void*, void*> m_data; // <owner, data>
};

class QWWrapObjectPrivate;
class QW_EXPORT QWWrapObject : public QObject, public QWObject
{
    Q_OBJECT

Q_SIGNALS:
    void beforeDestroy();

protected:
    QWWrapObject(QWWrapObjectPrivate &dd, QObject *parent = nullptr);
    ~QWWrapObject();

    QW_DECLARE_PRIVATE(QWWrapObject)
};

template<typename Handle, typename Derive>
class qw_reinterpret_cast
{
public:
    typedef Handle HandleType;
    typedef Derive DeriveType;

    QW_ALWAYS_INLINE Handle *handle() const {
        return reinterpret_cast<Handle*>(const_cast<Derive*>(static_cast<const Derive*>(this)));
    }

    QW_ALWAYS_INLINE static Derive *from(Handle *handle) {
        return reinterpret_cast<Derive*>(handle);
    }

    QW_ALWAYS_INLINE operator Handle* () const {
        return handle();
    }

    QW_ALWAYS_INLINE void operator delete(qw_reinterpret_cast *p, std::destroying_delete_t) {
        if constexpr (std::is_same<decltype(static_cast<Derive*>(p)->destroy()), void>::value) {
            static_cast<Derive*>(p)->Derive::destroy();
        } else {
            static_assert(false, "Can't destroy.");
        }
    }

private:
    qw_reinterpret_cast() = delete;
    QW_DISALLOW_DESTRUCTOR(qw_reinterpret_cast)
};

#define QW_CLASS_REINTERPRET_CAST(wlr_type_suffix) \
qw_##wlr_type_suffix : public qw_reinterpret_cast<wlr_##wlr_type_suffix, qw_##wlr_type_suffix>

#define QW_FUNC_MEMBER(wlr_type_suffix, wlr_func_suffix) \
template<typename ...Args> \
QW_ALWAYS_INLINE auto wlr_func_suffix(Args &&... args) { \
    return wlr_##wlr_type_suffix##_##wlr_func_suffix(*this, std::forward<Args>(args)...); \
}

#define QW_FUNC_STATIC(wlr_type_suffix, wlr_func_suffix) \
template<typename ...Args> \
QW_ALWAYS_INLINE static auto wlr_func_suffix(Args &&... args) { \
    return wlr_##wlr_type_suffix##_##wlr_func_suffix(std::forward<Args>(args)...); \
}

#define QW_MAYBE_FUNC_STATIC(wlr_type_suffix, wlr_func_suffix) \
template<typename ...Args> \
QW_ALWAYS_INLINE static void wlr_func_suffix(Args &&... args) { \
    constexpr bool exists = requires() { \
        wlr_##wlr_type_suffix##_##wlr_func_suffix(std::forward<Args>(args)...); \
    }; \
    if constexpr (exists) \
        wlr_##wlr_type_suffix##_##wlr_func_suffix(std::forward<Args>(args)...); \
}

// 1. clean functions
//.+wlr_([a-z]+)_([a-z_]+)\(.+ QW_FUNC_MEMBER($1, $2)

// 2. clean signals
//.+sc.connect\(&handle->events.([^,]+),.+ QW_SIGNAL($1, arguments?)

// 3. clean cpp files
//^(?!QW_[A-Z_]+\()(?!#)(?!extern).*\n

QW_END_NAMESPACE
