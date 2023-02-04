#include "yacapp.h"
#include <QFileInfo>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>

YACAPP::YACAPP(QQmlApplicationEngine &engine
               , Constants &constants
               , const Helper &helper
               , YACServerNetwork &network
               , CustomServerNetwork &customServerNetwork
               , QObject *parent)
    : QObject{parent},
      constants(constants),
      helper(helper),
      localStorage(0),
      network(network),
      customServerNetwork(customServerNetwork),
      searchProfilesModel(engine, "SearchProfilesModel"),
      knownProfilesModel(engine, "KnownProfilesModel"),
      messagesModel(engine)
{
    connect(&firebase2qt, &Firebase2Qt::deviceTokenChanged, this, &YACAPP::deviceTokenChanged);
    connect(&firebase2qt, &Firebase2Qt::newMessages, this, &YACAPP::newMessages);

    qDebug() << __FILE__ << ": " << __LINE__ << constants.getStateFilename();
    QFile jsonFile(constants.getStateFilename());
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray fileData(jsonFile.readAll());
    QJsonDocument config(QJsonDocument::fromJson(fileData));
    stringFromJSON(globalProjectConfigFilename, GlobalProjectConfigFilename);
    stringFromJSON(loginToken, LoginToken);
    deviceToken = config["deviceToken"].toString();

    //    for (size_t i(0); i < 3; ++i)
    //    {
    //        const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    //        const int randomStringLength = 12;

    //        QString randomString(QString::number(i) + "  ");
    //        for(int i=0; i<randomStringLength; ++i)
    //        {
    //            int index = rand() % possibleCharacters.length();
    //            QChar nextChar = possibleCharacters.at(index);
    //            randomString.append(nextChar);
    //        }
    //        QString senderId(knownProfilesModel.get(0).id());
    //        MessageObject *mo(new MessageObject(randomString,
    //                                            senderId,
    //                                            "",
    //                                            QDateTime::currentDateTime().addDays(i / 2),
    //                                            QDateTime::currentDateTime().addDays(i / 2),
    //                                            randomString,
    //                                            false));
    //        localStorage.insertMessage(*mo);

    //    }
}

void YACAPP::init(QString projectFilename)
{
    m_knownFiles.clear();
    m_knownMenueFiles.clear();
    projectFilename.replace("file://", "");
    setGlobalProjectConfigFilename(projectFilename);
    QString rawFolder(QFileInfo(projectFilename).path());
    rawFolder.replace("file://", "");
    if (rawFolder.right(1) != '/')
    {
        rawFolder += "/";
    }
    setAppFolder(rawFolder);
    if (appFolder() == "")
    {
        return;
    }
    globalConfig()->init(projectFilename);
    for (int i(0); i < globalConfig()->formFiles.size(); ++i)
    {
        getConfig(globalConfig()->formFiles[i]);
    }
    for (int i(0); i < globalConfig()->menueFiles.size(); ++i)
    {
        getMenueConfig(globalConfig()->menueFiles[i]);
    }
    setMainConfig(getConfig(globalConfig()->mainFormFilename()));
    cleanUpKnownFile();

    delete localStorage;
    if (globalConfig()->projectID().size())
    {
        loadAppConfig();
        localStorage = new LocalStorage(globalConfig()->projectID(), constants);
        knownProfilesModel.clear();
        localStorage->loadKnownContacts([this](DataObjectInterface *o){knownProfilesModel.append(dynamic_cast<ProfileObject*>(o));});
    }
}

void YACAPP::logout()
{
    setLoginToken("");
    saveState();
}

void YACAPP::leaveApp()
{
    setLoginToken("");
    setGlobalProjectConfigFilename("");
    globalConfig()->setProjectID("");
    {
        QMap<QString, ParsedConfig*>::iterator it(fileName2ParsedConfig.begin());
        while (it != fileName2ParsedConfig.end())
        {
            delete it.value();
            ++it;
        }
        fileName2ParsedConfig.clear();
    }
    {
        QMap<QString, MenueConfig*>::iterator it(fileName2MenueConfig.begin());
        while (it != fileName2MenueConfig.end())
        {
            delete it.value();
            ++it;
        }
        fileName2MenueConfig.clear();
    }
    m_knownFiles.clear();
    setGlobalConfig(new GlobalProjectConfig(true));
    saveState();
}

void YACAPP::saveState()
{
    QJsonObject config;
    stringToJSON(loginToken);
    stringToJSON(globalProjectConfigFilename);
    config["deviceToken"] = deviceToken;

    QFile jsonFile(constants.getStateFilename());
    jsonFile.open(QIODevice::WriteOnly);
    jsonFile.write(QJsonDocument(config).toJson());

    saveAppConfig();
}

void YACAPP::addKnownFile(QString const &filename)
{
    if (m_knownFiles.contains(filename))
    {
        return;
    }
    m_knownFiles.append(filename);
    emit knownFilesChanged();
}

void YACAPP::addKnownMenueFile(const QString &filename)
{
    if (m_knownMenueFiles.contains(filename))
    {
        return;
    }
    m_knownMenueFiles.append(filename);
    emit knownMenueFilesChanged();
}

void YACAPP::cleanUpKnownFile()
{
    {
        QMap<QString, ParsedConfig*>::iterator it(fileName2ParsedConfig.begin());
        while (it != fileName2ParsedConfig.end())
        {
            QFileInfo fi(it.key());
            if (!m_knownFiles.contains(fi.fileName()))
            {
                delete it.value();
                it = fileName2ParsedConfig.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    {
        QMap<QString, MenueConfig*>::iterator it(fileName2MenueConfig.begin());
        while (it != fileName2MenueConfig.end())
        {
            QFileInfo fi(it.key());
            if (!m_knownMenueFiles.contains(fi.fileName()))
            {
                delete it.value();
                it = fileName2MenueConfig.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void YACAPP::loadAppConfig()
{
    QFile jsonFile(constants.getAppConfigFilename(globalConfig()->projectID()));
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray fileData(jsonFile.readAll());
    QJsonDocument config(QJsonDocument::fromJson(fileData));

    appUserConfig()->setConfig(config["appUserConfig"]);
    QString serverNowISO(config["serverNowISO"].toString());
    if (serverNowISO.size())
    {
        setServerNow(QDateTime::fromString(serverNowISO, Qt::ISODate));
    }
}

void YACAPP::saveAppConfig()
{
    QJsonObject config;

    config["appUserConfig"] = appUserConfig()->getConfig();
    QString help(serverNow().toString(Qt::ISODate));
    config["serverNowISO"] = serverNow().toString(Qt::ISODate);

    QFile jsonFile(constants.getAppConfigFilename(globalConfig()->projectID()));
    jsonFile.open(QIODevice::WriteOnly);
    jsonFile.write(QJsonDocument(config).toJson());

}

ParsedConfig *YACAPP::getConfig(const QString &filename)
{
    QString fullFilename(appFolder() + filename);
    QMap<QString, ParsedConfig*>::Iterator configIt(fileName2ParsedConfig.find(fullFilename));
    if (configIt == fileName2ParsedConfig.end())
    {
        fileName2ParsedConfig[fullFilename] = new ParsedConfig;
        configIt = fileName2ParsedConfig.find(fullFilename);
    }
    if (!m_knownFiles.contains(filename))
    {
        configIt.value()->init(fullFilename);
        addKnownFile(filename);
    }
    return configIt.value();
}

MenueConfig *YACAPP::getMenueConfig(const QString &filename)
{
    if (!filename.size())
    {
        return &emptyMenue;
    }
    QString fullFilename(appFolder() + filename);
    QMap<QString, MenueConfig*>::Iterator configIt(fileName2MenueConfig.find(fullFilename));
    if (configIt == fileName2MenueConfig.end())
    {
        fileName2MenueConfig[fullFilename] = new MenueConfig;
        configIt = fileName2MenueConfig.find(fullFilename);
    }
    if (!m_knownMenueFiles.contains(filename))
    {
        configIt.value()->init(fullFilename);
        addKnownMenueFile(filename);
    }
    return configIt.value();
}

void YACAPP::loadNewProject(const QString &projectFilename)
{
    if (projectFilename.size() == 0)
    {
        return;
    }
    init(projectFilename);
}

void YACAPP::saveCurrentProject()
{
    globalConfig()->save(globalProjectConfigFilename());
    {
        QMap<QString, ParsedConfig*>::iterator it(fileName2ParsedConfig.begin());
        while (it != fileName2ParsedConfig.end())
        {
            it.value()->save(it.key());
            ++it;
        }
    }
    {
        QMap<QString, MenueConfig*>::iterator it(fileName2MenueConfig.begin());
        while (it != fileName2MenueConfig.end())
        {
            it.value()->save(it.key());
            ++it;
        }
    }
}

void YACAPP::downloadApp(QString url
                         , QString projectID
                         , QJSValue successCallback
                         , QJSValue errorCallback)
{
    if (url.right(1) != '/')
    {
        url += '/';
    }

    customServerNetwork.downloadApp(url + projectID + ".yacapp"
                                    , url + projectID + ".yacpck"
                                    , projectID
                                    , [this, projectID, successCallback](const QString &message) mutable
    {
        Q_UNUSED(message);
        loadNewProject(constants.getYacAppConfigPath(projectID) + projectID + ".yacapp");
        saveState();
        successCallback.call(QJSValueList());
    }
    , [errorCallback](const QString &errorMessage) mutable
    {
        qDebug() << __FILE__ << ": " << __LINE__ << errorMessage;
        errorCallback.call(QJSValueList() << errorMessage);
    });
}

void YACAPP::yacappServerGetAllAPPs(QJSValue successCallback,
                                    QJSValue errorCallback)
{
    network.yacappServerGetAllAPPs([successCallback](const QString &message) mutable
    {
        successCallback.call(QJSValueList() << message);
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    });
}

void YACAPP::yacappServerGetAPP(const QString &app_id,
                                const int current_installed_version,
                                QJSValue successCallback,
                                QJSValue errorCallback)
{
    network.yacappServerGetAPP(app_id,
                               current_installed_version,
                               [this, app_id, successCallback](const QString &message) mutable
    {
        if (message == "app version is up to date")
        {
            successCallback.call(QJSValueList() << message);
            return;
        }
        loadNewProject(constants.getYacAppConfigPath(app_id) + app_id + ".yacapp");
        saveState();
        successCallback.call(QJSValueList() << message);
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    });
}

void YACAPP::appUserRegister(const QString &loginEMail,
                             const QString &password,
                             QJSValue successCallback,
                             QJSValue errorCallback)
{
    network.yacappServerAppUserRegister(loginEMail,
                                        password,
                                        globalConfig()->projectID(),
                                        [successCallback](const QString &message) mutable
    {
        successCallback.call(QJSValueList() << message);
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserRequestVerifyToken(const QString &loginEMail,
                                       QJSValue successCallback,
                                       QJSValue errorCallback)
{
    network.yacappServerAppUserRequestVerifyToken(loginEMail,
                                                  globalConfig()->projectID(),
                                                  [successCallback](const QString &message) mutable
    {
        successCallback.call(QJSValueList() << message);
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserVerify(const QString &loginEMail,
                           const QString &verifyToken,
                           QJSValue successCallback,
                           QJSValue errorCallback)
{
    network.yacappServerAppUserVerify(loginEMail,
                                      verifyToken,
                                      globalConfig()->projectID(),
                                      [this, loginEMail, successCallback](const QJsonDocument &jsonDoc) mutable
    {
        QJsonObject object(jsonDoc.object());
        QString fstname(object["fstname"].toString());
        QString surname(object["surname"].toString());
        QString loginToken(object["loginToken"].toString());
        QString visible_name(object["visible_name"].toString());
        appUserConfig()->setLoginEMail(loginEMail);
        appUserConfig()->setLoginToken(loginToken);
        appUserConfig()->setFstname(fstname);
        appUserConfig()->setSurname(surname);
        appUserConfig()->setVisibleName(visible_name);
        saveState();
        successCallback.call(QJSValueList());

//        successCallback.call(QJSValueList() << message);
//        appUserConfig()->setLoginEMail(loginEMail);
//        appUserConfig()->setLoginToken(message);
//        saveState();
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserLogin(const QString &loginEMail,
                          const QString &password,
                          QJSValue successCallback,
                          QJSValue errorCallback)
{
    network.yacappServerAppUserLogin(loginEMail,
                                     password,
                                     globalConfig()->projectID(),
                                     deviceToken,
                                     [this, loginEMail, successCallback](const QJsonDocument &jsonDoc) mutable
    {
        QJsonObject object(jsonDoc.object());
        QString fstname(object["fstname"].toString());
        QString surname(object["surname"].toString());
        QString loginToken(object["loginToken"].toString());
        QString visible_name(object["visible_name"].toString());
        appUserConfig()->setLoginEMail(loginEMail);
        appUserConfig()->setLoginToken(loginToken);
        appUserConfig()->setFstname(fstname);
        appUserConfig()->setSurname(surname);
        appUserConfig()->setVisibleName(visible_name);
        saveState();
        successCallback.call(QJSValueList());
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserLogout()
{
    if (!appUserConfig()->saveLoginEMail())
    {
        appUserConfig()->setLoginEMail("");
    }
    appUserConfig()->setSaveLoginEMail(true);
    appUserConfig()->setLoginToken("");
    appUserConfig()->setFstname("");
    appUserConfig()->setSurname("");
    appUserConfig()->setVisibleName("");
    saveState();
}

void YACAPP::appUserRequestPasswordUpdate(const QString &loginEMail,
                                          QJSValue successCallback,
                                          QJSValue errorCallback)
{
    network.appUserRequestPasswordUpdate(loginEMail,
                                         globalConfig()->projectID(),
                                         [successCallback](const QString &message) mutable
    {
        successCallback.call(QJSValueList() << message);
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserUpdatePassword(const QString &loginEMail,
                                   const QString &password,
                                   const QString &updatePasswordToken,
                                   QJSValue successCallback,
                                   QJSValue errorCallback)
{
    network.appUserUpdatePassword(loginEMail,
                                  password,
                                  updatePasswordToken,
                                  globalConfig()->projectID(),
                                  [successCallback](const QString &message) mutable
    {
        successCallback.call(QJSValueList() << message);
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserGetWorktimeState(QJSValue successCallback,
                                     QJSValue errorCallback)
{
    if (!appUserConfig()->loginToken().size())
    {
        errorCallback.call(QJSValueList() << tr("please login first"));
        return;
    }
    network.appUserGetWorktimeState(globalConfig()->projectID(),
                                    appUserConfig()->loginEMail(),
                                    appUserConfig()->loginToken(),
                                    [successCallback, this](const QJsonDocument &jsonDoc) mutable
    {
        QJsonObject object(jsonDoc.object());
        QString message(object["message"].toString());
        QString workStart(object["workStart"].toString());
        QString pauseStart(object["pauseStart"].toString());
        QString offSiteWorkStart(object["offSiteWorkStart"].toString());
        appUserConfig()->setWorkStart(QDateTime::fromString(workStart, Qt::DateFormat::ISODateWithMs));
        appUserConfig()->setPauseStart(QDateTime::fromString(pauseStart, Qt::DateFormat::ISODateWithMs));
        appUserConfig()->setOffSiteWorkStart(QDateTime::fromString(offSiteWorkStart, Qt::DateFormat::ISODateWithMs));
        successCallback.call(QJSValueList() << message);
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserInsertWorktime(int worktimeType,
                                   int userMood,
                                   int dayRating,
                                   QJSValue successCallback,
                                   QJSValue errorCallback)
{
    if (!appUserConfig()->loginToken().size())
    {
        errorCallback.call(QJSValueList() << tr("please login first"));
        return;
    }
    network.appUserInsertWorktime(globalConfig()->projectID(),
                                  appUserConfig()->loginEMail(),
                                  appUserConfig()->loginToken(),
                                  worktimeType,
                                  userMood,
                                  dayRating,
                                  QDateTime::currentDateTime(),
                                  [successCallback, this](const QJsonDocument &jsonDoc) mutable
    {
        QJsonObject object(jsonDoc.object());
        QString message(object["message"].toString());
        QString workStart(object["workStart"].toString());
        QString pauseStart(object["pauseStart"].toString());
        QString offSiteWorkStart(object["offSiteWorkStart"].toString());
        appUserConfig()->setWorkStart(QDateTime::fromString(workStart, Qt::DateFormat::ISODateWithMs));
        appUserConfig()->setPauseStart(QDateTime::fromString(pauseStart, Qt::DateFormat::ISODateWithMs));
        appUserConfig()->setOffSiteWorkStart(QDateTime::fromString(offSiteWorkStart, Qt::DateFormat::ISODateWithMs));
        successCallback.call(QJSValueList() << message);
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );

}

void YACAPP::appUserSearchProfiles(const QString &needle,
                                   const int limit,
                                   const int offset,
                                   QJSValue successCallback,
                                   QJSValue errorCallback)
{
    if (needle.size() == 0)
    {
        searchProfilesModel.clear();
        return;
    }
    if (!appUserConfig()->loginToken().size())
    {
        errorCallback.call(QJSValueList() << tr("please login first"));
        return;
    }
    network.appUserSearchProfiles(globalConfig()->projectID(),
                                  appUserConfig()->loginEMail(),
                                  appUserConfig()->loginToken(),
                                  needle,
                                  limit,
                                  offset,
                                  [successCallback, this](const QJsonDocument &jsonDoc) mutable
    {
        QJsonObject object(jsonDoc.object());
        QString message(object["message"].toString());
        searchProfilesModel.clear();
        QJsonArray profiles(object["profiles"].toArray());
        for (int i(0); i < profiles.size(); ++i)
        {
            ProfileObject *po(new ProfileObject);
            QJsonObject profile(profiles[i].toObject());
            po->setId(profile["id"].toString());
            po->setVisibleName(profile["visible_name"].toString());
            searchProfilesModel.append(po);
        }
        successCallback.call(QJSValueList() << message);
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );

}

void YACAPP::fetchMessageUpdates()
{
    if (!appUserConfig()->loggedIn())
    {
        return;
    }
    network.appUserFetchMessageUpdates(globalConfig()->projectID(),
                                       appUserConfig()->loginEMail(),
                                       appUserConfig()->loginToken(),
                                       serverNow(),
                                       [this](const QJsonDocument &jsonDoc) mutable
    {
        QJsonObject object(jsonDoc.object());
        QString sni(jsonDoc["serverNowISO"].toString());
        setServerNow(QDateTime::fromString(sni, "yyyy-MM-ddThh:mm:ssZ"));
        saveState();
        QJsonArray messages(object["messages"].toArray());
        for (int i(0); i < messages.size(); ++i)
        {
            const QJsonObject message(messages[i].toObject());
            QByteArray content_base64(message["content_base64"].toString().toUtf8());
            QString toId(message["to_id"].toString());
            QString senderId(message["sender_id"].toString());
            MessageObject *mo(new MessageObject(message["id"].toString(),
                              senderId,
                              toId,
                              QDateTime::currentDateTime(),
                              QDateTime::currentDateTime(),
                              QByteArray::fromBase64(content_base64),
                              false));
            if (localStorage->insertMessage(*mo))
            {
                if (!knownProfilesModel.incUnreadMessages(senderId))
                {
                    network.appUserFetchProfile(globalConfig()->projectID(),
                                                appUserConfig()->loginEMail(),
                                                appUserConfig()->loginToken(),
                                                senderId,
                                                [this](const QJsonDocument &jsonDoc) mutable
                    {
                        QJsonObject profile(jsonDoc.object());
                        ProfileObject *po(new ProfileObject);
                        po->setId(profile["id"].toString());
                        po->setVisibleName(profile["visible_name"].toString());
                        if (knownProfilesModel.append(po))
                        {
                            localStorage->upsertKnownContact(*po);
                            knownProfilesModel.incUnreadMessages(po->id());
                        }
                    },
                    [](const QString &message) mutable
                    {
                        Q_UNUSED(message);
                    });
                    // fetch Profile and incUnreadMessage
                }
                if (messagesModel.profileId() == senderId)
                {
                    messagesModel.append(mo);
                }
            }
        }
    },
    [](const QString &message) mutable
    {
        Q_UNUSED(message);
    }
    );
}

void YACAPP::loadMessages(const QString &contactId)
{
    messagesModel.clear();
    localStorage->loadMessages(contactId, [this](DataObjectInterface *o){messagesModel.append(dynamic_cast<MessageObject*>(o));});
    messagesModel.setProfileID(contactId);
}

void YACAPP::sendMessage(const QString &profileId, const QString &content)
{
    MessageObject *mo(new MessageObject(QUuid::createUuid().toString(QUuid::WithoutBraces),
                                        "",
                                        profileId,
                                        QDateTime::currentDateTime(),
                                        QDateTime::currentDateTime(),
                                        content,
                                        false));
    messagesModel.append(mo);
    localStorage->insertMessage(*mo);

    network.appUserStoreMessage(globalConfig()->projectID(),
                                appUserConfig()->loginEMail(),
                                appUserConfig()->loginToken(),
                                mo->id(),
                                mo->receiverId(),
                                mo->base64(),
                                [](const QString &message)
    {
        Q_UNUSED(message);
    },
    [](const QString &message)
    {
        Q_UNUSED(message);
    });
}

void YACAPP::addProfileToKnownProfiles(const QString &id)
{
    ProfileObject *po(searchProfilesModel.getCopyById(id));
    if (knownProfilesModel.append(po))
    {
        localStorage->upsertKnownContact(*po);
    }
}

void YACAPP::removeProfileFromKnownProfiles(const QString &id)
{
    knownProfilesModel.removeById(id);
    localStorage->deleteKnownContact(id);
}

void YACAPP::switchLanguage(const QString &language)
{
    const QString baseName = "yacapp_" + language;
    if (translator.load(":/i18n/" + baseName)) {
        QCoreApplication::installTranslator(&translator);
    }

}

void YACAPP::deviceTokenChanged(QString deviceToken)
{
    if (this->deviceToken == deviceToken)
    {
        return;
    }
    this->deviceToken = deviceToken;
    saveState();
    qDebug() << "YACAPP: " << deviceToken;
    if (!appUserConfig()->loggedIn())
    {
        qDebug() << "not logged in";
        return;
    }
    network.appUserUpdateDeviceToken(globalConfig()->projectID(),
                                     appUserConfig()->loginEMail(),
                                     appUserConfig()->loginToken(),
                                     deviceToken,
                                     [](const QString &message)
         {
             Q_UNUSED(message);
         },
         [](const QString &message)
         {
             Q_UNUSED(message);
    });
}

void YACAPP::newMessages()
{
    fetchMessageUpdates();
}
