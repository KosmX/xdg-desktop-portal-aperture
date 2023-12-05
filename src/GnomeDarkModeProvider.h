//
// Created by kosmx on 12/5/23.
//

#ifndef XDG_DESKTOP_PORTAL_APERTURE_GNOMEDARKMODEPROVIDER_H
#define XDG_DESKTOP_PORTAL_APERTURE_GNOMEDARKMODEPROVIDER_H
#ifdef __cplusplus

#include "Settings.h"
#include "FreedesktopProvider.h"

namespace aperture {

    class GnomeDarkModeProvider : public SettingsProvider {
        Q_OBJECT
        Q_DISABLE_COPY_MOVE(GnomeDarkModeProvider);

    public:
        GnomeDarkModeProvider(Settings& settings, FreedesktopProvider* freedesktopProvider);
        ~GnomeDarkModeProvider() override;

        QString getNamespace() final;

        QVariant read(const QString &_namespace, const QString &key) override;
        QMap<QString, QVariantMap> readAll(const QStringList &namespaces) override;

    private slots:

        void onSettingsChanged();

    private:
        FreedesktopProvider* freedesktopProvider;

    };

} // aperture

#endif //__cplusplus
#endif //XDG_DESKTOP_PORTAL_APERTURE_GNOMEDARKMODEPROVIDER_H
