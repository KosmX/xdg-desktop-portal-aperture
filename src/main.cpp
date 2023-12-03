#include <QCoreApplication>
#include <syslog.h>
#include <unistd.h>
#include "DesktopPortal.h"

int main(int argc, char *argv[])
{

    if (daemon(false, false) != 0) {
        perror("daemon");
        exit(EXIT_FAILURE);
    }
    openlog("xdg-desktop-portal-aperture", LOG_PID, LOG_DAEMON);

    syslog(LOG_DEBUG, "Starting daemon");



    QCoreApplication a(argc, argv);


    syslog(LOG_INFO, "Entering event loop");

    aperture::DesktopPortal portal{};

    int r = a.exec();
    
    closelog();
    return r;
}
