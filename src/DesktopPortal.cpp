//
// Created by kosmx on 12/3/23.
//

#include "DesktopPortal.h"
#include "Settings.h"

aperture::DesktopPortal::DesktopPortal() = default;



const std::unique_ptr<QSettings> &aperture::DesktopPortal::getSettings() {
    return settingsFile;
}
