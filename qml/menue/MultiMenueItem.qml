import QtQuick 2.15
import "../items"

Item
{
    id: theMultiMenueItem
    width: 0
    height: 0
    property double openedX: 0
    property double openedY: 0
    property double closedX: 0
    property double closedY: 0
    signal clicked(string caption)
    property alias color: menueOption1.color
    property alias caption: theCaption.text
    property double activeButtonSize: 0
    property double inactiveButtonSize: 0
    property double myActiveSize: activeButtonSize * 0.8
    property int position: 0
    property int maxPosition: 0
    property int aniDuration: Constants.fastAnimationDuration

    SequentialAnimation
    {
        id: openAnimation
        PauseAnimation {
            duration: (maxPosition - position) * Constants.fastAnimationDuration / 5
        }
        ParallelAnimation
        {
            NumberAnimation
            {
                target: theMultiMenueItem
                property: "y"
                to: openedY
            }
            NumberAnimation
            {
                target: menueOption1
                property: "width"
                to: activeButtonSize
                duration: aniDuration
            }
            SmoothedAnimation
            {
                target: menueOption1
                property: "y"
                to: (position + 1) * 1.2 * -activeButtonSize
                duration: aniDuration
            }
        }

        PauseAnimation {
            duration: (position * 1.2) * Constants.fastAnimationDuration / 5
        }
        ParallelAnimation
        {
            NumberAnimation
            {
                target: theTextRectangle
                property: "x"
                to: activeButtonSize / 1.5
                duration: aniDuration
            }
            NumberAnimation
            {
                target: theTextRectangle
                property: "width"
                to: theCaption.contentWidth + theTextRectangle.radius * 2
                duration: aniDuration
            }
        }
    }

    SequentialAnimation
    {
        id: closeAnimation
        PauseAnimation {
            duration: position * Constants.fastAnimationDuration / 5
        }

        ParallelAnimation
        {
            NumberAnimation
            {
                target: theMultiMenueItem
                property: "y"
                to: closedY
            }
            NumberAnimation
            {
                target: theTextRectangle
                property: "width"
                to: inactiveButtonSize
                duration: aniDuration
            }
            NumberAnimation
            {
                target: theTextRectangle
                property: "x"
                to: - inactiveButtonSize / 2
                duration: aniDuration
            }
            SmoothedAnimation
            {
                target: menueOption1
                property: "y"
                to: -theMultiMenueItem.inactiveButtonSize / 2
                duration: theMultiMenueItem.aniDuration
            }
        }
        ParallelAnimation
        {
            NumberAnimation
            {
                target: menueOption1
                property: "width"
                to: inactiveButtonSize
                duration: aniDuration
            }
        }
    }
    SequentialAnimation
    {
        id: closeani2
    }
    function open()
    {
        visible = true
        closeAnimation.stop()
        openAnimation.start()
    }

    function close()
    {
        openAnimation.stop()
        closeAnimation.start()
    }
    Rectangle
    {
        id: theTextRectangle
        anchors.verticalCenter: menueOption1.verticalCenter
        x: menueOption1.x
        width: inactiveButtonSize
        height: inactiveButtonSize
        radius: height / 2
        color: Constants.multiMenueConfig.itemColor
        clip: true
        YACText
        {
            id: theCaption
            anchors.centerIn: parent
            color: Constants.multiMenueConfig.fontColor
        }
        MouseArea
        {
            anchors.fill: parent
            onClicked: theMultiMenueItem.clicked(theCaption.text)
        }
    }
    Rectangle
    {
        id: menueOption1
        width: parent.inactiveButtonSize
        height: width
        anchors.horizontalCenter: parent.horizontalCenter
        y: - height / 2
        radius: width / 2
        Text
        {
            anchors.centerIn: parent
            text: position + 1
            color: Constants.multiMenueConfig.fontColor
        }
        color: Constants.multiMenueConfig.itemColor
        MouseArea
        {
            anchors.fill: parent
            onClicked: theMultiMenueItem.clicked(theCaption.text)
        }
    }


}

