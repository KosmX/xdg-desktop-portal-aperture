//
// Created by kosmx on 12/4/23.
//

#ifndef XDG_DESKTOP_PORTAL_APERTURE_FREEDESKTOPPROVIDER_H
#define XDG_DESKTOP_PORTAL_APERTURE_FREEDESKTOPPROVIDER_H
#ifdef __cplusplus

#include "Settings.h"

namespace aperture {

    class FreedesktopProvider : public SettingsProvider {
        Q_OBJECT
        Q_DISABLE_COPY_MOVE(FreedesktopProvider)

    public:
        QString getNamespace() override;
        QVariant read(const QString &_namespace, const QString &key) override;
        QMap<QString, QVariantMap> readAll(const QStringList &namespaces) override

    };

} // aperture

#endif //__cplusplus
#endif //XDG_DESKTOP_PORTAL_APERTURE_FREEDESKTOPPROVIDER_H