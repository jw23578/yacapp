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
import "apppages/profile"


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

    YACDefaultMenue
    {
        z: 1
        id: theRealMenue
        visible: theMenue.type === "" || theMenue.type === "default"
        stackView: theStackView
        theMenue: yacApp.getMenueConfig(config.menueFilename)
        onPleaseUpdate: theMenue = yacApp.getMenueConfig(config.menueFilename)
    }
    function goProfile()
    {
        theRealMenue.close()
        yacApp.fetchMyProfile(function(message) {
            profileLoader.open()
        },
        function(message)
        {
            CPPQMLAppAndConfigurator.badMessage(qsTr("Could not fetch Profile, please try again later."), null, null);
        }
        )
    }

    SuperMenue
    {
        z: currentOpenedLoader == null || Constants.superMenueOpen ? 1 : 0
        id: theSuperMenue
        onPleaseRegisterOrLogin:
        {
            tokenLoginLoader.wantedCaption = wantedCaption
            tokenLoginLoader.open()
        }
        onMenueOpened:
        {
            theRealMenue.close()
            if (currentOpenedLoader != null)
            {
                currentOpenedLoader.close()
            }
        }
        onOpenCustomMenue:
        {
            if (currentOpenedLoader != null)
            {
                currentOpenedLoader.close()
            }

            theRealMenue.toggle()
        }
        onOpenNews:
        {
            theRealMenue.close()
            newsPageLoader.open()
        }
        onOpenMessages:
        {
            theRealMenue.close()
            knownProfilesLoader.open()
        }
        onOpenWorkTime:
        {
            theRealMenue.close()
            worktimeLoader.open()
        }
        onOpenAppointments: {
            theRealMenue.close()
            yacApp.appUserFetchAppointments(function(message)
            {
                appointmensLoader.open()
            },
            function(message)
            {
                CPPQMLAppAndConfigurator.badMessage(qsTr("could not load appointments, please try again later. ") + message, null, null)
            }
            )
        }
        onOpenProfile: goProfile()
        onOpenRights:
        {
            theRealMenue.close()
            yacApp.appUserFetchRightGroups(function(message) {
                rightgroupsLoader.open()
            },
            function(message) {
                CPPQMLAppAndConfigurator.badMessage(qsTr("Could not load Rightgroups, please try again later. ") + message, null, null)
            })

        }
        onOpenSpaces:
        {
            theRealMenue.close()
            yacApp.appUserFetchSpaces(function(message) {
                spacesLoader.open()
            },
            function(message) {
                CPPQMLAppAndConfigurator.badMessage(qsTr("Could not load Spaces, please try again later. ") + message, null, null)
            })
        }
    }

    PauseAnimation {
        duration: 200
        id: menueSwitchPause
        onStopped: theRealMenue.theMenue = yacApp.getMenueConfig(theStackView.currentItem.config.menueFilename)
    }
    Connections
    {
        target: yacApp
        function onMinimizeMenueSignal()
        {
            contentItem.height = contentItem.parent.height

        }
        function onRestoreMenueSignal()
        {
            contentItem.height = contentItem.parent.height - theSuperMenue.smallElemHeight
        }
    }
    Item
    {
        z: 0
        id: contentItem
        width: parent.width
        height: parent.height - theSuperMenue.smallElemHeight
        Behavior on height
        {
            NumberAnimation
            {
                duration: Constants.fastAnimationDuration
            }
        }

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

                height: theItem.height - theSuperMenue.smallElemHeight - header.height // contentItem.height - header.height
                width: parent.width
                onCurrentItemChanged: {
                    header.headerConfig = currentItem.config.header
                    theItem.currentItemChanged(currentItem.config)
                    menueSwitchPause.start()
                }
                property int updateTrigger: yacApp.updateTrigger
                onUpdateTriggerChanged:
                {
                    Helper.jsLog("ontestChanged")
                    header.headerConfig = currentItem.config.header
                    theItem.currentItemChanged(currentItem.config)
                }
            }
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
                    onProfileClicked: goProfile()
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
            property string wantedCaption: ""
            theComponent: Component
            {
                id: tokenLogin
                YACUserTokenLogin
                {
                    onCloseClicked: tokenLoginLoader.close()
                    onLoginSuccessful:
                    {
                        tokenLoginLoader.close()
                        theSuperMenue.actionSwitch(tokenLoginLoader.wantedCaption)
                    }
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
                    onLogout: profileLoader.close()
                }
            }
        }
    }
}
