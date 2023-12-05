//
// Created by kosmx on 12/5/23.
//

#ifndef XDG_DESKTOP_PORTAL_APERTURE_KDEPROVIDER_H
#define XDG_DESKTOP_PORTAL_APERTURE_KDEPROVIDER_H
#ifdef __cplusplus

#include "Settings.h"

namespace aperture {

    class KdeProvider : public SettingsProvider {
        Q_OBJECT;

    public:
        KdeProvider(Settings& parent) : SettingsProvider(parent) {}

        QString getNamespace() override final;

        QVariant read(const QString &_namespace, const QString &key) override;

        QMap<QString, QVariantMap> readAll(const QStringList &namespaces) override;

    };

} // aperture

#endif //__cplusplus
#endif //XDG_DESKTOP_PORTAL_APERTURE_KDEPROVIDER_H
