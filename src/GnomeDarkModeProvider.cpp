//
// Created by kosmx on 12/5/23.
//

#include "GnomeDarkModeProvider.h"


using namespace Qt::Literals::StringLiterals;

namespace aperture {
    GnomeDarkModeProvider::GnomeDarkModeProvider(Settings &settings, FreedesktopProvider *freedesktopProvider): SettingsProvider(settings), freedesktopProvider(freedesktopProvider) {
        connect(freedesktopProvider, &FreedesktopProvider::settingsChanged, this, &GnomeDarkModeProvider::onSettingsChanged);
    }

    GnomeDarkModeProvider::~GnomeDarkModeProvider() {
        disconnect(freedesktopProvider, &FreedesktopProvider::settingsChanged, this, &GnomeDarkModeProvider::onSettingsChanged);
    }

    QVariant GnomeDarkModeProvider::read(const QString &_namespace, const QString &key) {
        if (_namespace == u"org.gnome.desktop.interface"_s && key == u"color-scheme") {
            return freedesktopProvider->read(freedesktopProvider->getNamespace(), key) == 1 ? u"prefer-dark"_s : u"prefer-light"_s;
        }
        return {};
    }

    QMap<QString, QVariantMap> GnomeDarkModeProvider::readAll(const QStringList &namespaces) {
        QConfig map{};

        QVariantMap gnomeInterface{};
        gnomeInterface[u"color-scheme"_s] = read(u"org.gnome.desktop.interface"_s, u"color-scheme"_s);
        map[u"org.gnome.desktop.interface"_s] = gnomeInterface;

        return map;
    }

    void GnomeDarkModeProvider::onSettingsChanged() {
        settings.emitSettingsChanged(u"org.gnome.desktop.interface"_s, u"color-scheme"_s, read(u"org.gnome.desktop.interface"_s, u"color-scheme"_s));
    }

    QString GnomeDarkModeProvider::getNamespace() {
        return u"org.gnome.desktop.interface"_s;
    }
} // aperture