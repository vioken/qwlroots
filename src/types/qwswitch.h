// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWSWITCH_H
#define QWSWITCH_H

#include <qwinputdevice.h>
#include <qwswitchinterface.h>

struct wlr_switch;
struct wlr_input_device;

QW_BEGIN_NAMESPACE

class QWSwitchPrivate;
class QW_EXPORT QWSwitch : public QWInputDevice
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWSwitch)
public:
    inline wlr_switch *handle() const {
        return QWObject::handle<wlr_switch>();
    }

    static QWSwitch *get(wlr_switch *handle);
    static QWSwitch *from(wlr_switch *handle);
    static QWSwitch *fromInputDevice(wlr_input_device *inputDevice);

    template<class Interface, typename... Args>
    inline static typename std::enable_if<std::is_base_of<QWSwitchInterface, Interface>::value, QWSwitch*>::type
    create(Args&&... args) {
        Interface *i = new Interface();
        i->QWSwitchInterface::template init<Interface>(std::forward<Args>(args)...);
        return new QWSwitch(i->handle(), true);
    }

Q_SIGNALS:
    void toggle();

private:
    QWSwitch(wlr_switch *handle, bool isOwner = false);
    ~QWSwitch() override = default;
};

QW_END_NAMESPACE

#endif // QWSWITCH_H
