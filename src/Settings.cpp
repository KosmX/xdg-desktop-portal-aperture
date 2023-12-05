//
// Created by kosmx on 12/3/23.
//

#include "Settings.h"
#include "FreedesktopProvider.h"
#include "KdeProvider.h"
#include "GnomeDarkModeProvider.h"

#include <QtDBus/QDBusContext>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>
#include <algorithm>
#include <syslog.h>
#include <QtDBus/QDBusMetaType>

namespace aperture {

    bool namespaceContains(const QStringList &namespaces, const QString &_namespace) {
        return std::any_of(namespaces.begin(), namespaces.end(), [&](const auto &item) {
            return item.isEmpty() ||
                   item.endsWith("*") && _namespace.startsWith(item.left(item.size() - 1)) ||
                   item.startsWith(_namespace.left(item.length())); // the spec isn't clear, soo maybe?
        });
    }


    Settings::Settings(DesktopPortal *parent) : QDBusAbstractAdaptor(parent), portal(parent) {

        providers.push_back(std::make_unique<FreedesktopProvider>(*this));
        providers.push_back(std::make_unique<KdeProvider>(*this));
        providers.push_back(std::make_unique<GnomeDarkModeProvider>(*this, dynamic_cast<FreedesktopProvider*>(providers[0].get())));

        qDBusRegisterMetaType<QMap<QString, QVariantMap>>();
    }

    void Settings::Read(const QString &_namespace, const QString &key) {
        syslog(LOG_DEBUG, "Read %s %s", _namespace.toStdString().c_str(), key.toStdString().c_str());

        bool messageSent = false;

        for (auto &provider: providers) {
            if (_namespace.startsWith(provider->getNamespace())) {

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

        auto reply = portal->message().createErrorReply(QDBusError::UnknownProperty,
                                                        QStringLiteral("Property not found"));
        QDBusConnection::sessionBus().send(reply);
    }

    void Settings::ReadAll(const QStringList &namespaces) {
        syslog(LOG_DEBUG, "ReadAll %s", namespaces.join(", ").toStdString().c_str());

        QMap<QString, QVariantMap> result;

        for (auto &provider: providers) {
            if (namespaceContains(namespaces, provider->getNamespace())) {
                result.insert(provider->readAll(namespaces));
            }
        }

        auto reply = portal->message().createReply(QVariant::fromValue(result));
        QDBusConnection::sessionBus().send(reply);
    }

    void Settings::emitSettingsChanged(const QString &_namespace, const QString &key, const QVariant &newValue) {
        syslog(LOG_DEBUG, "Settings changed %s %s", _namespace.toStdString().c_str(), key.toStdString().c_str());
        emit SettingsChanged(_namespace, key, QDBusVariant(newValue));
    }
} // aperture