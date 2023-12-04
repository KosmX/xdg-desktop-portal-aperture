//
// Created by kosmx on 12/4/23.
//

#include "FreedesktopProvider.h"
#include <QLatin1StringView>


using namespace Qt::Literals::StringLiterals;

namespace aperture {


    static constexpr auto colorScheme = "color-scheme"_L1;
    static constexpr auto accentColor = "accent-color"_L1;

    QString FreedesktopProvider::getNamespace() {
        return u"org.freedesktop.appearance"_s;
    }

    QVariant FreedesktopProvider::read(const QString &_namespace, const QString &key) {
        if (_namespace == getNamespace()) {
            if (key == colorScheme) {
                return // TODO: have settings file
            }
        }
        return QVariant();
    }
} // aperture