//
// Created by kosmx on 12/5/23.
//

#include "KdeProvider.h"
#include <syslog.h>

using namespace Qt::Literals::StringLiterals;

namespace aperture {
    QString KdeProvider::getNamespace() {
        return u"org.kde.kdeglobals"_s;
    }

    QMap<QString, QVariantMap> KdeProvider::readAll(const QStringList &namespaces) {
        QConfig result;

        for (auto entry: settings.getSettings().asKeyValueRange()) {
            auto ns = getNamespace() + u"."_s + entry.first;
            if (namespaceContains(namespaces, ns)) {
                QVariantMap map;
                for (auto &key: entry.second.keys()) {
                    map[key] = entry.second.value(key).toString();
                }
                result[ns] = map;
            }
        }

        return result;
    }

    QVariant KdeProvider::read(const QString &_namespace, const QString &key) {
        auto ns = _namespace.right(_namespace.size() - getNamespace().size() - 1);
        syslog(LOG_DEBUG, "Read KDE global setting %s", ns.toStdString().c_str());
        return settings.getSettings()[ns][key]; // when you can do this, then you're using something good (except for nullability, that would be a problem if QVariant couldn't be null)
    }

    KdeProvider::KdeProvider(Settings &parent) : SettingsProvider(parent) {
        connect(settings.getPortal(), &DesktopPortal::settingChanged, this, &KdeProvider::onSettingChanged);
    }

    KdeProvider::~KdeProvider() {
        disconnect(settings.getPortal(), &DesktopPortal::settingChanged, this, &KdeProvider::onSettingChanged);
    }

    void KdeProvider::onSettingChanged(const QString &ns, const QString &key, const QVariant &oldValue,
                                       const QVariant &newValue) {

        auto _namespace = getNamespace() + u"."_s + ns;
        settings.emitSettingsChanged(_namespace, key, newValue);
    }
} // aperture