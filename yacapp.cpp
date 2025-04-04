#include "yacapp.h"
#include <QFileInfo>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include "yacAppAndServer/rightnumbers.h"
#include "logger.h"
#include "orm_implementions/t0028_message_images.h"
#include "QUrlQuery"
#include <QDir>
#include "orm_implementions/t0030_documents.h"

bool testFunction(TemplatedDataModel<t0030_documents> &target)
{
    return true;
}

YACAPP::YACAPP(QQmlApplicationEngine &engine
               , CPPQMLAppAndConfigurator &cppQMLAppAndConfigurator
               , ThirdPartyLogin &thirdPartyLogin
               , Constants &constants
               , const Helper &helper
               , YACServerNetwork &network
               , CustomServerNetwork &customServerNetwork
               , QObject *parent)
    : QObject{parent},
    timer(0),
    cppQMLAppAndConfigurator(cppQMLAppAndConfigurator),
    thirdPartyLogin(thirdPartyLogin),
    constants(constants),
    helper(helper),
    localStorage(0),
    network(network),
    customServerNetwork(customServerNetwork),
    knownProfilesModel(engine, "KnownProfilesModel"),
    selectedProfilesModel(engine, "SelectedProfilesModel"),
    searchProfilesModel(engine, "SearchProfilesModel"),
    messagesModel(engine),
    appointmentsModel(engine, "AppointmentsModel", "appointment"),
    rightGroupsModel(engine, "RightGroupsModel", "rightgroup"),
    allRightsModel(engine, "AllRightsModel", "rightmpo"),
    spacesModel(engine, "SpacesModel", "space"),
    spaceRequestsModel(engine, "SpaceRequestsModel", "spaceRequest"),
    worktimeMainsModel(engine, "WorktimesModel", "worktime"),
    newsModel(engine, "NewsModel", "news"),
    documentsModel(engine, "DocumentsModel", "document")
{
    documentsModel.setFetchMoreFuntion([this](TemplatedDataModel<t0030_documents> &target)
                                       {
        appUserFetchDocuments(currentFetchDocumentsNeedle, target.size(), 10, QJSValue::NullValue, QJSValue::NullValue);
    });
    network.acceptErrors("/getAPP", 3);
    network.acceptErrors("/fetchMessageUpdates", 100);
    network.networkDefectCallback = [this](const QString &message){setServerConnectionDefectMessage(message);};
    network.networkGoodCallback = [this](){setServerConnectionDefectMessage("");};
    m_moodModel.push_back(tr("Perfect"));
    m_moodModel.push_back(tr("Better"));
    m_moodModel.push_back(tr("Neutral"));
    m_moodModel.push_back(tr("Worse"));
    m_moodModel.push_back(tr("Bad"));
    for (const auto &rn: Rights::allRightNumberObjects)
    {
        MultiPurposeObject *mpo(new MultiPurposeObject);
        mpo->setId(QString::number(rn.second->number));
        mpo->setCaption(rn.second->meaning);
        allRightsModel.append(mpo);
    }
    engine.addImageProvider("async", new AsyncImageProvider(*this,
                                                            network));
    connect(&network, &NetworkInterface::missingRight, this, &YACAPP::missingRight);

    connect(&timer, &QTimer::timeout, this, &YACAPP::timeout);
    const int timerIntervalMSecs(100);
    timer.setInterval(timerIntervalMSecs);
    timer.start();

    connect(&firebase2qt, &Firebase2Qt::deviceTokenChanged, this, &YACAPP::deviceTokenChanged);
    connect(&firebase2qt, &Firebase2Qt::newMessages, this, &YACAPP::newMessages);

    DEFAULT_LOG(QString("constants.getStateFilename(): ") + constants.getStateFilename());
    QFile jsonFile(constants.getStateFilename());
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray fileData(jsonFile.readAll());
    QJsonDocument config(QJsonDocument::fromJson(fileData));
    stringFromJSON(globalProjectConfigFilename, GlobalProjectConfigFilename);

    thirdPartyLogin.setLoginToken(config["loginToken"].toString());  // should be removed from here to thirdPartyLogin FIXME

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

void YACAPP::init()
{
    m_knownFiles.clear();
    m_knownMenueFiles.clear();

    globalConfig()->init(globalProjectConfigFilename(), constants);
    DEFAULT_LOG(QString("Projectname: ") + globalConfig()->projectName());
    DEFAULT_LOG(QString("Third: ") + globalConfig()->third());
    DEFAULT_LOG(QString("Mandant: ") + globalConfig()->mandant());
    network.setThirdMandant(globalConfig()->third(), globalConfig()->mandant());
    setApplicationTitle(globalConfig()->projectName());

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
    setUpdateTrigger(updateTrigger() + 1);
}

QString YACAPP::getProfileVisibleName(const QString &id)
{
    return knownProfilesModel.getById(id).visibleName();
}

QString YACAPP::getProfileImageId(const QString &id)
{
    return knownProfilesModel.getById(id).profileImageId();
}

void YACAPP::logout()
{
    // logout should probably moved to ThirdPartyLogin FIXME
    thirdPartyLogin.setLoginToken("");
    saveState();
}

void YACAPP::leaveApp()
{
    QString appId(globalConfig()->projectID());
    delete localStorage;
    localStorage = 0;
    thirdPartyLogin.setLoginToken(""); // should probably moved to ThirdPartyLogin FIXME
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

    QDir path(constants.getWriteablePath(appId));
    path.removeRecursively();
    path.setPath(constants.getCachePath(appId));
    path.removeRecursively();
    // TODO: clear all Models (am besten irgendwie zentral)
}

void YACAPP::saveState()
{
    QJsonObject config;

    config["loginToken"] = thirdPartyLogin.loginToken();  // FIXME, move to ThirdPartyLogin

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
        if (ftf.imageType == "appImage")
        {
            network.getAppImage(ftf.imageId,
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
                                    Q_UNUSED(message)
                                });
        }
        else
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
                                          Q_UNUSED(message)
                                      });
        }
    }
    filesToFetch.clear();
}

void YACAPP::timeout()
{
    fetchFiles();
}

void YACAPP::missingRight(int rightNumber)
{
    Q_UNUSED(rightNumber)
    emit cppQMLAppAndConfigurator.badMessage(tr("You are not allowed"), QJSValue::NullValue, QJSValue::NullValue);
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
    if (!jsonFile.exists())
    {
        return;
    }
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

QString YACAPP::coalesce(const QString &s, const QString &ifEmpty) const
{
    if (s.size())
    {
        return s;
    }
    return ifEmpty;
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

void YACAPP::loadAppAndInitialize(QString projectFilename)
{
    if (projectFilename.size() == 0)
    {
        return;
    }
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
    init();
}

void YACAPP::saveAppToFile(QString projectFilename)
{
    projectFilename.replace("file://", "");
    globalConfig()->save(projectFilename, constants);
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
                                const QString &installation_code,
                                const int current_installed_version,
                                QJSValue successCallback,
                                QJSValue errorCallback)
{
    network.yacappServerGetAPP(app_id,
                               installation_code,
                               current_installed_version,
                               [this, app_id, successCallback](const QString &message) mutable
                               {
                                   if (message == "app version is up to date")
                                   {
                                       successCallback.call(QJSValueList() << message);
                                       return;
                                   }
                                   loadAppAndInitialize(constants.getYacAppConfigPath(app_id) + app_id + ".yacapp");
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
                                         appUserConfig()->loadKeyPair();
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
        errorCallback.call(QJSValueList() << tr("Please login first"));
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
        errorCallback.call(QJSValueList() << tr("Please login first"));
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

void YACAPP::appUserInsertWorktimeBeginEnd(const int worktimeType,
                                           const QDateTime begin,
                                           const QDateTime end,
                                           QJSValue successCallback,
                                           QJSValue errorCallback)
{
    if (!appUserConfig()->loginToken().size())
    {
        errorCallback.call(QJSValueList() << tr("Please login first"));
        return;
    }
    network.appUserInsertWorktimeBeginEnd(globalConfig()->projectID(),
                                          appUserConfig()->loginEMail(),
                                          appUserConfig()->loginToken(),
                                          worktimeType,
                                          begin,
                                          end,
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

void YACAPP::appUserFetchWorktimes(const QDateTime &since,
                                   const QDateTime &until,
                                   QJSValue successCallback,
                                   QJSValue errorCallback)
{
    network.appUserFetchWorktimes(globalConfig()->projectID(),
                                  appUserConfig()->loginEMail(),
                                  appUserConfig()->loginToken(),
                                  since,
                                  until,
                                  [successCallback, this](const QJsonDocument &jsonDoc) mutable
                                  {
                                      QJsonObject object(jsonDoc.object());
                                      worktimeMainsModel.clear();
                                      QJsonArray worktimes(object["worktimes"].toArray());
                                      WorktimeMainObject *activeMainObject(0);
                                      QDateTime currentWorkStart;
                                      QDateTime currentPauseStart;
                                      QDateTime currentOffSiteWorkStart;
                                      for (int i(0); i < worktimes.size(); ++i)
                                      {
                                          WorktimeObject *wo(new WorktimeObject);
                                          QJsonObject worktime(worktimes[i].toObject());
                                          wo->fromJSON(worktime);
                                          if (activeMainObject == 0 && wo->type() != WorktimeObject::WorkStartType)
                                          {
                                              continue;
                                          }
                                          if (activeMainObject)
                                          {
                                              activeMainObject->subentries.append(wo);
                                          }
                                          if (wo->type() == WorktimeObject::WorkEndType)
                                          {
                                              activeMainObject->setend_ts(wo->ts());
                                              int minutes(helper.minutesBetween(currentWorkStart, wo->ts()));
                                              activeMainObject->setbrutto_work_minutes(minutes);
                                              minutes -= activeMainObject->brutto_pause_minutes();
                                              int autopauseMinutes(0);
                                              if (minutes > 60 * 9)
                                              {
                                                  if (activeMainObject->netto_pause_minutes() < 45)
                                                  {
                                                      autopauseMinutes = std::min(minutes - 60 * 9, 45 - activeMainObject->netto_pause_minutes());
                                                  }
                                              }
                                              else
                                              {
                                                  if (minutes > 60 * 6)
                                                  {
                                                      if (activeMainObject->netto_pause_minutes() < 30)
                                                      {
                                                          autopauseMinutes = std::min(minutes - 60 * 6, 30 - activeMainObject->netto_pause_minutes());
                                                      }
                                                  }

                                              }
                                              minutes -= autopauseMinutes;
                                              activeMainObject->setautopause_minutes(autopauseMinutes);
                                              activeMainObject->setnetto_work_minutes(minutes);

                                              DEFAULT_LOG(QString("worktimeMainsModel.count: ") + QString::number(worktimeMainsModel.size()));

                                              worktimeMainsModel.append(activeMainObject);

                                              DEFAULT_LOG(QString("worktimeMainsModel.count: ") + QString::number(worktimeMainsModel.size()));
                                              activeMainObject = 0;
                                          }
                                          if (wo->type() == WorktimeObject::PauseStartType)
                                          {
                                              currentPauseStart = wo->ts();
                                          }
                                          if (wo->type() == WorktimeObject::PauseEndType)
                                          {
                                              int minutes(helper.minutesBetween(currentPauseStart, wo->ts()));
                                              activeMainObject->setbrutto_pause_minutes(activeMainObject->brutto_pause_minutes() + minutes);
                                              if (minutes >= 15)
                                              {
                                                  activeMainObject->setnetto_pause_minutes(activeMainObject->netto_pause_minutes() + minutes);
                                              }
                                          }
                                          if (wo->type() == WorktimeObject::OffSiteWorkStartType)
                                          {
                                              currentOffSiteWorkStart = wo->ts();
                                          }
                                          if (wo->type() == WorktimeObject::OffSiteWorkEndType)
                                          {
                                              int minutes(helper.minutesBetween(currentOffSiteWorkStart, wo->ts()));
                                              activeMainObject->setbrutto_offsitework_minutes(activeMainObject->brutto_offsitework_minutes() + minutes);
                                          }
                                          if (wo->type() == WorktimeObject::WorkStartType)
                                          {
                                              activeMainObject = new WorktimeMainObject;
                                              activeMainObject->setId(helper.generateUuid());
                                              activeMainObject->setbegin_ts(wo->ts());
                                              currentWorkStart = wo->ts();
                                          }
                                      }
                                      if (activeMainObject)
                                      {
                                          worktimeMainsModel.append(activeMainObject);
                                          activeMainObject = 0;
                                      }

                                      successCallback.call(QJSValueList());
                                  },
                                  [errorCallback](const QString &message) mutable
                                  {
                                      errorCallback.call(QJSValueList() << message);
                                  });
}

void YACAPP::appUserDeleteWorktime(const QString &id, QJSValue successCallback, QJSValue errorCallback)
{
    if (!appUserConfig()->loginToken().size())
    {
        errorCallback.call(QJSValueList() << tr("Please login first"));
        return;
    }
    network.appUserDeleteWorktime(globalConfig()->projectID(),
                                  appUserConfig()->loginEMail(),
                                  appUserConfig()->loginToken(),
                                  id,
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

void YACAPP::appUserUpdateProfile(const QString &fstname,
                                  const QString &surname,
                                  const QString &visible_name,
                                  const QColor &color,
                                  const QColor &message_font_color,
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
    appUserConfig()->setColor(color);
    appUserConfig()->setMessageFontColor(message_font_color);
    appUserConfig()->setSearchingFuzzyAllowed(searching_fuzzy_allowed);
    appUserConfig()->setSearchingExactlyAllowed(searching_exactly_allowed);
    saveState();
    network.appUserUpdateProfile(globalConfig()->projectID(),
                                 appUserConfig()->loginEMail(),
                                 appUserConfig()->loginToken(),
                                 fstname,
                                 surname,
                                 visible_name,
                                 color,
                                 message_font_color,
                                 profileFilename,
                                 searching_exactly_allowed,
                                 searching_fuzzy_allowed,
                                 password,
                                 appUserConfig()->getPublicKey(),
                                 [this, successCallback](const QJsonDocument &jsonDoc) mutable
                                 {
                                     QJsonObject object(jsonDoc.object());
                                     QString image_id(object[tableFields.image_id].toString());
                                     if (helper.validUuid(image_id))
                                     {
                                         appUserConfig()->setProfileImageId(image_id);
                                     }
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
        errorCallback.call(QJSValueList() << tr("Please login first"));
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

void YACAPP::appUserDeleteDocument(QString const &id,
                                   QJSValue errorCallback)
{
    network.appUserDeleteORM(globalConfig()->projectID(),
                             appUserConfig()->loginEMail(),
                             appUserConfig()->loginToken(),
                             t0030_documents().getORMName(),
                             id,
                             [this, id](const QJsonDocument &jsonDoc) mutable
                             {
                                 documentsModel.removeById(id);
                             },
                             [errorCallback](const QString &message) mutable
                             {
                                 errorCallback.call(QJSValueList() << message);
                             }
                             );
}


void YACAPP::appUserFetchDocument(QString const id,
                                  QString const document_name,
                                  QString const document_type,
                                  QJSValue successCallback,
                                  QJSValue errorCallback)
{
    QString idFilename(getCacheFile(id));
    QString documentFilename(getCacheFile(document_name + "." + document_type));
    QFileInfo fiIDFile(idFilename);
    QFileInfo fiDocumentFile(documentFilename);
    if (fiIDFile.exists()
        && fiDocumentFile.exists()
        && fiIDFile.lastRead() > fiDocumentFile.lastRead())
    {
        successCallback.call(QJSValueList() << documentFilename);
        return;
    }
    std::map<QString, QString> params = {{"id", id}};
    network.appUserFetchORM(globalConfig()->projectID(),
                            appUserConfig()->loginEMail(),
                            appUserConfig()->loginToken(),
                            t0030_documents().getORMName(),
                            params,
                            [this, documentFilename, idFilename, successCallback, errorCallback](const QJsonDocument &jsonDoc) mutable
                            {
                                QJsonObject object(jsonDoc.object());
                                t0030_documents *t0030(new t0030_documents);
                                ORM2QJson o2j;
                                o2j.fromJson(object, *t0030);
                                {
                                    QFile documentFile(documentFilename);
                                    documentFile.open(QIODeviceBase::WriteOnly);
                                    documentFile.write(QByteArray::fromBase64(t0030->transfer_document_base64().toUtf8()));
                                }
                                {
                                    QFile idFile(idFilename);
                                    idFile.open(QIODeviceBase::WriteOnly);
                                    idFile.write(documentFilename.toUtf8());
                                }
                                successCallback.call(QJSValueList() << documentFilename);
                            },
                            [errorCallback](const QString &message) mutable
                            {
                                errorCallback.call(QJSValueList() << message);
                            }
                            );
}

void YACAPP::appUserFetchDocuments(const QString &needle,
                                   int offset,
                                   int limit,
                                   QJSValue successCallback,
                                   QJSValue errorCallback)
{
    bool clearTheModel(false);
    if (currentFetchDocumentsNeedle != needle)
    {
        clearTheModel = true;
        currentFetchDocumentsNeedle = needle;
    }
    std::map<QString, QString> params = {{"offset", QString::number(offset)}, {"limit", QString::number(limit)}};
    if (currentFetchDocumentsNeedle.size())
    {
        params["needle"] = currentFetchDocumentsNeedle;
    }
    network.appUserFetchORM(globalConfig()->projectID(),
                            appUserConfig()->loginEMail(),
                            appUserConfig()->loginToken(),
                            t0030_documents().getORMName(),
                            params,
                            [this, clearTheModel, limit, successCallback](const QJsonDocument &jsonDoc) mutable
                            {
                                if (clearTheModel)
                                {
                                    documentsModel.clear();
                                }
                                QJsonObject object(jsonDoc.object());
                                QJsonArray documents(object["documents"].toArray());
                                for (int i(0); i < documents.size(); ++i)
                                {
                                    QJsonObject document(documents[i].toObject());
                                    t0030_documents *t0030(new t0030_documents);
                                    ORM2QJson o2j;
                                    o2j.fromJson(document, *t0030);
                                    DEFAULT_LOG(QString("document_name: ") + t0030->document_name());
                                    documentsModel.append(t0030);
                                }
                                if (documents.size() < limit)
                                {
                                    documentsModel.lastObjectFetched();
                                }
                                successCallback.call(QJSValueList());
                            },
                            [errorCallback](const QString &message) mutable
                            {
                                errorCallback.call(QJSValueList() << message);
                            }
                            );
}

void YACAPP::appUserPostDocument(const QUrl fileUrl,
                                 const QString niceFilename,
                                 const QStringList keywords,
                                 QJSValue successCallback,
                                 QJSValue errorCallback)
{
    QFile file(fileUrl.toLocalFile());
    if (!file.exists())
    {
        errorCallback.call(QJSValueList() << "file not found");
        return;
    }
    file.open(QIODeviceBase::ReadOnly);
    t0030_documents *t0030(new t0030_documents);
    QByteArray fileContent(file.readAll());
    QString data(fileContent.toBase64());
    t0030->generateID();
    QByteArray comma_separated_catchphrases;
    for (auto const &cp: keywords)
    {
        if (comma_separated_catchphrases.size())
        {
            comma_separated_catchphrases += ",";
        }
        comma_separated_catchphrases += cp.toUtf8();
    }
    t0030->settransfer_comma_separated_catchphrases_base64(comma_separated_catchphrases.toBase64());
    t0030->settransfer_document_base64(data);
    t0030->setcreated_datetime(QDateTime::currentDateTime());
    QFileInfo fileInfo(fileUrl.toLocalFile());
    t0030->setdocument_name(niceFilename.size() ? niceFilename : fileInfo.baseName());
    t0030->setdocument_type(fileInfo.completeSuffix());
    t0030->setdocument_size(fileInfo.size());
    t0030->setdocument_version(1);
    t0030->setdocument_id(ExtUuid::generateUuid());
    network.appUserPostORM(globalConfig()->projectID(),
                           appUserConfig()->loginEMail(),
                           appUserConfig()->loginToken(),
                           *t0030,
                           [successCallback](const QJsonDocument &jsonDoc) mutable
                           {
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
                                         for (int i(0); i < appointments.size(); ++i)
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
    appUserFetchORM(t0021_right_group().getORMName(),
                    {},
                    rightGroupsModel,
                    successCallback,
                    errorCallback);
}

void YACAPP::appUserInsertOrUpdateRightGroup(const QString &id, const QString &name, const bool automatic, const QString access_code,
                                             const bool request_allowed,
                                             const bool visible_for_non_members,
                                             QJSValue successCallback, QJSValue errorCallback)
{
    t0021_right_group *t0021(new t0021_right_group);
    t0021->setid(id);
    t0021->setname(name);
    t0021->setautomatic(automatic);
    t0021->setaccess_code(access_code);
    t0021->setrequest_allowed(request_allowed);
    t0021->setvisible_for_non_members(visible_for_non_members);
    network.appUserPostORM(globalConfig()->projectID(),
                           appUserConfig()->loginEMail(),
                           appUserConfig()->loginToken(),
                           *t0021,
                           [t0021, this, successCallback](const QJsonDocument &jsonDoc) mutable
                           {
                               QJsonObject object(jsonDoc.object());
                               t0021->setid(object["id"].toString());
                               rightGroupsModel.append(t0021);
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

void YACAPP::appUserFetchRightGroupRights(const QString &id,
                                          QJSValue successCallback,
                                          QJSValue errorCallback)
{
    network.appUserFetchORM(globalConfig()->projectID(),
                            appUserConfig()->loginEMail(),
                            appUserConfig()->loginToken(),
                            "t0023_right2rightgroup",
                            {{tableFields.right_group_id, id}},
                            [this, successCallback](const QJsonDocument &jsonDoc) mutable
                            {
                                QJsonObject object(jsonDoc.object());
                                QJsonArray array(object["t0023_right2rightgroup"].toArray());
                                m_currentFetchedIds.clear();
                                for (int i(0); i < array.size(); ++i)
                                {
                                    t0023_right2rightgroup t0023;
                                    orm2json.fromJson(array[i].toObject(), t0023);
                                    m_currentFetchedIds.push_back(QString::number(t0023.right_number()));
                                }
                                emit currentFetchedIdsChanged();
                                successCallback.call(QJSValueList());
                            },
                            [errorCallback](const QString &message) mutable
                            {
                                errorCallback.call(QJSValueList() << message);
                            });
}

void YACAPP::appUserFetchRightGroupMember(const QString &right_group_id,
                                          QJSValue successCallback,
                                          QJSValue errorCallback)
{
    network.appUserFetchRightGroupMember(globalConfig()->projectID(),
                                         appUserConfig()->loginEMail(),
                                         appUserConfig()->loginToken(),
                                         right_group_id,
                                         [this, successCallback](const QJsonDocument &jsonDoc) mutable
                                         {
                                             selectedProfilesModel.clear();
                                             QJsonObject object(jsonDoc.object());
                                             QJsonArray member(object["member"].toArray());
                                             for (int i(0); i < member.size(); ++i)
                                             {
                                                 QJsonObject m(member[i].toObject());
                                                 ProfileObject *po(new ProfileObject);
                                                 po->fromJSON(m);
                                                 selectedProfilesModel.append(po);
                                             }
                                             successCallback.call(QJSValueList());
                                         },
                                         [errorCallback](const QString &message) mutable
                                         {
                                             errorCallback.call(QJSValueList() << message);
                                         }
                                         );
}

void YACAPP::appUserInsertOrUpdateRightGroup2AppUser(const QString &id,
                                                     const QString &right_group_id,
                                                     const QString &appuser_id,
                                                     const QDateTime &requested_datetime,
                                                     const QDateTime &approved_datetime,
                                                     const QString &approved_appuser_id,
                                                     const QDateTime &denied_datetime,
                                                     const QString &denied_appuser_id,
                                                     QJSValue successCallback,
                                                     QJSValue errorCallback)
{
    t0022_right_group2appuser t0022;
    t0022.setid(id);
    t0022.setright_group_id(right_group_id);
    t0022.setappuser_id(appuser_id);
    t0022.setrequested_datetime(requested_datetime);
    t0022.setapproved_datetime(approved_datetime);
    t0022.setapproved_appuser_id(approved_appuser_id);
    t0022.setdenied_datetime(denied_datetime);
    t0022.setdenied_appuser_id(denied_appuser_id);
    network.appUserPostORM(globalConfig()->projectID(),
                           appUserConfig()->loginEMail(),
                           appUserConfig()->loginToken(),
                           t0022,
                           [successCallback](const QJsonDocument &jsonDoc) mutable
                           {
                               QJsonObject object(jsonDoc.object());
                               successCallback.call(QJSValueList() << object["message"].toString());
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
                                   for (int i(0); i < spaces.size(); ++i)
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
                                        const bool request_allowed,
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
                                       request_allowed,
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
                                      appUserConfig()->setFstname(profile[tableFields.fstname].toString() + "anna");
                                      appUserConfig()->setSurname(profile[tableFields.surname].toString());
                                      appUserConfig()->setVisibleName(profile[tableFields.visible_name].toString());
                                      appUserConfig()->setProfileImageId(profile[tableFields.image_id].toString());
                                      appUserConfig()->setSearchingExactlyAllowed(profile[tableFields.searching_exactly_allowed].toBool());
                                      appUserConfig()->setSearchingFuzzyAllowed(profile[tableFields.searching_fuzzy_allowed].toBool());
                                      QString s(profile[tableFields.color].toString());
                                      DEFAULT_LOG(QString("color: ") +s);
                                      appUserConfig()->setColor(coalesce(profile[tableFields.color].toString(), "orange"));
                                      DEFAULT_LOG(QString("color: ") + appUserConfig()->color().name());
                                      appUserConfig()->setMessageFontColor(coalesce(profile[tableFields.message_font_color].toString(), "black"));
                                      successCallback.call(QJSValueList());
                                  },
                                  [this, errorCallback](const QString &message) mutable
                                  {
                                      if (message == "not logged in")
                                      {
                                          appUserLogout();
                                      }
                                      errorCallback.call(QJSValueList() << message);
                                  });
}

void YACAPP::minimizeMenue()
{
    setSuperMenueMinimized(true);
    emit minimizeMenueSignal();
}

void YACAPP::restoreMenue()
{
    setSuperMenueMinimized(false);
    emit restoreMenueSignal();
}

TransmissionTracker *YACAPP::tracker(const QString &uuid) const
{
    return network.tracker(uuid);
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
                                           QJsonArray spaceRequests(object["spaceRequests"].toArray());
                                           for (int i(0); i < spaceRequests.size(); ++i)
                                           {
                                               const QJsonObject spaceRequest(spaceRequests[i].toObject());
                                               SpaceRequestObject *sro(new SpaceRequestObject);
                                               sro->fromJSON(spaceRequest);
                                               NewsObject *no(new NewsObject);
                                               sro->to(*no);
                                               QString appuserId(sro->appuser_id());
                                               spaceRequestsModel.append(sro);
                                               newsModel.append(no);

                                               if (!knownProfilesModel.contains(appuserId))
                                               {
                                                   network.appUserFetchProfile(globalConfig()->projectID(),
                                                                               appUserConfig()->loginEMail(),
                                                                               appUserConfig()->loginToken(),
                                                                               appuserId,
                                                                               [this](const QJsonDocument &jsonDoc) mutable
                                                                               {
                                                                                   QJsonObject profile(jsonDoc.object());
                                                                                   ProfileObject *po(new ProfileObject);
                                                                                   po->fromJSON(profile);
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
                                           }

                                           QJsonArray messages(object["messages"].toArray());
                                           for (int i(0); i < messages.size(); ++i)
                                           {
                                               const QJsonObject message(messages[i].toObject());
                                               QByteArray content_base64(message["content_base64"].toString().toUtf8());
                                               QByteArray content;
                                               if (content_base64.contains("-"))
                                               {
                                                   content = content_base64;
                                               }
                                               else
                                               {
                                                   content = QByteArray::fromBase64(content_base64);
                                               }
                                               QString toId(message["to_id"].toString());
                                               QString senderId(message["sender_id"].toString());
                                               QString deleted_datetimeString(message[tableFields.deleted_datetime].toString());
                                               QDateTime deleted_datetime(QDateTime::fromString(deleted_datetimeString, Qt::DateFormat::ISODateWithMs));
                                               MessageObject *mo(new MessageObject(message["id"].toString(),
                                                                                   senderId,
                                                                                   toId,
                                                                                   QDateTime::currentDateTime(),
                                                                                   QDateTime::currentDateTime(),
                                                                                   deleted_datetime,
                                                                                   content,
                                                                                   false,
                                                                                   appUserConfig()->getPrivateKey()));
                                               bool inserted(false);
                                               bool updated(false);
                                               localStorage->upsertMessage(*mo, inserted, updated);
                                               if (inserted)
                                               {
                                                   if (mo->deleted_datetime().isNull() && !mo->read())
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
                                                                                               po->fromJSON(profile);
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
                                                   }
                                               }
                                               if (messagesModel.profileId() == senderId)
                                               {
                                                   if (mo->deleted_datetime().isValid())
                                                   {
                                                       messagesModel.removeById(mo->id());
                                                   }
                                                   else
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

void YACAPP::sendMessage(const QString &profileId,
                         const QString &text
                         , QStringList images
                         , QStringList imagesWidhts
                         , QStringList imagesHeighs)
{
    DEFAULT_LOG("sending message");
    QJsonArray imageArray;
    for (int i(0); i < images.size(); ++i)
    {
        DEFAULT_LOG(QString("imageid: ") + images[i]);
        DEFAULT_LOG(QString("width: ") + imagesWidhts[i]);
        DEFAULT_LOG(QString("height: ") + imagesHeighs[i]);
        QJsonObject image;
        image["imageid"] = images[i];
        image["width"] = imagesWidhts[i].toInt();
        image["height"] = imagesHeighs[i].toInt();
        imageArray.append(image);
    }
    QJsonObject jsonContent;
    jsonContent["text"] = text;
    jsonContent["images"] = imageArray;
    QJsonDocument document(jsonContent);
    MessageObject *mo(new MessageObject(helper.generateUuid(),
                                        appUserConfig()->id(),
                                        profileId,
                                        QDateTime::currentDateTime(),
                                        QDateTime::currentDateTime(),
                                        QDateTime(),
                                        document.toJson(),
                                        false,
                                        appUserConfig()->getPrivateKey()));
    messagesModel.append(mo);
    bool inserted(false);
    bool updated(false);
    localStorage->upsertMessage(*mo, inserted, updated);

    ProfileObject &receiver(knownProfilesModel.getById(profileId));

    network.appUserStoreMessage(globalConfig()->projectID(),
                                appUserConfig()->loginEMail(),
                                appUserConfig()->loginToken(),
                                mo->id(),
                                mo->receiverId(),
                                mo->encryptToBase64(receiver.public_key_base64()),
                                [](const QString &message)
                                {
                                    Q_UNUSED(message);
                                },
                                [](const QString &message)
                                {
                                    Q_UNUSED(message);
                                });
}

QString YACAPP::storeMessageImage(const QString &imageFilename, int widht, int height)
{
    t0028_message_images t0028;
    t0028.setid(helper.generateUuid());
    QFile file(imageFilename);
    file.rename(getUuidCacheImageFilename(t0028.id()));
    if (!file.open(QFile::ReadOnly))
    {
        return "";
    }
    QByteArray imageData(file.readAll().toBase64());
    t0028.setapp_id(globalConfig()->projectID());
    t0028.setcreater_id(appUserConfig()->id());
    t0028.setwidth(widht);
    t0028.setheight(height);
    t0028.settransfer_image_base64(imageData);
    DEFAULT_LOG(QString("imageData.size(): ") + QString::number(imageData.size()));
    network.appUserPostORM(globalConfig()->projectID(),
                           appUserConfig()->loginEMail(),
                           appUserConfig()->loginToken(),
                           t0028,
                           [](const QJsonDocument &document){
                               Q_UNUSED(document)
                           },
                           [](const QString &message){
                               Q_UNUSED(message)
                           });
    return t0028.id();
}

void YACAPP::deleteMessage(const QString &messageId)
{
    QUrlQuery query;
    query.addQueryItem("id", messageId);

    network.yacappServerDelete(tableNames.t0007_messages,
                               query,
                               globalConfig()->projectID(),
                               appUserConfig()->loginEMail(),
                               appUserConfig()->loginToken(),
                               [this, messageId](const QJsonDocument &document){
                                   Q_UNUSED(document)
                                   messagesModel.removeById(messageId);
                                   localStorage->deleteMessage(messageId);
                               },
                               [](const QString &message){
                                   Q_UNUSED(message)
                               });
}

void YACAPP::deleteAllMyMessages()
{
    network.appUserDeleteAllMyMessages(globalConfig()->projectID(),
                                       appUserConfig()->loginEMail(),
                                       appUserConfig()->loginToken(),
                                       [this](const QJsonDocument &document){
                                           Q_UNUSED(document)
                                           messagesModel.clear();
                                           localStorage->deleteAllMessages();
                                           ONLY_DESKTOP_LOG("all messages deleted");
                                       },
                                       [](const QString &message){
                                           Q_UNUSED(message)
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

void YACAPP::addProfileToSelectedProfiles(const QString &id)
{
    ProfileObject *po(searchProfilesModel.getCopyById(id));
    selectedProfilesModel.append(po);
}

void YACAPP::removeFromSelectedProfiles(const QString &id)
{
    selectedProfilesModel.removeById(id);
}

void YACAPP::switchLanguage(const QString &language)
{
    const QString baseName = "yacapp_" + language;
    if (translator.load(":/i18n/" + baseName)) {
        QCoreApplication::installTranslator(&translator);
    }

}

#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
#endif

void YACAPP::goTakePhoto(bool squared, bool circled, QJSValue target)
{
#ifdef Q_OS_ANDROID
    if (QtAndroidPrivate::checkPermission("android.permission.CAMERA").result() == QtAndroidPrivate::Denied)
    {

    }
#endif
    emit takePhoto(squared, circled, target);
}

QString YACAPP::getNewProfileImageFilename()
{
    QString newProfileImageFilename(constants.getWriteablePath(globalConfig()->projectID()) + "newProfileImage.jpg");
    DEFAULT_LOG_VARIABLE(newProfileImageFilename)
    return newProfileImageFilename;
}

QString YACAPP::getCacheImageFilename() const
{
    QString cacheImageFilename(constants.getCachePath(globalConfig()->projectID()) + helper.generateUuid() + ".jpg");
    DEFAULT_LOG_VARIABLE(cacheImageFilename)
    return cacheImageFilename;
}

QString YACAPP::getUuidCacheImageFilename(const QString &uuid) const
{
    QString cacheImageFilename(constants.getCachePath(globalConfig()->projectID()) + uuid + ".jpg");
    DEFAULT_LOG_VARIABLE(cacheImageFilename)
    return cacheImageFilename;
}

QString YACAPP::getCacheFile(const QString &fnameWithExtension) const
{
    QString cacheFilename(constants.getCachePath(globalConfig()->projectID()) + fnameWithExtension);
    DEFAULT_LOG_VARIABLE(cacheFilename)
    return cacheFilename;
}

#define STRING_FROM_JSON_OBJECT(field, jsonObject) \
QString field(jsonObject[#field].toString())

    void YACAPP::fetchProfileAndUpsertKnownProfiles(const QString &profileId)
{
    network.appUserFetchProfile(globalConfig()->projectID(),
                                appUserConfig()->loginEMail(),
                                appUserConfig()->loginToken(),
                                profileId,
                                [this](const QJsonDocument &jsonDoc) mutable
                                {
                                    QJsonObject profile(jsonDoc.object());
                                    STRING_FROM_JSON_OBJECT(id, profile);
                                    STRING_FROM_JSON_OBJECT(image_id, profile);
                                    STRING_FROM_JSON_OBJECT(visible_name, profile);
                                    STRING_FROM_JSON_OBJECT(public_key_base64, profile);
                                    if (knownProfilesModel.contains(id))
                                    {
                                        ProfileObject &po(knownProfilesModel.getById(id));
                                        po.setProfileImageId(image_id);
                                        po.setVisibleName(visible_name);
                                        po.setPublic_key_base64(public_key_base64);
                                        localStorage->upsertKnownContact(po);
                                    }
                                    return;

                                    ProfileObject *po(new ProfileObject);
                                    po->setId(id);
                                    po->setVisibleName(visible_name);
                                    po->setProfileImageId(image_id);
                                    po->setPublic_key_base64(public_key_base64);
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
    DEFAULT_LOG(QString("YACAPP: ") + deviceToken);
    if (!appUserConfig()->loggedIn())
    {
        DEFAULT_LOG("not logged in");
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
