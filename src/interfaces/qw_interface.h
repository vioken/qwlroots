// Copyright (C) 2023 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

#include <type_traits>

QW_BEGIN_NAMESPACE

class QW_EXPORT QWInterface
{
public:
    template<typename Handle>
    inline Handle *handle() const {
        return reinterpret_cast<Handle*>(m_handle);
    }

    template<typename Impl>
    inline Impl *impl() const {
        return reinterpret_cast<Impl*>(m_handleImpl);
    }

protected:
#define QW_INIT_INTERFACE_FUNC(funMagicKey, member, value)\
    .member = (funMagicKey & (1 << virtualFuncIndex(value))) ? nullptr : impl::member

    template <class Derived, typename F>
    inline static constexpr
    typename std::enable_if<std::is_base_of<QWInterface, typename QtPrivate::FunctionPointer<F>::Object>::value && std::is_base_of<QWInterface, Derived>::value, bool>::type
    functionOverrided(F) {
        using FunInfo = QtPrivate::FunctionPointer<F>;
        return std::is_same<typename FunInfo::Object, Derived>::value;
    }

    typedef quint64 FuncMagicKey;
    template <typename F>
    static inline uint virtualFuncIndex(F f) {
        const uint index = (*(quintptr*)&f) / sizeof(quintptr);
        assert(index < sizeof(FuncMagicKey) * 8);
        return index;
    }
    template <class Derived>
    static constexpr FuncMagicKey getFuncMagicKey() {
        return 0;
    }
    template <class Derived, typename Head, typename... Tail>
    static inline FuncMagicKey getFuncMagicKey(Head func, Tail&&... funcs) {
        constexpr int maxFuncCount = sizeof(FuncMagicKey) * 8;
        Q_STATIC_ASSERT(sizeof...(funcs) + 1 <= maxFuncCount);
        // Mark to 1 if the function is not overrided
        const FuncMagicKey magic = functionOverrided<Derived>(func) ? 0 : (1 << virtualFuncIndex(func));
        return magic | getFuncMagicKey<Derived>(std::forward<Tail>(funcs)...);
    }

    void *m_handle = nullptr;
    void *m_handleImpl = nullptr;
};

template<typename Handle, typename Impl, typename Derive>
class qw_interface
{
public:
    typedef Handle HandleType;
    typedef Impl ImplType;
    typedef Derive DeriveType;

    QW_ALWAYS_INLINE Handle *handle() const {
        return m_handle;
    }

    QW_ALWAYS_INLINE Impl *impl() const {
        return m_handleImpl;
    }

    QW_ALWAYS_INLINE operator Impl* () const {
        return impl();
    }

protected:
    template <class Derived, typename F>
    inline static constexpr
    typename std::enable_if<std::is_base_of<qw_interface, typename QtPrivate::FunctionPointer<F>::Object>::value && std::is_base_of<qw_interface, Derived>::value, bool>::type
    overrided(F) {
        using FunInfo = QtPrivate::FunctionPointer<F>;
        return std::is_same<typename FunInfo::Object, Derived>::value;
    }

    struct _handle : public Handle
    {
        Derive *interface;
    };

    qw_interface()
        : m_handleImpl(new Impl)
        , m_handle(calloc(1, sizeof(_handle)))
    {
        static_cast<_handle*>(m_handle)->interface = static_cast<Derive*>(this);
    }

    Handle *m_handle = nullptr;
    Impl *m_handleImpl = nullptr;
};

#define QW_CLASS_INTERFACE(wlr_type_suffix) \
template<typename Derive> \
class qw_##wlr_type_suffix##_interface : public qw_interface<wlr_##wlr_type_suffix, wlr_##wlr_type_suffix##_impl, Derive>

#define QW_INTERFACE_BIND(name) \
struct _interface_##name { \
    template <typename ...Args> \
    static auto callback(HandleType *handle, Args &&... args) { \
        return static_cast<_handle*>(handle)->interface->name(std::forward<Args>(args)...); \
    } \
    _interface_##name() { \
        if constexpr (!allows_null || overrided(DeriveType::name)) { \
            m_handleImpl->name = callback<typename QtPrivate::FunctionPointer<DeriveType::name>::Arguments>; \
        } else { \
            m_handleImpl->name = nullptr; \
        } \
    } \
} qw_interface_##name;

#define QW_INTERFACE0(name) \
virtual QtPrivate::FunctionPointer<ImplType::name>::ReturnType \
name(QtPrivate::FunctionPointer<ImplType::name>::Arguments::Cdr) = 0; \
QW_INTERFACE_BIND(name)

#define QW_INTERFACE1(name, default_value) \
virtual QtPrivate::FunctionPointer<ImplType::name>::ReturnType \
name(QtPrivate::FunctionPointer<ImplType::name>::Arguments::Cdr) { return default_value; } \
QW_INTERFACE_BIND(name)

QW_END_NAMESPACE
