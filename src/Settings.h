//
// Created by kosmx on 12/3/23.
//

#ifndef XDG_DESKTOP_PORTAL_APERTURE_SETTINGS_H
#define XDG_DESKTOP_PORTAL_APERTURE_SETTINGS_H
#ifdef __cplusplus

#include <QObject>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QSettings>
#include <QString>
#include <QtDBus/QDBusVariant>

#include "DesktopPortal.h"

namespace aperture {

    class SettingsProvider : public QObject {
        Q_OBJECT
    public:
        Q_DISABLE_COPY_MOVE(SettingsProvider)
        ~SettingsProvider() override = default;

        virtual QString getNamespace() = 0;
        virtual QVariant read(const QString& _namespace, const QString& key) = 0;
        virtual QMap<QString, QVariantMap> readAll(const QStringList& namespaces) = 0;
    };

    class Settings : public QDBusAbstractAdaptor {
        Q_OBJECT
        Q_CLASSINFO("D-Bus Interface", "org.freedesktop.impl.portal.Settings")
        Q_PROPERTY(uint version READ version CONSTANT)
        Q_DISABLE_COPY_MOVE(Settings);

    public:
        Settings(DesktopPortal* parent, std::unique_ptr<QSettings>&& settings);
        ~Settings() override = default;

        [[nodiscard]] uint version() const {
            return 2;
        }

        [[nodiscard]] const std::unique_ptr<QSettings>& getSettings() const {
            return portal->getSettings();
        }

        [[nodiscard]] const DesktopPortal* getPortal() const {
            return portal;
        }

    public slots:
        void ReadAll(const QStringList& namespaces);

        void Read(const QString& _namespace, const QString& key);

    signals:
        // Before invoking this, settings should be already updated.
        void SettingsChanged(const QString& _namespace, const QString& key, const QDBusVariant& newValue);

    private:
        DesktopPortal* portal;
        std::vector<std::unique_ptr<SettingsProvider>> providers;

    };

} // aperture

#endif //__cplusplus
#endif //XDG_DESKTOP_PORTAL_APERTURE_SETTINGS_H
