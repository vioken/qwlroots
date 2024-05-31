// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <qwoutputinterface.h>

#include <QObject>
#include <type_traits>

struct wlr_output;
struct wlr_output_mode;
struct wlr_output_state;
struct wlr_drm_format_set;
struct wlr_output_cursor;
struct wlr_buffer;
struct wl_resource;
struct wlr_output_event_present;
struct wlr_output_event_damage;
struct wlr_output_event_bind;
struct wlr_output_event_precommit;
struct wlr_output_event_commit;
struct wlr_output_event_request_state;
struct wlr_output_state;
struct wlr_swapchain;
struct wlr_render_pass;
struct pixman_region32;
typedef pixman_region32 pixman_region32_t;

typedef uint32_t wl_output_transform_t;
typedef uint32_t wl_output_subpixel_t;

QW_BEGIN_NAMESPACE

class QWAllocator;
class QWRenderer;
class QWBuffer;
class QWOutputInterface;
class QWOutputPrivate;
class QW_EXPORT QWOutput : public QWWrapObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWOutput)
public:
    ~QWOutput() = default;

    inline wlr_output *handle() const {
        return QWObject::handle<wlr_output>();
    }

    static QWOutput *get(wlr_output *handle);
    static QWOutput *from(wlr_output *handle);
    static QWOutput *from(wl_resource *resource);
    template<class Interface, typename... Args>
    inline static typename std::enable_if<std::is_base_of<QWOutputInterface, Interface>::value, QWOutput*>::type
    create(Args&&... args) {
        Interface *i = new Interface();
        i->QWOutputInterface::template init<Interface>(std::forward<Args>(args)...);
        return new QWOutput(i->handle(), true);
    }

#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
    void enable(bool enable);
    void createGlobal();
#else
    void createGlobal(QWDisplay *display);
#endif
    void destroyGlobal();
    bool initRender(QWAllocator *allocator, QWRenderer *renderer);
    wlr_output_mode *preferredMode() const;

#if WLR_VERSION_MINOR < 18
    void setMode(wlr_output_mode *mode);
    void setCustomMode(const QSize &size, int32_t refresh);
    void setTransform(wl_output_transform_t wl_output_transform);
    void enableAdaptiveSync(bool enabled);
    void setRenderFormat(uint32_t format);
    void setScale(float scale);
    void setSubpixel(wl_output_subpixel_t wl_output_subpixel);
    void setDamage(pixman_region32 *damage);
#endif
    void setName(const QByteArray &name);
    void setDescription(const QByteArray &desc);
    void scheduleDone();

    QSize transformedResolution() const;
    QSize effectiveResolution() const;

#if WLR_VERSION_MINOR < 18
    bool attachRender(int *bufferAge);
    void attachBuffer(QWBuffer *buffer);
#endif
    void lockAttachRender(bool lock);

    uint32_t preferredReadFormat() const;
#if WLR_VERSION_MINOR < 18
    bool test();
    bool commit();
    void rollback();
#endif
    bool testState(wlr_output_state *state);
    bool commitState(wlr_output_state *state);
    static void finishState(wlr_output_state *state);
    void scheduleFrame();

    size_t getGammaSize() const;
    void setGamma(size_t size, const uint16_t *r, const uint16_t *g, const uint16_t *b);
    void lockSoftwareCursors(bool lock);
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
    void renderSoftwareCursors(pixman_region32 *damage);
#endif
    const wlr_drm_format_set *getPrimaryFormats(uint32_t bufferCaps);

    void addSoftwareCursorsToRenderPass(wlr_render_pass *render_pass, const pixman_region32_t *damage);
    bool configurePrimarySwapchain(const wlr_output_state *state, wlr_swapchain **swapchain);

Q_SIGNALS:
    void frame();
    void damage(wlr_output_event_damage *event);
    void needsFrame();
    void precommit(wlr_output_event_precommit *event);
    void commit(wlr_output_event_commit *event);
    void present(wlr_output_event_present *event);
    void bind(wlr_output_event_bind *event);
    void descriptionChanged();
    void requestState(wlr_output_event_request_state *state);

private:
    QWOutput(wlr_output *handle, bool isOwner);
};

class QW_EXPORT QWOutputCursor
{
public:
    QWOutputCursor() = delete;
    QW_DISALLOW_DESTRUCTOR(QWOutputCursor)

    void operator delete(QWOutputCursor *p, std::destroying_delete_t);
    wlr_output_cursor *handle() const;

    static QWOutputCursor *from(wlr_output_cursor *handle);
    static QWOutputCursor *create(QWOutput *output);

    bool setBuffer(QWBuffer *buffer, const QPoint &hotspot);
    bool move(const QPointF &pos);
};

QW_END_NAMESPACE
