//
// Created by kosmx on 12/4/23.
//

#include "FreedesktopProvider.h"
#include <QLatin1StringView>
#include <QtDBus/QDBusArgument>


using namespace Qt::Literals::StringLiterals;

namespace aperture {


    struct ColorArray {
        double r = 0.0;
        double g = 0.0;
        double b = 0.0;

        operator QVariant() const {
            return QVariant::fromValue(*this);
        }

        static ColorArray ofString(const QString& str) {
            auto l = str.split(",");
            return {l[0].toDouble(), l[1].toDouble(), l[2].toDouble()};
        }
    };

    QDBusArgument& operator<<(QDBusArgument& dbus, const ColorArray& colorArray) {
        dbus.beginStructure();
        dbus << colorArray.r << colorArray.g << colorArray.b;
        dbus.endStructure();
        return dbus;
    }

    const QDBusArgument& operator>>(const QDBusArgument& dbus, ColorArray& array) {
        dbus.beginStructure();
        dbus >> array.r >> array.g >> array.b;
        dbus.endStructure();
        return dbus;
    }

    static constexpr auto colorScheme = "color-scheme"_L1;
    static constexpr auto accentColor = "accent-color"_L1;

    QString FreedesktopProvider::getNamespace() {
        return u"org.freedesktop.appearance"_s;
    }

    QVariant FreedesktopProvider::read(const QString &_namespace, const QString &key) {
        if (_namespace == getNamespace()) {
            if (key == colorScheme) {
                return settings.getSettings().value(u"general.colorScheme"_s, ""), )
            }
        }
        return QVariant();
    }
} // aperture