// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwoutput.h"
#include "qwbuffer.h"
#include "util/qwsignalconnector.h"
#include "render/qwallocator.h"
#include "render/qwrenderer.h"

#include <QImage>
#include <QSize>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_output.h>
#undef static
}

static_assert(std::is_same_v<wl_output_transform_t, std::underlying_type_t<wl_output_transform>>);
static_assert(std::is_same_v<wl_output_subpixel_t, std::underlying_type_t<wl_output_subpixel>>);

QW_BEGIN_NAMESPACE

class QWOutputPrivate : public QWObjectPrivate
{
public:
    QWOutputPrivate(wlr_output *handle, bool isOwner, QWOutput *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.frame, this, &QWOutputPrivate::on_frame);
        sc.connect(&handle->events.damage, this, &QWOutputPrivate::on_damage);
        sc.connect(&handle->events.needs_frame, this, &QWOutputPrivate::on_needs_frame);
        sc.connect(&handle->events.precommit, this, &QWOutputPrivate::on_precommit);
        sc.connect(&handle->events.commit, this, &QWOutputPrivate::on_commit);
        sc.connect(&handle->events.present, this, &QWOutputPrivate::on_present);
        sc.connect(&handle->events.bind, this, &QWOutputPrivate::on_bind);
        sc.connect(&handle->events.description, this, &QWOutputPrivate::on_description);
        sc.connect(&handle->events.destroy, this, &QWOutputPrivate::on_destroy);
    }
    ~QWOutputPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_output_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_frame(void *data);
    void on_damage(void *data);
    void on_needs_frame(void *data);
    void on_precommit(void *data);
    void on_commit(void *data);
    void on_present(void *data);
    void on_bind(void *data);
    void on_description(void *data);
    void on_destroy(void *data);

    static QHash<void*, QWOutput*> map;
    QW_DECLARE_PUBLIC(QWOutput)
    QWSignalConnector sc;
};
QHash<void*, QWOutput*> QWOutputPrivate::map;

void QWOutputPrivate::on_frame(void *)
{
    Q_EMIT q_func()->frame();
}

void QWOutputPrivate::on_damage(void *data)
{
    Q_EMIT q_func()->damage(reinterpret_cast<wlr_output_event_damage*>(data));
}

void QWOutputPrivate::on_needs_frame(void *)
{
    Q_EMIT q_func()->needsFrame();
}

void QWOutputPrivate::on_precommit(void *data)
{
    Q_EMIT q_func()->precommit(reinterpret_cast<wlr_output_event_precommit*>(data));
}

void QWOutputPrivate::on_commit(void *data)
{
    Q_EMIT q_func()->commit(reinterpret_cast<wlr_output_event_commit*>(data));
}

void QWOutputPrivate::on_present(void *data)
{
    Q_EMIT q_func()->present(reinterpret_cast<wlr_output_event_present*>(data));
}

void QWOutputPrivate::on_bind(void *data)
{
    Q_EMIT q_func()->bind(reinterpret_cast<wlr_output_event_bind*>(data));
}

void QWOutputPrivate::on_description(void *)
{
    Q_EMIT q_func()->descriptionChanged();
}

void QWOutputPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWOutput::QWOutput(wlr_output *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWOutputPrivate(handle, isOwner, this))
{

}

QWOutput *QWOutput::get(wlr_output *handle)
{
    return QWOutputPrivate::map.value(handle);
}

QWOutput *QWOutput::from(wlr_output *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWOutput(handle, false);
}

QWOutput *QWOutput::from(wl_resource *resource)
{
    auto handle = wlr_output_from_resource(resource);
    if (!handle)
        return nullptr;
    return from(handle);
}

void QWOutput::enable(bool enable)
{
    wlr_output_enable(handle(), enable);
}

void QWOutput::createGlobal()
{
    wlr_output_create_global(handle());
}

void QWOutput::destroyGlobal()
{
    wlr_output_destroy_global(handle());
}

bool QWOutput::initRender(QWAllocator *allocator, QWRenderer *renderer)
{
    return wlr_output_init_render(handle(), allocator->handle(), renderer->handle());
}

wlr_output_mode *QWOutput::preferredMode() const
{
    return wlr_output_preferred_mode(handle());
}

void QWOutput::setMode(wlr_output_mode *mode)
{
    wlr_output_set_mode(handle(), mode);
}

void QWOutput::setCustomMode(const QSize &size, int32_t refresh)
{
    wlr_output_set_custom_mode(handle(), size.width(), size.height(), refresh);
}

void QWOutput::setTransform(wl_output_transform_t transform)
{
    wlr_output_set_transform(handle(), static_cast<wl_output_transform>(transform));
}

void QWOutput::enableAdaptiveSync(bool enabled)
{
    wlr_output_enable_adaptive_sync(handle(), enabled);
}

void QWOutput::setRenderFormat(uint32_t format)
{
    wlr_output_set_render_format(handle(), format);
}

void QWOutput::setScale(float scale)
{
    wlr_output_set_scale(handle(), scale);
}

void QWOutput::setSubpixel(wl_output_subpixel_t subpixel)
{
    wlr_output_set_subpixel(handle(), static_cast<wl_output_subpixel>(subpixel));
}

void QWOutput::setName(const QByteArray &name)
{
    wlr_output_set_name(handle(), name.constData());
}

void QWOutput::setDescription(const QByteArray &desc)
{
    wlr_output_set_description(handle(), desc.constData());
}

void QWOutput::scheduleDone()
{
    wlr_output_schedule_done(handle());
}

QSize QWOutput::transformedResolution() const
{
    QSize size;
    wlr_output_transformed_resolution(handle(), &size.rwidth(), &size.rheight());
    return size;
}

QSize QWOutput::effectiveResolution() const
{
    QSize size;
    wlr_output_effective_resolution(handle(), &size.rwidth(), &size.rheight());
    return size;
}

bool QWOutput::attachRender(int *bufferAge)
{
    return wlr_output_attach_render(handle(), bufferAge);
}

void QWOutput::lockAttachRender(bool lock)
{
    wlr_output_lock_attach_render(handle(), lock);
}

void QWOutput::attachBuffer(QWBuffer *buffer)
{
    wlr_output_attach_buffer(handle(), buffer->handle());
}

uint32_t QWOutput::preferredReadFormat() const
{
    return wlr_output_preferred_read_format(handle());
}

void QWOutput::setDamage(pixman_region32 *damage)
{
    wlr_output_set_damage(handle(), damage);
}

bool QWOutput::test()
{
    return wlr_output_test(handle());
}

bool QWOutput::commit()
{
    return wlr_output_commit(handle());
}

void QWOutput::rollback()
{
    wlr_output_rollback(handle());
}

bool QWOutput::testState(wlr_output_state *state)
{
    return wlr_output_test_state(handle(), state);
}

bool QWOutput::commitState(wlr_output_state *state)
{
    return wlr_output_commit_state(handle(), state);
}

void QWOutput::scheduleFrame()
{
    wlr_output_schedule_frame(handle());
}

size_t QWOutput::getGammaSize() const
{
    return wlr_output_get_gamma_size(handle());
}

void QWOutput::setGamma(size_t size, const uint16_t *r, const uint16_t *g, const uint16_t *b)
{
    wlr_output_set_gamma(handle(), size, r, g, b);
}

void QWOutput::lockSoftwareCursors(bool lock)
{
    wlr_output_lock_software_cursors(handle(), lock);
}

void QWOutput::renderSoftwareCursors(pixman_region32 *damage)
{
    wlr_output_render_software_cursors(handle(), damage);
}

const wlr_drm_format_set *QWOutput::getPrimaryFormats(uint32_t bufferCaps)
{
    return wlr_output_get_primary_formats(handle(), bufferCaps);
}

#if WLR_VERSION_MINOR > 16
void QWOutput::addSoftwareCursorsToRenderPass(wlr_render_pass *render_pass, const pixman_region32_t *damage)
{
    wlr_output_add_software_cursors_to_render_pass(handle(), render_pass, damage);
}

bool QWOutput::configurePrimarySwapchain(const wlr_output_state *state, wlr_swapchain **swapchain)
{
    return wlr_output_configure_primary_swapchain(handle(), state, swapchain);
}
#endif

void QWOutputCursor::operator delete(QWOutputCursor *p, std::destroying_delete_t)
{
    wlr_output_cursor_destroy(p->handle());
}

wlr_output_cursor *QWOutputCursor::handle() const
{
    return reinterpret_cast<wlr_output_cursor*>(const_cast<QWOutputCursor*>(this));
}

QWOutputCursor *QWOutputCursor::from(wlr_output_cursor *handle)
{
    return reinterpret_cast<QWOutputCursor*>(handle);
}

QWOutputCursor *QWOutputCursor::create(QWOutput *output)
{
    auto handle = wlr_output_cursor_create(output->handle());
    return from(handle);
}

#if WLR_VERSION_MINOR <= 16
bool QWOutputCursor::setImage(const QImage &image, const QPoint &hotspot)
{
    return wlr_output_cursor_set_image(handle(), reinterpret_cast<const uint8_t*>(image.constBits()),
                                       image.bitPlaneCount(), image.width(), image.height(),
                                       hotspot.x(), hotspot.y());
}
#endif

bool QWOutputCursor::setBuffer(QWBuffer *buffer, const QPoint &hotspot)
{
    return wlr_output_cursor_set_buffer(handle(), buffer->handle(), hotspot.x(), hotspot.y());
}

bool QWOutputCursor::move(const QPointF &pos)
{
    return wlr_output_cursor_move(handle(), pos.x(), pos.y());
}

QW_END_NAMESPACE
