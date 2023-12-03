//
// Created by kosmx on 12/3/23.
//

#include "Settings.h"

#include <QtDBus/QDBusContext>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>
#include <algorithm>
#include <syslog.h>

namespace aperture {



    Settings::Settings(DesktopPortal *parent, std::unique_ptr<QSettings> &&settings): QDBusAbstractAdaptor(parent), settings(std::move(settings)), portal(parent) {

    }

    void Settings::Read(const QString &_namespace, const QString &key) {
        syslog(LOG_DEBUG, "Read %s %s", _namespace.toStdString().c_str(), key.toStdString().c_str());

        bool messageSent = false;

        for(auto& provider : providers) {
            if (provider->group().startsWith(_namespace)) {

                auto value = provider->read(_namespace, key);
                if (!value.isNull()) {
                    auto reply = portal->message().createReply(QVariant::fromValue(QDBusVariant(value)));
                    QDBusConnection::sessionBus().send(reply);
                    return;
                }
            }
        }
        // If we got here, we don't have the requested property.

        syslog(LOG_DEBUG, "Read property missing %s %s", _namespace.toStdString().c_str(), key.toStdString().c_str());

        auto reply = portal->message().createErrorReply(QDBusError::UnknownProperty, QStringLiteral("Property not found"));
        QDBusConnection::sessionBus().send(reply);
    }
} // aperture