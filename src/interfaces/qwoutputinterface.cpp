// Copyright (C) 2023 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwoutputinterface.h"
#include "qwbackend.h"
#include "types/qwbuffer.h"
#include "qwdisplay.h"

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_output.h>
#undef static
#include <wlr/interfaces/wlr_output.h>
}

QW_BEGIN_NAMESPACE

struct _wlr_output : public wlr_output
{
    _wlr_output(QWOutputInterface *i)
        : interface(i) {}

    QWOutputInterface *interface;
};

inline static QWOutputInterface *interface(wlr_output *handle) {
    auto _handle = static_cast<_wlr_output*>(handle);
    Q_ASSERT(_handle->interface && _handle->impl == handle->impl);
    return static_cast<_wlr_output*>(handle)->interface;
}

namespace impl {
static bool set_cursor(wlr_output *handle, wlr_buffer *buffer,
                       int hotspot_x, int hotspot_y) {
    return interface(handle)->setCursor(QWBuffer::from(buffer), QPoint(hotspot_x, hotspot_y));
}

static bool move_cursor(wlr_output *handle, int x, int y) {
    return interface(handle)->moveCursor(QPoint(x, y));
}

static void destroy(wlr_output *handle) {
    delete interface(handle);
}

static bool test(wlr_output *handle, const wlr_output_state *state) {
    return interface(handle)->test(state);
}

static bool commit(wlr_output *handle, const wlr_output_state *state) {
    return interface(handle)->commit(state);
}

static size_t get_gamma_size(wlr_output *handle) {
    return interface(handle)->getGammaSize();
}

static const wlr_drm_format_set *get_cursor_formats(
        wlr_output *handle, uint32_t buffer_caps) {
    return interface(handle)->getCursorFormats(buffer_caps);
}

static void get_cursor_size(wlr_output *handle, int *width, int *height) {
    const QPoint &pos = interface(handle)->getCursorSize();
    if (width)
        *width = pos.x();
    if (height)
        *height = pos.y();
}

static const wlr_drm_format_set *get_primary_formats(
        wlr_output *handle, uint32_t buffer_caps) {
    return interface(handle)->getPrimaryFormats(buffer_caps);
}
}

QWOutputInterface::~QWOutputInterface()
{
    free(handle());
    delete static_cast<wlr_output_impl*>(m_handleImpl);
}

bool QWOutputInterface::setCursor(QWBuffer *buffer, const QPoint &hotspot)
{
    Q_UNUSED(buffer);
    Q_UNUSED(hotspot);
    return false;
}

bool QWOutputInterface::moveCursor(const QPoint &pos)
{
    Q_UNUSED(pos);
    return false;
}

bool QWOutputInterface::test(const wlr_output_state *state) const
{
    Q_UNUSED(state);
    return true;
}

size_t QWOutputInterface::getGammaSize() const
{
    return 0;
}

const wlr_drm_format_set *QWOutputInterface::getCursorFormats(uint32_t bufferCaps) const
{
    Q_UNUSED(bufferCaps);
    return nullptr;
}

const wlr_drm_format_set *QWOutputInterface::getPrimaryFormats(uint32_t bufferCaps) const
{
    Q_UNUSED(bufferCaps);
    return nullptr;
}

QPoint QWOutputInterface::getCursorSize() const
{
    return QPoint(0, 0);
}

QWOutputInterface *QWOutputInterface::get(wlr_output *handle)
{
    return interface(handle);
}

void QWOutputInterface::init(FuncMagicKey funMagicKey, QWBackend *backend, QWDisplay *display)
{
    auto impl = new wlr_output_impl {
        QW_INIT_INTERFACE_FUNC(funMagicKey, set_cursor, &QWOutputInterface::setCursor),
        QW_INIT_INTERFACE_FUNC(funMagicKey, move_cursor, &QWOutputInterface::moveCursor),
        .destroy = impl::destroy,
        QW_INIT_INTERFACE_FUNC(funMagicKey, test, &QWOutputInterface::test),
        .commit = impl::commit,
        QW_INIT_INTERFACE_FUNC(funMagicKey, get_gamma_size, &QWOutputInterface::getGammaSize),
        QW_INIT_INTERFACE_FUNC(funMagicKey, get_cursor_formats, &QWOutputInterface::getCursorFormats),
        QW_INIT_INTERFACE_FUNC(funMagicKey, get_cursor_size, &QWOutputInterface::getCursorSize),
        QW_INIT_INTERFACE_FUNC(funMagicKey, get_primary_formats, &QWOutputInterface::getPrimaryFormats),
    };

    m_handleImpl = impl;
    m_handle = calloc(1, sizeof(_wlr_output));
    static_cast<_wlr_output *>(m_handle)->interface = this;
    wlr_output_init(handle(), backend->handle(), impl, display->handle());
}

QW_END_NAMESPACE
