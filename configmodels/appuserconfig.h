#ifndef APPUSERCONFIG_H
#define APPUSERCONFIG_H

#include "configinterface.h"
#include "yacappmacros.h"
#include <QDateTime>
#include <QColor>

class AppUserConfig: public ConfigInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, fstname, Fstname, "");
    YACAPPPROPERTY(QString, surname, Surname, "");
    YACAPPPROPERTY(QString, visibleName, VisibleName, "");
    YACAPPPROPERTY(QColor, color, Color, "darkorange");
    YACAPPPROPERTY(QString, profileImageId, ProfileImageId, "");
    YACAPPPROPERTY(bool, searchingExactlyAllowed, SearchingExactlyAllowed, false);
    YACAPPPROPERTY(bool, searchingFuzzyAllowed, SearchingFuzzyAllowed, false);
    YACAPPPROPERTY(bool, saveLoginEMail, SaveLoginEMail, true);
    YACAPPPROPERTY(QString, loginEMail, LoginEMail, "");
    YACAPPPROPERTYCUSTOMSET(QString, loginToken, LoginToken, "", void setLoginToken(QString n){if (m_loginToken == n) return; m_loginToken = n; setLoggedIn(loginToken().size() > 0); emit loginTokenChanged();});
    YACAPPPROPERTY(bool, loggedIn, LoggedIn, false);
    YACAPPPROPERTY(QDateTime, workStart, WorkStart, QDateTime());
    YACAPPPROPERTY(QDateTime, pauseStart, PauseStart, QDateTime());
    YACAPPPROPERTY(QDateTime, offSiteWorkStart, OffSiteWorkStart, QDateTime());
    QByteArray publicKey;
    QByteArray privateKey;
    void saveKeyPair();
public:
    explicit AppUserConfig(QObject *parent = nullptr);

    void loadKeyPair();
    void setConfig(const QJsonValue &config) override;
    QJsonObject getConfig() const override;

    void clear();

    QString getPublicKey() const;
    QString getPrivateKey() const;
};

#endif // APPUSERCONFIG_H
