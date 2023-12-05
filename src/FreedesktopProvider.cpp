//
// Created by kosmx on 12/4/23.
//

#include "FreedesktopProvider.h"
#include <QLatin1StringView>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusMetaType>
#include <QtGui/QTextListFormat>


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
        if (l.size() < 3) {
            return {};
        }
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
            auto group = settings.getSettings().value(u"general"_s);
            if (key == colorScheme) {
                QVariant setting = group.value(u"colorScheme"_s);
                if (setting.isNull()) {
                    auto val = settings.getSettings()[u"Colors:Window"_s][u"BackgroundNormal"_s];
                    if (!val.isNull()) {
                        auto array = ColorArray::ofString(val.toString());
                        auto gray = qGray(array.r, array.g, array.b);
                        setting = QVariant::fromValue(gray < 192 ? u"dark"_s : ""); // lower than 192 then prefer dark
                    }
                }
                return setting.isNull() ? 0 : setting.toString() == u"dark"_s ? 1 : 2;
            } else if (key == accentColor) {

                QString setting = group.value(u"general.accentColor"_s, "").toString();
                if (setting.isEmpty()) {
                    auto kgroup = settings.getSettings().value(u"Colors:Selection"_s);
                    setting = kgroup.value("BackgroundNormal", "").toString();
                }
                if (setting.isEmpty()) setting = u"0,114,255"_s;

                return ColorArray::ofString(setting);
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