//
// Created by kosmx on 12/3/23.
//

#include "DesktopPortal.h"
#include "Settings.h"

#include <QLatin1StringView>

using namespace Qt::Literals::StringLiterals;

aperture::DesktopPortal::DesktopPortal(): config(qSettingsToConfig(*settingsFile)) {
    
    // copy the config file
    // Copying the file is necessary for making diffs
};



const std::unique_ptr<QConfig> &aperture::DesktopPortal::getSettings() {
    return config;
}

std::unique_ptr<QConfig> aperture::DesktopPortal::qSettingsToConfig(QSettings &settings) {

    auto config = std::make_unique<QConfig>();

    QMap<QString, QVariant> general;
    for (auto& key : settings.childKeys()) {
        //syslog(LOG_DEBUG, "CONFIG key %s to %s", key.toStdString().c_str(), settingsFile->value(key).toString().toStdString().c_str());
        general[key] = settings.value(key).toStringList().join(',');
    }

    config->insert(u"general"_s, general);
    for (auto& group : settings.childGroups()) {
        settings.beginGroup(group);
        QMap<QString, QVariant> ns;
        for (auto& subkey : settings.childKeys()) {
            ns[subkey] = settings.value(subkey).toStringList().join(',');
            printf("%s\n", settings.value(subkey).toStringList().join(',').toStdString().c_str());
        }

        config->insert(group, ns);
        settings.endGroup();
    }

    return config;
}
