//
// Created by kosmx on 12/3/23.
//

#ifndef XDG_DESKTOP_PORTAL_APERTURE_DESKTOPPORTAL_H
#define XDG_DESKTOP_PORTAL_APERTURE_DESKTOPPORTAL_H
#ifdef __cplusplus

#include <QObject>
#include <QtDBus/QDBusContext>
#include <QSettings>
#include <QFileSystemWatcher>
#include <memory>

using QConfig = QMap<QString, QVariantMap>;

namespace aperture {

    class DesktopPortal : public QObject , public QDBusContext {
        Q_OBJECT
        Q_DISABLE_COPY_MOVE(DesktopPortal);

    private:
        std::unique_ptr<QSettings> settingsFile = std::make_unique<QSettings>();
        std::unique_ptr<QConfig> config;

        std::unique_ptr<QFileSystemWatcher> watcher = std::make_unique<QFileSystemWatcher>(this);
    public:
        DesktopPortal();
        ~DesktopPortal() override;
        const std::unique_ptr<QConfig> &getSettings();

        static std::unique_ptr<QConfig> qSettingsToConfig(QSettings& settings);

    //public slots:

    private slots:
        void onConfigChanged(const QString& path);

    signals:
        void settingsChanged(const std::unique_ptr<QConfig>& oldSettings, const std::unique_ptr<QConfig>& newSettings);

        void settingChanged(const QString& ns, const QString& key, const QVariant& oldValue, const QVariant& newValue);

    };

}


#endif //__cplusplus
#endif //XDG_DESKTOP_PORTAL_APERTURE_DESKTOPPORTAL_H
