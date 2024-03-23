import QtQuick 2.15
import "../items"

Item
{
    property double closedCenterX: 0
    property double closedCenterY: 0
    property double openedCenterY: 0
    property double openedCenterX: 0
    id: theMultiMenueButton
    height: 0
    width: parent.width

    YACTextEditWithBackground
    {
        id: buttonHeightCalc
        visible: false
        font.pixelSize: Constants.defaultFontPixelSize * Constants.x4largerTextFactor
    }

    property double closedXOffset: inactiveButtonSize * 0.5
    property int aniDuration: 200
    property double extraSize: 4
    property double activeButtonSize: buttonHeightCalc.height + extraSize // width / 7
    property double inactiveButtonSize: activeButtonSize // width / 13
    property bool isOpen: false
    property bool behaviorsActive: false
    property alias model: theItemsRepeater.model
    signal clicked(string caption)

    property bool hide: false
    onHideChanged: hide ? opacity = 0 : opacity = 1
    Behavior on opacity
    {
        NumberAnimation {
            duration: theMultiMenueButton.aniDuration
        }
    }

    PauseAnimation {
        id: showAni
        duration: 200
        onFinished:
        {
            behaviorsActive = true
            openCloseButton.x = theMultiMenueButton.closedCenterX - openCloseButton.width / 2
            openCloseButton.y = theMultiMenueButton.closedCenterY - openCloseButton.height / 2 - extraSize / 2
        }
    }

    function moveIn()
    {
        showAni.start()
    }
    function moveOut()
    {
        console.log("move out")
        openCloseButton.x = -theMultiMenueButton.width
    }

    function open()
    {
        console.log("open")
        isOpen = true
        openCloseButton.width = inactiveButtonSize
        openCloseButton.x = theMultiMenueButton.openedCenterX - openCloseButton.width / 2
        openCloseButton.y = theMultiMenueButton.openedCenterY - openCloseButton.height / 2 -  extraSize / 2

        for (var i = 0; i < theItemsRepeater.count; ++i)
        {
            theItemsRepeater.itemAt(i).open()
        }
    }
    function close()
    {
        console.log("close")
        isOpen = false
        openCloseButton.width = activeButtonSize
        openCloseButton.x = theMultiMenueButton.closedCenterX - openCloseButton.width / 2
        openCloseButton.y = theMultiMenueButton.closedCenterY - openCloseButton.height / 2 - extraSize / 2

        for (var i = 0; i < theItemsRepeater.count; ++i)
        {
            theItemsRepeater.itemAt(i).close()
        }
    }
    function toggle()
    {
        if (isOpen)
        {
            close()
        }
        else
        {
            open()
        }
    }

    Repeater
    {
        id: theItemsRepeater
        MultiMenueItem
        {
            visible: false
            x: theMultiMenueButton.closedCenterX
            y: theMultiMenueButton.closedCenterY - extraSize / 2
            closedX: theMultiMenueButton.closedCenterX
            closedY: theMultiMenueButton.closedCenterY - extraSize / 2
            openedX: theMultiMenueButton.openedCenterX
            openedY: theMultiMenueButton.openedCenterY - openCloseButton.height / 2 - extraSize
//            anchors.centerIn: openCloseButton
            position: index
            maxPosition: theItemsRepeater.count
            activeButtonSize: theMultiMenueButton.activeButtonSize
            inactiveButtonSize: theMultiMenueButton.inactiveButtonSize
            caption: modelData.caption
            onClicked: {
                theMultiMenueButton.close()
                theMultiMenueButton.clicked(caption)
            }
        }
    }
    Rectangle
    {
        id: openCloseButton
        y: theMultiMenueButton.closedCenterY - openCloseButton.height / 2
        x: -2 * width
        radius: width / 2
        width: activeButtonSize
        height: width
        color: Constants.multiMenueConfig.color
        border.width: 1
        border.color: Constants.multiMenueConfig.borderColor
        YACImage
        {
            anchors.centerIn: parent
            width: parent.width * 0.7
            height: width
            source: theMultiMenueButton.isOpen ? "qrc:/images/images/close_menue_icon.svg" : "qrc:/images/images/menue_move_icon.svg"
        }
        Behavior on width {
            enabled: theMultiMenueButton.behaviorsActive
            NumberAnimation {
                duration: theMultiMenueButton.aniDuration
            }
        }
        Behavior on x {
            enabled: theMultiMenueButton.behaviorsActive
            NumberAnimation {
                duration: theMultiMenueButton.aniDuration
            }
        }
        Behavior on y {
            enabled: theMultiMenueButton.behaviorsActive
            NumberAnimation {
                duration: theMultiMenueButton.aniDuration
            }
        }
        MouseArea
        {
            anchors.fill: parent
            onClicked: theMultiMenueButton.toggle()
        }
    }

}
