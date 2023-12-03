//
// Created by kosmx on 12/3/23.
//

#include "Settings.h"

#include <QtDBus/QDBusContext>
#include <QtDBus/QDBusMessage>
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
                    //auto reply = portal->message().createReply(QVariant::fromValue(QDBusVariant(value)));
                }
            }
        }
    }
} // aperture