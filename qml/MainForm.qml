import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import com.yacapp.globalprojectconfig 1.0
import com.yacapp.parsedconfig 1.0
import com.yacapp.menueconfig 1.0
import "items"
import "apppages"
import "menue"
import "apppages/appointments"
import "apppages/worktimes"
import "apppages/rights"
import "apppages/spaces"
import "apppages/news"


Item
{
    id: theItem
    anchors.fill: parent
    focus: true
    property var currentOpenedLoader: null
    property ParsedConfig config: yacApp.mainConfig
    signal currentItemChanged(ParsedConfig config)
    function openFilename(filename)
    {
        theStackView.push("SuperForm.qml", {
                              "config": yacApp.getConfig(filename),
                              "stackView": theStackView,
                              "theMenue": theRealMenue
                          })
    }

    PauseAnimation {
        duration: 200
        id: menueSwitchPause
        onStopped: theRealMenue.theMenue = yacApp.getMenueConfig(theStackView.currentItem.config.menueFilename)
    }
    Item
    {
        id: contentItem
        width: parent.width
        height: parent.height - theSuperMenue.smallElemHeight
        Column
        {
            width: parent.width
            id: mainForm
            YACHeader
            {
                id: header
                width: parent.width
            }
            StackView
            {
                id: theStackView
                initialItem:
                    SuperForm
                {
                    config: theItem.config
                    stackView: theStackView
                    theMenue: theRealMenue
                }
                height: contentItem.height - header.height - footer.height
                width: parent.width
                onCurrentItemChanged: {
                    header.headerConfig = currentItem.config.header
                    footer.footerConfig = currentItem.config.footer
                    theItem.currentItemChanged(currentItem.config)
                    menueSwitchPause.start()
                }
            }

            YACFooter
            {
                id: footer
                minimumHeight: theRealMenue.openCloseButtonHeight
            }
        }
        BasePageLoader
        {
            id: newsPageLoader
            parentSuperMenue: theSuperMenue
            parentCurrentOpenedLoader: currentOpenedLoader
            onOpened: currentOpenedLoader = newsPageLoader
            theComponent: Component
            {
                NewsPage
                {
                }
            }
        }
        BasePageLoader
        {
            id: knownProfilesLoader
            loginNeeded: true
            parentSuperMenue: theSuperMenue
            parentCurrentOpenedLoader: currentOpenedLoader
            onOpened: currentOpenedLoader = knownProfilesLoader
            theComponent: Component
            {
                YACUserKnownProfiles
                {
                }
            }
        }

        BasePageLoader
        {
            id: appointmensLoader
            loginNeeded: true
            parentSuperMenue: theSuperMenue
            parentCurrentOpenedLoader: currentOpenedLoader
            onOpened: currentOpenedLoader = appointmensLoader
            theComponent: Component
            {
                AppUserAppointments
                {
                }
            }
        }
        BasePageLoader
        {
            id: tokenLoginLoader
            parentSuperMenue: theSuperMenue
            parentCurrentOpenedLoader: currentOpenedLoader
            onOpened: currentOpenedLoader = tokenLoginLoader
            theComponent: Component
            {
                id: tokenLogin
                YACUserTokenLogin
                {
                }
            }
        }
        BasePageLoader
        {
            id: registerVerifyLoginLoader
            parentSuperMenue: theSuperMenue
            parentCurrentOpenedLoader: currentOpenedLoader
            onOpened: currentOpenedLoader = registerVerifyLoginLoader
            theComponent: Component
            {
                YACUserRegisterVerifyLogin
                {
                }
            }
        }
        BasePageLoader
        {
            id: worktimeLoader
            loginNeeded: true
            parentSuperMenue: theSuperMenue
            parentCurrentOpenedLoader: currentOpenedLoader
            onOpened: currentOpenedLoader = worktimeLoader
            theComponent: Component
            {
                YACUserWorktimeState
                {
                }
            }
        }
        BasePageLoader
        {
            id: rightgroupsLoader
            loginNeeded: true
            parentSuperMenue: theSuperMenue
            parentCurrentOpenedLoader: currentOpenedLoader
            onOpened: currentOpenedLoader = rightgroupsLoader
            theComponent: Component
            {
                AppUserRightGroups
                {
                }
            }
        }
        BasePageLoader
        {
            id: spacesLoader
            loginNeeded: true
            parentSuperMenue: theSuperMenue
            parentCurrentOpenedLoader: currentOpenedLoader
            onOpened: currentOpenedLoader = spacesLoader
            theComponent: Component
            {
                AppUserSpaces
                {
                }
            }
        }

        BasePageLoader
        {
            id: profileLoader
            loginNeeded: true
            parentSuperMenue: theSuperMenue
            parentCurrentOpenedLoader: currentOpenedLoader
            onOpened: currentOpenedLoader = profileLoader
            theComponent: Component
            {
                YACUserProfile
                {
                }
            }
        }
    }

    YACDefaultMenue
    {
        id: theRealMenue
        visible: theMenue.type === "" || theMenue.type === "default"
        stackView: theStackView
        theMenue: yacApp.getMenueConfig(config.menueFilename)
    }

    YACImage
    {
        anchors.top: parent.top
        anchors.topMargin: parent.height * yacApp.globalConfig.logoOffsetYPerThousand / 1000.0
        anchors.left: parent.left
        anchors.leftMargin: parent.width * yacApp.globalConfig.logoOffsetXPerThousand / 1000.0
        visible: source != ""
        source: yacApp.globalConfig.logoUrl
        width: parent.width * yacApp.globalConfig.logoWidthPerThousand / 1000.0
        height: parent.height * yacApp.globalConfig.logoHeightPerThousand / 1000.0
        MouseArea
        {
            anchors.fill: parent
            onClicked: theStackView.pop(null)
        }
    }

    SuperMenue
    {
        id: theSuperMenue
        onMenueOpened: currentOpenedLoader.close()
        onOpenCustomMenue:
        {
            theRealMenue.toggle()
        }
        onOpenNews: newsPageLoader.open()
        onOpenMessages: knownProfilesLoader.open()
        onOpenWorkTime: worktimeLoader.open()
        onOpenAppointments: {
            if (!yacApp.appUserConfig.loggedIn)
            {
                yacApp.badMessage(qsTr("Please login first."), null, null);
                return
            }
            yacApp.appUserFetchAppointments(function(message)
            {
                appointmensLoader.open()
            },
            function(message)
            {
                yacApp.badMessage(qsTr("could not load appointments, please try again later. ") + message, null, null)
            }
            )
        }
        onOpenProfile: {
            if (yacApp.appUserConfig.loginToken == "")
            {
                tokenLoginLoader.open()
            }
            else
            {
                yacApp.fetchMyProfile(function(message) {
                    profileLoader.open()
                },
                function(message)
                {
                    yacApp.badMessage(qsTr("Could not fetch Profile, please try again later."), null, null);
                }
                )
            }
        }
        onOpenRights: rightgroupsLoader.open()
        onOpenSpaces: spacesLoader.open()
    }


    Loader
    {
        id: appUserProfileLoader
        anchors.fill: parent
        active: yacApp.globalConfig.appUserEnabled
        source: "qrc:/qml/apppages/AppUserProfileIcon.qml"
    }

    MultiMenueButton
    {
        id: theMultiMenueButton
        anchors.left: parent.left
        y: parent.height - theSuperMenue.smallElemHeight
    }
}
