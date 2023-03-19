import QtQuick 2.15

Item
{
    id: theMultiMenueItem
    width: 0
    height: 0
    property alias color: menueOption1.color
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
                target: menueOption1
                property: "width"
                to: activeButtonSize
                duration: aniDuration
            }
            SmoothedAnimation
            {
                target: menueOption1
                property: "y"
                to: (position + 1) * -activeButtonSize - activeButtonSize/2
                duration: aniDuration
            }
        }

        PauseAnimation {
            duration: (maxPosition - position) * Constants.fastAnimationDuration / 5
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
                to: activeButtonSize * 1.5
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
            SmoothedAnimation
            {
                target: menueOption1
                property: "y"
                to: -theMultiMenueItem.inactiveButtonSize / 2
                duration: theMultiMenueItem.aniDuration
            }

        }
    }
    SequentialAnimation
    {
        id: closeani2
    }
    function open()
    {
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
        border.width: 1
        border.color: "grey"
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
            text: "T"
        }
        border.width: 1
        border.color: "grey"
    }


}

