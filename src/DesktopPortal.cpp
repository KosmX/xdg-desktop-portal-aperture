//
// Created by kosmx on 12/3/23.
//

#include "DesktopPortal.h"
#include "Settings.h"

aperture::DesktopPortal::DesktopPortal() {
    
    // copy the config file
    for (auto& key : this->settingsFile->allKeys()) {
        (*config)[key] = settingsFile->value(key);
    }

    // Copying the file is necessary for making diffs
};



const std::unique_ptr<QMap<QString, QVariant>> &aperture::DesktopPortal::getSettings() {
    return config;
}
