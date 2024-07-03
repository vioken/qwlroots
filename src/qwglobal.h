// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwconfig.h>
#include <QtCore/qglobal.h>

#include <concepts>

#ifdef QW_NAMESPACE
#define QW_BEGIN_NAMESPACE namespace QW_NAMESPACE {
#define QW_END_NAMESPACE }
#define QW_USE_NAMESPACE using namespace QW_NAMESPACE;
#else
#define QW_NAMESPACE
#define QW_BEGIN_NAMESPACE
#define QW_END_NAMESPACE
#define QW_USE_NAMESPACE
#endif

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

QW_BEGIN_NAMESPACE

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

QW_END_NAMESPACE
