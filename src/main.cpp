#include <QCoreApplication>
#include <QtDBus/QDBusConnection>
#include <syslog.h>
#include <unistd.h>
#include <QDBusInterface>
#include "DesktopPortal.h"
#include "Settings.h"

// Handler for Qt log messages that sends output to syslog as well as standard error.
void SyslogMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    Q_UNUSED(context)

    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
        case QtDebugMsg:
            syslog(LOG_DEBUG, "%s", localMsg.constData());
            break;
        case QtInfoMsg:
            syslog(LOG_INFO, "%s", localMsg.constData());
            break;
        case QtWarningMsg:
            syslog(LOG_WARNING, "%s", localMsg.constData());
            break;
        case QtCriticalMsg:
            syslog(LOG_CRIT, "%s", localMsg.constData());
            break;
        case QtFatalMsg:
            syslog(LOG_ALERT, "%s", localMsg.constData());
            break;
    }
}

int main(int argc, char *argv[])
{

    //*   // commented out for debugging :)
    if (daemon(false, false) != 0) {
        perror("daemon");
        exit(EXIT_FAILURE);
    }//*/


    openlog("xdg-desktop-portal-aperture", LOG_PID, LOG_DAEMON);

    qInstallMessageHandler(SyslogMessageHandler);
    syslog(LOG_DEBUG, "Starting daemon");

    QCoreApplication a(argc, argv);

    QCoreApplication::setOrganizationDomain("xdg-desktop-portal-aperture");
    QCoreApplication::setApplicationName("aperture");


    auto connection = QDBusConnection::sessionBus();
    if (!connection.isConnected()) {
        syslog(LOG_ERR, "Can't connect to session bus");
        closelog();
        return EXIT_FAILURE;
    }

    syslog(LOG_DEBUG, "Session bus connection OK");

    aperture::DesktopPortal portal{};
    aperture::Settings settings{&portal};

    syslog(LOG_DEBUG, "Config loaded, registering DBus service");

    connection.registerObject("/org/freedesktop/portal/desktop", &portal);
    if(!connection.registerService("org.freedesktop.impl.portal.desktop.aperture")) {
        syslog(LOG_ERR, "Can't register service");
        closelog();
        return EXIT_FAILURE;
    }

    syslog(LOG_INFO, "Entering event loop");

    int r = a.exec();

    syslog(LOG_INFO, "Stoppping daemon");
    closelog();
    return r;
}
