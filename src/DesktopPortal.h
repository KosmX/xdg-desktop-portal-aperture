//
// Created by kosmx on 12/3/23.
//

#ifndef XDG_DESKTOP_PORTAL_APERTURE_DESKTOPPORTAL_H
#define XDG_DESKTOP_PORTAL_APERTURE_DESKTOPPORTAL_H
#ifdef __cplusplus

#include <QObject>
#include <QtDBus/QDBusContext>

namespace aperture {

    class Settings;

    class DesktopPortal : public QObject , public QDBusContext {
        Q_OBJECT
        Q_DISABLE_COPY_MOVE(DesktopPortal)
    private:
        Settings* settings = nullptr;
    public:
        DesktopPortal();

    signals:
        void settingsChanged();

    };

}


#endif //__cplusplus
#endif //XDG_DESKTOP_PORTAL_APERTURE_DESKTOPPORTAL_H
