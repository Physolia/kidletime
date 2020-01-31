/* This file is part of the KDE libraries
 * SPDX-FileCopyrightText: 2009 Dario Freddi <drf at kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-only
 */

#include "xscreensaverbasedpoller.h"

#include <config-kidletime.h>

#include <QX11Info>

#include <X11/Xlib.h>
#include <X11/extensions/scrnsaver.h>

XScreensaverBasedPoller::XScreensaverBasedPoller(QObject *parent)
    : WidgetBasedPoller(parent), m_screenSaverIface(nullptr)
{

}

XScreensaverBasedPoller::~XScreensaverBasedPoller()
{
}

bool XScreensaverBasedPoller::additionalSetUp()
{
    m_screenSaverIface = new OrgFreedesktopScreenSaverInterface(QLatin1String("org.freedesktop.ScreenSaver"),
            QLatin1String("/ScreenSaver"),
            QDBusConnection::sessionBus(), this);

    connect(m_screenSaverIface, SIGNAL(ActiveChanged(bool)), SLOT(screensaverActivated(bool)));

    return true;
}

void XScreensaverBasedPoller::screensaverActivated(bool activated)
{
    // We care only if it has been disactivated

    if (!activated) {
        m_screenSaverIface->SimulateUserActivity();
        emit resumingFromIdle();
    }
}

int XScreensaverBasedPoller::getIdleTime()
{
    XScreenSaverInfo *mitInfo = nullptr;
    mitInfo = XScreenSaverAllocInfo();
    XScreenSaverQueryInfo(QX11Info::display(), DefaultRootWindow(QX11Info::display()), mitInfo);
    int ret = mitInfo->idle;
    XFree(mitInfo);
    return ret;
}

void XScreensaverBasedPoller::simulateUserActivity()
{
    stopCatchingIdleEvents();
    XResetScreenSaver(QX11Info::display());
    XFlush(QX11Info::display());
    emit resumingFromIdle();
}

