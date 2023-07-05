// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwconfig.h>
#include <QtCore/qglobal.h>

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

#include <QScopedPointer>

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

QW_END_NAMESPACE
