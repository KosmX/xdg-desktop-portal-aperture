#include <QCoreApplication>
#include <QtDBus/QDBusConnection>
#include <syslog.h>
#include <unistd.h>
#include <QDBusInterface>
#include "DesktopPortal.h"
#include "Settings.h"

int main(int argc, char *argv[])
{

    //*   // commented out for debugging :)
    if (daemon(false, false) != 0) {
        perror("daemon");
        exit(EXIT_FAILURE);
    }//*/

    qputenv("QT_LOGGING_TO_CONSOLE", QByteArray("0"));

    openlog("xdg-desktop-portal-aperture", LOG_PID, LOG_DAEMON);

    syslog(LOG_DEBUG, "Starting daemon");

    QCoreApplication a(argc, argv);

    QCoreApplication::setOrganizationDomain("xdg-desktop-portal-aperture");
    QCoreApplication::setApplicationName("aperture");


    syslog(LOG_INFO, "Entering event loop");

    auto connection = QDBusConnection::sessionBus();
    if (!connection.isConnected()) {
        syslog(LOG_ERR, "Can't connect to session bus");
        return EXIT_FAILURE;
    }

    aperture::DesktopPortal portal{};
    aperture::Settings settings{&portal};


    connection.registerObject("/org/freedesktop/portal/desktop", &portal);
    if(!connection.registerService("org.freedesktop.impl.portal.desktop.aperture")) {
        syslog(LOG_ERR, "Can't register service");
        return EXIT_FAILURE;
    }

    int r = a.exec();

    syslog(LOG_INFO, "Stoppping daemon");
    closelog();
    return r;
}
