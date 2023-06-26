// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdatadevice.h"

extern "C" {
#include <wlr/types/wlr_data_device.h>
}

static_assert(std::is_same_v<wl_data_device_manager_dnd_action_t, std::underlying_type_t<wl_data_device_manager_dnd_action>>);

QW_BEGIN_NAMESPACE

void QWDataSource::operator delete(QWDataSource *p, std::destroying_delete_t)
{
    wlr_data_source_destroy(p->handle());
}

wlr_data_source *QWDataSource::handle() const
{
    return reinterpret_cast<wlr_data_source*>(const_cast<QWDataSource*>(this));
}

QWDataSource *QWDataSource::from(wlr_data_source *handle)
{
    return reinterpret_cast<QWDataSource*>(handle);
}

void QWDataSource::accept(uint32_t serial, const char *mime_type)
{
    wlr_data_source_accept(handle(), serial, mime_type);
}

void QWDataSource::dndAction(wl_data_device_manager_dnd_action_t action)
{
    wlr_data_source_dnd_action(handle(), static_cast<wl_data_device_manager_dnd_action>(action));
}

void QWDataSource::dndDrop()
{
    wlr_data_source_dnd_drop(handle());
}

void QWDataSource::dndFinish()
{
    wlr_data_source_dnd_finish(handle());
}

void QWDataSource::send(const char *mime_type, int32_t fd)
{
    wlr_data_source_send(handle(), mime_type, fd);
}

QW_END_NAMESPACE
