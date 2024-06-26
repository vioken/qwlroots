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

template<typename Handle, typename Impl>
class qw_interface
{
protected:
    struct _handle : public Handle {
        qw_interface *interface;
    };

public:
    typedef Handle HandleType;
    typedef Impl ImplType;

    QW_ALWAYS_INLINE Handle *handle() const {
        return m_handle;
    }

    QW_ALWAYS_INLINE Impl *impl() const {
        return m_handleImpl;
    }

    QW_ALWAYS_INLINE operator Impl* () const {
        return impl();
    }

    QW_ALWAYS_INLINE static qw_interface *get(Handle *handle) {
        auto h = static_cast<_handle*>(handle);
        Q_ASSERT(h->interface && h->impl == handle->impl);
        return h->interface;
    }

protected:
    template <typename I, I i, typename II, II ii>
    struct qw_interface_binder {
        typedef I interface_type;
        typedef II impl_type;

        template<typename T>
        struct caller {};

        template <class Obj, typename Ret, typename ...Args>
        struct caller<Ret (Obj::*) (Args...)> {
            typedef Ret return_type;
            // typedef Args... arguments;

            static Ret call(HandleType *handle, Args &&... args) {
                return (static_cast<_handle*>(handle)->interface->*i)(std::forward<Args>(args)...);
            }
        };

        typedef caller<I> callback;
        typedef typename callback::return_type return_type;

        static inline void bind(qw_interface *self) {
            self->m_handleImpl->*ii = callback::call;
        }

        qw_interface_binder(qw_interface *self) {
            bind(self);
        }
    };

    template <typename I, typename II>
    void bind_interface(I impl, II i) {
        qw_interface_binder<I, impl, II, i>::bind(this);
    }

    static void destroy(Handle *handle) {
        auto self = get(handle);
        Q_ASSERT(self);
        delete self;
    }

    qw_interface()
        : m_handleImpl(new Impl)
        , m_handle(calloc(1, sizeof(_handle)))
    {
        static_cast<_handle*>(m_handle)->interface = this;
        constexpr bool has_destroy = requires(const Impl &i) {
            i.destroy;
        };

        if constexpr (has_destroy) {
            m_handleImpl->destroy = destroy;
        }
    }

    virtual ~qw_interface() {
        free(handle());
        delete impl();
    }

    Handle *m_handle = nullptr;
    Impl *m_handleImpl = nullptr;
};

#define QW_CLASS_INTERFACE(wlr_type_suffix) \
qw_##wlr_type_suffix##_interface : public qw_interface<wlr_##wlr_type_suffix, wlr_##wlr_type_suffix##_impl>

#define QW_INTERFACE(name) \
private: \
template <typename I, typename II> \
struct qw_interface_##name { \
    static_assert(std::is_base_of<qw_interface, I>::value, "Please inherit the qw_foo_interface calss."); \
    qw_interface_##name(qw_interface *self) { \
        constexpr bool has_impl = requires(const I &i) { \
            &i.name; \
        }; \
        constexpr bool has_interface = requires(const II &i) { \
            &i.name; \
        }; \
        if constexpr (has_impl) { \
            if constexpr (has_interface) { \
                self->bind_interface(&I::name, &II::name); \
            } else { \
                static_assert(false, "Not found \"" QT_STRINGIFY(name) "\", Please check your wlroot's version."); \
            } \
        } else { \
            self->impl()->name = nullptr; \
        } \
    } \
}; \
qw_interface_##name<Derive, ImplType> _interface_##name = this;

QW_END_NAMESPACE
