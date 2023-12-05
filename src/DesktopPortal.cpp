//
// Created by kosmx on 12/3/23.
//

#include "DesktopPortal.h"
#include "Settings.h"
#include <syslog.h>

#include <QLatin1StringView>

using namespace Qt::Literals::StringLiterals;

aperture::DesktopPortal::DesktopPortal(): config(qSettingsToConfig(*settingsFile)) {
    
    // copy the config file
    // Copying the file is necessary for making diffs

    watcher->addPath(settingsFile->fileName());

    connect(watcher.get(), &QFileSystemWatcher::fileChanged, this, &DesktopPortal::onConfigChanged);
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
        }

        config->insert(group, ns);
        settings.endGroup();
    }

    return config;
}

void aperture::DesktopPortal::onConfigChanged(const QString &path) {
    syslog(LOG_INFO, "Config file changed, reloading and sending signals");
    auto oldSettings = std::move(config);
    settingsFile->sync();
    config = qSettingsToConfig(*settingsFile);

    emit settingsChanged(oldSettings, config);


    // Diffing config and find changes
    for (auto& newNamespace : config->keys()) {
        auto oldGroup = oldSettings->value(newNamespace);
        auto newGroup = config->value(newNamespace);
        for (auto& key : newGroup.keys()) {
            auto oldValue = oldGroup.value(key);
            auto newValue = newGroup.value(key);
            if (oldValue != newValue) {
                emit settingChanged(newNamespace, key, oldValue, newValue);
            }
        }
        for (auto& key : oldGroup.keys()) {
            if (!newGroup.contains(key)) {
                emit settingChanged(newNamespace, key, oldGroup.value(key), QVariant{});
            }
        }
    }
    for (auto& oldNamespace : oldSettings->keys()) {
        if (!config->contains(oldNamespace)) {
            auto oldGroup = oldSettings->value(oldNamespace);
            for (auto& key : oldGroup.keys()) {
                emit settingChanged(oldNamespace, key, oldGroup.value(key), QVariant{});
            }
        }
    }

}

aperture::DesktopPortal::~DesktopPortal() {
    disconnect(watcher.get(), &QFileSystemWatcher::fileChanged, this, &DesktopPortal::onConfigChanged); // clean-up
}
