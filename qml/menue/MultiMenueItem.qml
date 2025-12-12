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
    property double buttonSize: 0
    property double myActiveSize: buttonSize * 0.8
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
                to: buttonSize
                duration: aniDuration
            }
            SmoothedAnimation
            {
                target: menueOption1
                property: "y"
                to: (position + 1) * 1.2 * -buttonSize
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
                to: buttonSize / 1.5
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
                to: buttonSize
                duration: aniDuration
            }
            NumberAnimation
            {
                target: theTextRectangle
                property: "x"
                to: - buttonSize / 2
                duration: aniDuration
            }
            SmoothedAnimation
            {
                target: menueOption1
                property: "y"
                to: -theMultiMenueItem.buttonSize / 2
                duration: theMultiMenueItem.aniDuration
            }
        }
        ParallelAnimation
        {
            NumberAnimation
            {
                target: menueOption1
                property: "width"
                to: buttonSize
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
        width: buttonSize
        height: buttonSize
        radius: height / 2
        border.width: Constants.buttonBorderWidth
        border.color: Constants.buttonPrimaryBorderColor
        color: Constants.buttonPrimaryColor
        clip: true
        YACText
        {
            id: theCaption
            anchors.centerIn: parent
            color: Constants.textPrimaryColor
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
        width: parent.buttonSize
        height: width
        anchors.horizontalCenter: parent.horizontalCenter
        y: - height / 2
        radius: width / 2
        border.width: Constants.buttonBorderWidth
        border.color: Constants.buttonPrimaryBorderColor
        Text
        {
            anchors.centerIn: parent
            text: position + 1
            color: Constants.textPrimaryColor
        }
        color: Constants.buttonPrimaryColor
        MouseArea
        {
            anchors.fill: parent
            onClicked: theMultiMenueItem.clicked(theCaption.text)
        }
    }


}

