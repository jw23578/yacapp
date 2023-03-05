#include "yacapp.h"
#include <QFileInfo>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include "yacAppAndServer/rightnumbers.h"

YACAPP::YACAPP(QQmlApplicationEngine &engine
               , Constants &constants
               , const Helper &helper
               , YACServerNetwork &network
               , CustomServerNetwork &customServerNetwork
               , QObject *parent)
    : QObject{parent},
      timer(0),
      constants(constants),
      helper(helper),
      localStorage(0),
      network(network),
      imageProvider(*this,
                    network),
      customServerNetwork(customServerNetwork),
      searchProfilesModel(engine, "SearchProfilesModel"),
      knownProfilesModel(engine, "KnownProfilesModel"),
      messagesModel(engine),
      appointmentsModel(engine),
      rightGroupsModel(engine, "RightGroupsModel", "rightgroup"),
      allRightsModel(engine, "AllRightsModel", "rightmpo"),
      spacesModel(engine, "SpacesModel", "space")
{
    for (const auto &rn: Rights::allRightNumberObjects)
    {
        MultiPurposeObject *mpo(new MultiPurposeObject);
        mpo->setId(QString::number(rn.second->number));
        mpo->setCaption(rn.second->meaning);
        allRightsModel.append(mpo);
    }
    engine.addImageProvider("async", &imageProvider);
    connect(&network, &NetworkInterface::missingRight, this, &YACAPP::missingRight);

    connect(&timer, &QTimer::timeout, this, &YACAPP::timeout);
    const int timerIntervalMSecs(100);
    timer.setInterval(timerIntervalMSecs);
    timer.start();

    connect(&firebase2qt, &Firebase2Qt::deviceTokenChanged, this, &YACAPP::deviceTokenChanged);
    connect(&firebase2qt, &Firebase2Qt::newMessages, this, &YACAPP::newMessages);

    qDebug() << __FILE__ << ": " << __LINE__ << constants.getStateFilename();
    QFile jsonFile(constants.getStateFilename());
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray fileData(jsonFile.readAll());
    QJsonDocument config(QJsonDocument::fromJson(fileData));
    stringFromJSON(globalProjectConfigFilename, GlobalProjectConfigFilename);
    stringFromJSON(loginToken, LoginToken);
    boolFromJSON(secondStart, SecondStart);
    if (!secondStart())
    {
        saveState();
    }
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
    appUserConfig()->clear();
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
    config["secondStart"] = true;

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

void YACAPP::addFileToFetch(const QString &imageType,
                            const QString &imageId,
                            AsyncImageResponse *air,
                            const QString &imageFilename)
{
    const std::lock_guard<std::mutex> guard(filesToFetchMutex);
    SFileToFetch ftf;
    ftf.imageId = imageId;
    ftf.imageType = imageType;
    ftf.air = air;
    ftf.imageFilename = imageFilename;
    filesToFetch.push_back(ftf);
}

void YACAPP::fetchFiles()
{
    const std::lock_guard<std::mutex> guard(filesToFetchMutex);
    for (auto ftf: filesToFetch)
    {
        network.appUserFetchImage(globalConfig()->projectID(),
                                  appUserConfig()->loginEMail(),
                                  appUserConfig()->loginToken(),
                                  ftf.imageType,
                                  ftf.imageId,
                                  [ftf](const QJsonDocument &jsonDoc)
        {
            QJsonObject imageData(jsonDoc.object());
            QByteArray dataBase64(imageData["imageDataBase64"].toString().toUtf8());
            QByteArray data(QByteArray::fromBase64(dataBase64));
            QFile file(ftf.imageFilename);
            file.open(QFile::WriteOnly);
            file.write(data);
            file.close();
            QImage image(ftf.imageFilename);
            ftf.air->setImage(image);
        }
        ,
        [](QString message)
        {
            int z1 = 0;
        });

    }
    filesToFetch.clear();
}

void YACAPP::timeout()
{
    fetchFiles();
}

void YACAPP::missingRight(int rightNumber)
{
    emit badMessage(tr("You are not allowed"), QJSValue::NullValue, QJSValue::NullValue);
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
        QString fstname(object[tableFields.fstname].toString());
        QString surname(object[tableFields.surname].toString());
        QString loginToken(object["loginToken"].toString());
        QString visible_name(object[tableFields.visible_name].toString());
        appUserConfig()->setLoginEMail(loginEMail);
        appUserConfig()->setLoginToken(loginToken);
        appUserConfig()->setFstname(fstname);
        appUserConfig()->setSurname(surname);
        appUserConfig()->setVisibleName(visible_name);
        appUserConfig()->setId(object[tableFields.id].toString());
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
        QString fstname(object[tableFields.fstname].toString());
        QString surname(object[tableFields.surname].toString());
        QString loginToken(object["loginToken"].toString());
        QString visible_name(object[tableFields.visible_name].toString());
        appUserConfig()->setLoginEMail(loginEMail);
        appUserConfig()->setLoginToken(loginToken);
        appUserConfig()->setFstname(fstname);
        appUserConfig()->setSurname(surname);
        appUserConfig()->setVisibleName(visible_name);
        appUserConfig()->setId(object[tableFields.id].toString());
        appUserConfig()->setProfileImageId(object[tableFields.image_id].toString());
        saveState();
        fetchMessageUpdates();
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

void YACAPP::appUserUpdateProfile(const QString &fstname,
                                  const QString &surname,
                                  const QString &visible_name,
                                  const QString &profileFilename,
                                  const bool searching_exactly_allowed,
                                  const bool searching_fuzzy_allowed,
                                  const QString &password,
                                  QJSValue successCallback,
                                  QJSValue errorCallback)
{
    appUserConfig()->setFstname(fstname);
    appUserConfig()->setSurname(surname);
    appUserConfig()->setVisibleName(visible_name);
    appUserConfig()->setSearchingFuzzyAllowed(searching_fuzzy_allowed);
    appUserConfig()->setSearchingExactlyAllowed(searching_exactly_allowed);
    saveState();
    network.appUserUpdateProfile(globalConfig()->projectID(),
                                 appUserConfig()->loginEMail(),
                                 appUserConfig()->loginToken(),
                                 fstname,
                                 surname,
                                 visible_name,
                                 profileFilename,
                                 searching_exactly_allowed,
                                 searching_fuzzy_allowed,
                                 password,
                                 [successCallback](const QString &message) mutable
    {
        Q_UNUSED(message);
        successCallback.call(QJSValueList());
    },
    [errorCallback](const QString &message) mutable
    {
        Q_UNUSED(message);
        errorCallback.call(QJSValueList());
    });

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
            po->setVisibleName(profile[tableFields.visible_name].toString());
            po->setProfileImageId(profile[tableFields.image_id].toString());
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

void YACAPP::appUserInsertAppointment(const QString &appointment_group_id,
                                      const QString &appointment_template_id,
                                      const QString &caption,
                                      const QDateTime &start_datetime,
                                      const QDateTime &end_datetime,
                                      const bool visible_for_everybody,
                                      QJSValue successCallback,
                                      QJSValue errorCallback)
{
    network.appUserInsertAppointment(globalConfig()->projectID(),
                                     appUserConfig()->loginEMail(),
                                     appUserConfig()->loginToken(),
                                     appointment_group_id,
                                     appointment_template_id,
                                     caption,
                                     start_datetime,
                                     end_datetime,
                                     visible_for_everybody,
                                     [this, successCallback](const QJsonDocument &jsonDoc) mutable
    {
        QJsonObject object(jsonDoc.object());
        QJsonObject appointment(object["appointment"].toObject());
        AppointmentObject *a(new AppointmentObject);
        a->fromJSON(appointment);
        appointmentsModel.append(a);
        successCallback.call(QJSValueList());
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserFetchAppointments(QJSValue successCallback,
                                      QJSValue errorCallback)
{
    network.appUserFetchAppointments(globalConfig()->projectID(),
                                     appUserConfig()->loginEMail(),
                                     appUserConfig()->loginToken(),
                                     [this, successCallback](const QJsonDocument &jsonDoc) mutable
    {
        QJsonObject object(jsonDoc.object());
        QJsonArray appointments(object["appointments"].toArray());
        for (size_t i(0); i < appointments.size(); ++i)
        {
            QJsonObject appointment(appointments[i].toObject());
            AppointmentObject *a(new AppointmentObject);
            a->fromJSON(appointment);
            appointmentsModel.append(a);
        }
        successCallback.call(QJSValueList());
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserDeleteAppointment(const QString &id,
                                      QJSValue successCallback,
                                      QJSValue errorCallback)
{
    network.appUserDeleteAppointment(globalConfig()->projectID(),
                                     appUserConfig()->loginEMail(),
                                     appUserConfig()->loginToken(),
                                     id,
                                     [successCallback, this, id](const QString &message) mutable
    {
        appointmentsModel.removeById(id);
        successCallback.call(QJSValueList() << message);
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    });
}

void YACAPP::appUserFetchRightGroups(QJSValue successCallback, QJSValue errorCallback)
{
    network.appUserFetchRightGroups(globalConfig()->projectID(),
                                    appUserConfig()->loginEMail(),
                                    appUserConfig()->loginToken(),
                                    [this, successCallback](const QJsonDocument &jsonDoc) mutable
    {
        rightGroupsModel.clear();
        QJsonObject object(jsonDoc.object());
        QJsonArray rightgroups(object["rightgroups"].toArray());
        for (size_t i(0); i < rightgroups.size(); ++i)
        {
            QJsonObject rightgroup(rightgroups[i].toObject());
            GroupObject *rg(new GroupObject);
            rg->fromJSON(rightgroup);
            rightGroupsModel.append(rg);
        }
        successCallback.call(QJSValueList());
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserInsertOrUpdateRightGroup(const QString &id, const QString &name, const bool automatic, QJSValue successCallback, QJSValue errorCallback)
{
    network.appUserInsertOrUpdateRightGroup(globalConfig()->projectID(),
                                            appUserConfig()->loginEMail(),
                                            appUserConfig()->loginToken(),
                                            id,
                                            name,
                                            automatic,
                                            [this, id, successCallback](const QJsonDocument &jsonDoc) mutable
    {
        QJsonObject object(jsonDoc.object());
        QJsonObject rightgroup(object["rightgroup"].toObject());
        if (id.size())
        {
            GroupObject *rg(rightGroupsModel.getById(id));
            if (rg)
            {
                rg->fromJSON(rightgroup);
            }
        }
        else
        {
            GroupObject *rg(new GroupObject);
            rg->fromJSON(rightgroup);
            rightGroupsModel.append(rg);
        }
        successCallback.call(QJSValueList());
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserDeleteRightGroup(const QString &id, QJSValue successCallback, QJSValue errorCallback)
{
    network.appUserDeleteRightGroup(globalConfig()->projectID(),
                                    appUserConfig()->loginEMail(),
                                    appUserConfig()->loginToken(),
                                    id,
                                    [this, id, successCallback](const QString &message) mutable
    {
        rightGroupsModel.deleteById(id);
        successCallback.call(QJSValueList() << message);
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserFetchRightGroup(const QString &id,
                                    QJSValue successCallback,
                                    QJSValue errorCallback)
{
    network.appUserFetchRightGroup(globalConfig()->projectID(),
                                   appUserConfig()->loginEMail(),
                                   appUserConfig()->loginToken(),
                                   id,
                                   [this, successCallback](const QJsonDocument &jsonDoc) mutable
    {
        QJsonObject object(jsonDoc.object());
        QJsonObject rightgroup(object["rightgroup"].toObject());
        QJsonArray rightNumbers(rightgroup["rightNumbers"].toArray());
        m_currentFetchedIds.clear();
        for (int i(0); i < rightNumbers.size(); ++i)
        {
            m_currentFetchedIds.push_back(rightNumbers[i].toString());
        }
        emit currentFetchedIdsChanged();
        successCallback.call(QJSValueList());
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserFetchSpaces(QJSValue successCallback, QJSValue errorCallback)
{
    network.appUserFetchSpaces(globalConfig()->projectID(),
                               appUserConfig()->loginEMail(),
                               appUserConfig()->loginToken(),
                               [this, successCallback](const QJsonDocument &jsonDoc) mutable
    {
        spacesModel.clear();
        QJsonObject object(jsonDoc.object());
        QJsonArray spaces(object["spaces"].toArray());
        for (size_t i(0); i < spaces.size(); ++i)
        {
            QJsonObject space(spaces[i].toObject());
            SpaceObject *so(new SpaceObject);
            so->fromJSON(space);
            spacesModel.append(so);
        }
        successCallback.call(QJSValueList());
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserInsertOrUpdateSpace(const QString &id,
                                        const QString &name,
                                        const QString &access_code,
                                        const bool automatic,
                                        QJSValue successCallback,
                                        QJSValue errorCallback)
{
    network.appUserInsertOrUpdateSpace(globalConfig()->projectID(),
                                       appUserConfig()->loginEMail(),
                                       appUserConfig()->loginToken(),
                                       id,
                                       name,
                                       access_code,
                                       automatic,
                                       [this, id, successCallback](const QJsonDocument &jsonDoc) mutable
    {
        QJsonObject object(jsonDoc.object());
        QJsonObject space(object["space"].toObject());
        if (id.size())
        {
            SpaceObject *so(spacesModel.getById(id));
            if (so)
            {
                so->fromJSON(space);
            }
        }
        else
        {
            SpaceObject *so(new SpaceObject);
            so->fromJSON(space);
            spacesModel.append(so);
        }
        successCallback.call(QJSValueList());
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserDeleteSpace(const QString &id, QJSValue successCallback, QJSValue errorCallback)
{
    network.appUserDeleteSpace(globalConfig()->projectID(),
                               appUserConfig()->loginEMail(),
                               appUserConfig()->loginToken(),
                               id,
                               [this, id, successCallback](const QString &message) mutable
    {
        spacesModel.deleteById(id);
        successCallback.call(QJSValueList() << message);
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );
}

void YACAPP::appUserFetchSpace(const QString &id, QJSValue successCallback, QJSValue errorCallback)
{
    network.appUserFetchSpace(globalConfig()->projectID(),
                              appUserConfig()->loginEMail(),
                              appUserConfig()->loginToken(),
                              id,
                              [this, successCallback](const QJsonDocument &jsonDoc) mutable
    {
        QJsonObject object(jsonDoc.object());
        QJsonObject space(object["space"].toObject());
        QJsonArray member(space["member"].toArray());
        m_currentFetchedIds.clear();
        for (int i(0); i < member.size(); ++i)
        {
            m_currentFetchedIds.push_back(member[i].toString());
        }
        emit currentFetchedIdsChanged();
        successCallback.call(QJSValueList());
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList() << message);
    }
    );

}

void YACAPP::appUserRequestSpaceAccess(const QString space_id, QJSValue successCallback, QJSValue errorCallback)
{
    network.appUserRequestSpaceAccess(globalConfig()->projectID(),
                              appUserConfig()->loginEMail(),
                              appUserConfig()->loginToken(),
                              space_id,
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

void YACAPP::fetchMyProfile(QJSValue successCallback,
                            QJSValue errorCallback)
{
    network.appUserFetchMyProfile(globalConfig()->projectID(),
                                  appUserConfig()->loginEMail(),
                                  appUserConfig()->loginToken(),
                                  [this, successCallback](const QJsonDocument &jsonDoc) mutable
    {
        QJsonObject profile(jsonDoc.object());
        appUserConfig()->setFstname(profile[tableFields.fstname].toString());
        appUserConfig()->setSurname(profile[tableFields.surname].toString());
        appUserConfig()->setVisibleName(profile[tableFields.visible_name].toString());
        appUserConfig()->setProfileImageId(profile[tableFields.image_id].toString());
        appUserConfig()->setSearchingExactlyAllowed(profile[tableFields.searching_exactly_allowed].toBool());
        appUserConfig()->setSearchingFuzzyAllowed(profile[tableFields.searching_fuzzy_allowed].toBool());
        successCallback.call(QJSValueList());
    },
    [errorCallback](const QString &message) mutable
    {
        errorCallback.call(QJSValueList());
        Q_UNUSED(message);
    });

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
                if (senderId != appUserConfig()->id())
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
                            po->setId(profile[tableFields.id].toString());
                            po->setVisibleName(profile[tableFields.visible_name].toString());
                            po->setProfileImageId(profile[tableFields.image_id].toString());
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
                    }
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

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif

void YACAPP::goTakePhoto(bool squared, bool circled, QJSValue target)
{
#ifdef Q_OS_ANDROID
    if (QtAndroid::checkPermission("android.permission.CAMERA") == QtAndroid::PermissionResult::Denied)
    {

    }
#endif
    emit takePhoto(squared, circled, target);
}

QString YACAPP::getNewProfileImageFilename()
{
    return constants.getWriteablePath(globalConfig()->projectID()) + "newProfileImage.jpg";
}

void YACAPP::fetchProfileAndUpsertKnownProfiles(const QString &profileId)
{
    network.appUserFetchProfile(globalConfig()->projectID(),
                                appUserConfig()->loginEMail(),
                                appUserConfig()->loginToken(),
                                profileId,
                                [this](const QJsonDocument &jsonDoc) mutable
    {
        QJsonObject profile(jsonDoc.object());
        QString id(profile[tableFields.id].toString());
        QString imageId(profile[tableFields.image_id].toString());
        QString visible_name(profile[tableFields.visible_name].toString());
        if (knownProfilesModel.contains(id))
        {
            ProfileObject &po(knownProfilesModel.getById(id));
            po.setProfileImageId(imageId);
            po.setVisibleName(visible_name);
            localStorage->upsertKnownContact(po);
        }
        return;

        ProfileObject *po(new ProfileObject);
        po->setId(id);
        po->setVisibleName(visible_name);
        po->setProfileImageId(imageId);
        if (knownProfilesModel.append(po))
        {
            localStorage->upsertKnownContact(*po);
        }
    },
    [](const QString &message) mutable
    {
        Q_UNUSED(message);
    });
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
