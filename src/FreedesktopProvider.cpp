//
// Created by kosmx on 12/4/23.
//

#include "FreedesktopProvider.h"
#include <QLatin1StringView>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusMetaType>


using namespace Qt::Literals::StringLiterals;

struct ColorArray {
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    operator QVariant() const {
        return QVariant::fromValue(*this);
    }

    static ColorArray ofString(const QString& str) {
        auto l = str.split(",");
        return {l[0].trimmed().toDouble(), l[1].trimmed().toDouble(), l[2].trimmed().toDouble()};
    }
};

Q_DECLARE_METATYPE(ColorArray);

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

namespace aperture {


    static constexpr auto colorScheme = "color-scheme"_L1;
    static constexpr auto accentColor = "accent-color"_L1;

    QString FreedesktopProvider::getNamespace() {
        return u"org.freedesktop.appearance"_s;
    }

    QVariant FreedesktopProvider::read(const QString &_namespace, const QString &key) {
        if (_namespace == getNamespace()) {
            if (key == colorScheme) {
                return settings.getSettings().value(u"general.colorScheme"_s, "true").toBool() ? 1 : 2;
            } else if (key == accentColor) {
                return ColorArray::ofString(settings.getSettings().value(u"general.accentColor"_s, "1,1,1").toString());
            }
        }

        return {};
    }

    QMap<QString, QVariantMap> FreedesktopProvider::readAll(const QStringList &namespaces) {
        auto map = QMap<QString, QVariantMap>();

        map.insert(getNamespace(), {
            {colorScheme, read(getNamespace(), colorScheme)},
            {accentColor, read(getNamespace(), accentColor)}
        });

        return map;
    }

    FreedesktopProvider::FreedesktopProvider(Settings &settings) : SettingsProvider(settings) {
        qDBusRegisterMetaType<ColorArray>();
    }
} // aperture