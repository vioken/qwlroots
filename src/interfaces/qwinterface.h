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

QW_END_NAMESPACE
