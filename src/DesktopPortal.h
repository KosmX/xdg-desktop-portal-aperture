//
// Created by kosmx on 12/3/23.
//

#ifndef XDG_DESKTOP_PORTAL_APERTURE_DESKTOPPORTAL_H
#define XDG_DESKTOP_PORTAL_APERTURE_DESKTOPPORTAL_H
#ifdef __cplusplus

#include <QObject>
#include <QtDBus/QDBusContext>
#include <QSettings>
#include <memory>

using QConfig = QMap<QString, QVariantMap>;

namespace aperture {

    class DesktopPortal : public QObject , public QDBusContext {
        Q_OBJECT
        Q_DISABLE_COPY_MOVE(DesktopPortal);

    private:
        std::unique_ptr<QSettings> settingsFile = std::make_unique<QSettings>();
        std::unique_ptr<QConfig> config;
    public:
        DesktopPortal();
        const std::unique_ptr<QConfig> &getSettings();

        static std::unique_ptr<QConfig> qSettingsToConfig(QSettings& settings);

    //public slots:

    signals:
        void settingsChanged(const std::unique_ptr<QSettings>& oldSettings, const std::unique_ptr<QSettings>& newSettings);

    };

}


#endif //__cplusplus
#endif //XDG_DESKTOP_PORTAL_APERTURE_DESKTOPPORTAL_H
