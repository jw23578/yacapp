import QtQuick 2.15
import "../items"

Item
{
    id: theMenu
    anchors.fill: parent
    signal openNews()
    signal openCustomMenue()
    signal openWorkTime()
    signal openAppointments();
    signal openSpaces()
    signal openRights()
    signal openProfile()
    signal openMessages()
    signal menueOpened()
    signal menueClosed()
    signal pleaseRegisterOrLogin(string wantedCaption)
    function actionSwitch(caption)
    {
        switch (caption)
        {
        case qsTr("Menue"): openCustomMenue();break;
        case qsTr("News"): openNews();break;
        case qsTr("Worktime"): openWorkTime();break;
        case qsTr("Appointments"): openAppointments();break;
        case qsTr("Spaces"): openSpaces();break;
        case qsTr("Rights"): openRights();break;
        case qsTr("Profile"): openProfile();break;
        case qsTr("Messages"): openMessages();break;
        }
    }

    Connections
    {
        target: yacApp
        function onSuperMenueChanged()
        {
            fillMenueItems()
        }
    }
    property int updateTrigger: yacApp.updateTrigger
    onUpdateTriggerChanged: fillMenueItems()

    property int menueItemsLength: 0
    property var menueItems: []
    function fillMenueItems()
    {
        theRepeater.model = null
        menueItems = []
        for (var i = 0; i < allMenueItems.length; ++i)
        {
            console.log(i)
            console.log(allMenueItems[i].menueEnabled)
            if (allMenueItems[i].menueEnabled)
            {
                menueItems.push(allMenueItems[i])
            }
        }
        theRepeater.model = menueItems
        menueItemsLength = menueItems.length
    }

    property var allMenueItems: [
        {menueEnabled: true,
            caption: qsTr("Menue"),
            iconUrl: "",
            loginNeeded: false},
        {menueEnabled: yacApp.globalConfig.appUserEnabled,
            caption: qsTr("Profile"),
            iconUrl: "",
            loginNeeded: true},
        {menueEnabled: yacApp.globalConfig.appUserEnabled && yacApp.globalConfig.appUserMessagesEnabled,
            caption: qsTr("Messages"),
            iconUrl: "",
            loginNeeded: true},
        {menueEnabled: yacApp.globalConfig.appUserNewsEnabled,
            caption: qsTr("News"),
            iconUrl: "",
            loginNeeded: true},
        {menueEnabled: yacApp.globalConfig.appUserEnabled && yacApp.globalConfig.appUserWorktimeEnabled,
            caption: qsTr("Worktime"),
            iconUrl: "qrc:/images/images/appointments_menue_icon.svg",
            loginNeeded: true},
        {menueEnabled: yacApp.globalConfig.appUserEnabled && yacApp.globalConfig.appUserAppointmentsEnabled,
            caption: qsTr("Appointments"),
            iconUrl: "",
            loginNeeded: true},
        {menueEnabled: yacApp.globalConfig.appUserEnabled && yacApp.globalConfig.appUserSpacesEnabled,
            caption: qsTr("Spaces"),
            iconUrl: "",
            loginNeeded: true},
        {menueEnabled: yacApp.globalConfig.appUserEnabled && yacApp.globalConfig.appUserRightsEnabled,
            caption: qsTr("Rights"),
            iconUrl: "",
            loginNeeded: true}
    ]
    property int animationDuration: 400
    property int animationSlowdown: 30
    property int animactionVelocity: 600
    property double smallElemWidth: Constants.appTotalWidth / Constants.mainMenueConfig.smallElemCount
    property double smallElemHeight: Constants.appTotalHeight * Constants.mainMenueConfig.smallElemHeightFactor
    property double largeElemHeight: Constants.appTotalHeight * Constants.mainMenueConfig.largeElemHeightFactor
    function open()
    {
        menueOpened()
        Constants.superMenueOpen = true
        waitBeforeSetClosed.stop()
        horizontalFlickable.y = 0
        horizontalFlickable.contentX = 0
        horizontalFlickable.interactive = false
        verticalFlickable.interactive = true
        verticalFlickable.contentHeight = menueItemsLength * largeElemHeight
        verticalFlickable.contentY = 0
        theOpenBar.y = theOpenBar.height
        for(var i = 0; i < theRepeater.count; ++i)
        {
            var rect = theRepeater.itemAt(i)
            rect.y = i * largeElemHeight
            rect.x = 0
            rect.goLarge()
        }
    }

    PauseAnimation
    {
        id: waitBeforeSetClosed
        duration: animationDuration
        onFinished: Constants.superMenueOpen = false
    }
    property double openValue: 1 - horizontalFlickable.y / (theMenu.height - smallElemHeight)
    function close()
    {
        if (!Constants.superMenueOpen)
        {
            return
        }
        menueClosed()
        waitBeforeSetClosed.start()
        horizontalFlickable.y = theMenu.height - smallElemHeight
        horizontalFlickable.contentX = 0
        horizontalFlickable.interactive = true
        verticalFlickable.interactive = false
        verticalFlickable.contentY = 0
        theOpenBar.y = height - theMenu.smallElemHeight + theOpenBar.height
        for(var i = 0; i < theRepeater.count; ++i)
        {
            var rect = theRepeater.itemAt(i)
            rect.y = 0
            rect.x = i * smallElemWidth
            rect.goSmall()
        }
    }

    Flickable
    {
        id: horizontalFlickable
        y: theMenu.height - smallElemHeight
        width: parent.width
        height: parent.height
        contentWidth: menueItemsLength * smallElemWidth
        Behavior on y
        {
            SmoothedAnimation {
                duration: animationDuration
                velocity: animactionVelocity
            }
        }
        Behavior on contentX
        {
            SmoothedAnimation {
                duration: animationDuration
                velocity: animactionVelocity
            }
        }
        Flickable
        {
            id: verticalFlickable
            height: theMenu.height
            width: theMenu.width
            interactive: false
            contentHeight: height
            Behavior on contentY
            {
                SmoothedAnimation {
                    duration: animationDuration
                    velocity: animactionVelocity
                }
            }
            Repeater
            {
                id: theRepeater                
                SuperMenueElem
                {
                    text: modelData.caption
                    x: smallElemWidth * index
                    y: 0
                    animationDuration: theMenu.animationDuration
                    animationSlowdown: theMenu.animationSlowdown * 0
                    animactionVelocity: theMenu.animactionVelocity
                    onClicked: {
                        if (modelData.loginNeeded && !yacApp.appUserConfig.loggedIn)
                        {
                            theMenu.pleaseRegisterOrLogin(modelData.caption)
                            return
                        }
                        actionSwitch(modelData.caption)
                    }
                    iconUrl: modelData.iconUrl
                }
            }
        }
    }
    Rectangle
    {
        id: theOpenBar
        z: 1
        x: (parent.width - width) / 2
        y: parent.height - theMenu.smallElemHeight + height
        width: theMenu.smallElemWidth / 1.5
        height: theMenu.smallElemHeight / 14
        radius: height / 2
        color: "white"
        MouseArea
        {
            anchors.fill: parent
            anchors.margins: -theOpenBar.height
            onClicked: {
                theSuperMenue.close()
                if (Constants.superMenueOpen) {
                    theSuperMenue.close()
                } else {theSuperMenue.open()}
            }
        }
        Behavior on y
        {
            SmoothedAnimation {
                duration: animationDuration
                velocity: animactionVelocity
            }
        }
    }
    Component.onCompleted:
    {
        fillMenueItems()
        Helper.jsLog("theMenu.smallElemHeight: " + theMenu.smallElemHeight)
    }
}
