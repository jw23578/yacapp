import QtQuick 2.15

Item
{
    id: theMultiMenueButton
    height: 1
    width: parent.width
    property double closedXOffset: inactiveButtonSize * 0.5
    property int aniDuration: 200
    property double activeButtonSize: width / 7
    property double inactiveButtonSize: width / 13
    property bool isOpen: false
    property bool positionLeft: true
    property bool behaviorsActive: false
    property alias model: theItemsRepeater.model

    PauseAnimation {
        id: showAni
        duration: 200
        onFinished:
        {
            behaviorsActive = true
            openCloseButton.x = positionLeft ? closedXOffset : theMultiMenueButton.width - activeButtonSize - closedXOffset
        }
    }

    Component.onCompleted: {
        showAni.start()
    }

    function open()
    {
        isOpen = true
        openCloseButton.width = inactiveButtonSize
        openCloseButton.x = (positionLeft ? inactiveButtonSize * 0.25 : theMultiMenueButton.width - inactiveButtonSize - inactiveButtonSize * 0.25) + (activeButtonSize - inactiveButtonSize) * 0.75
        openCloseButton.y = activeButtonSize / 2// -inactiveButtonSize -inactiveButtonSize * 0.25

        for (var i = 0; i < theItemsRepeater.count; ++i)
        {
            theItemsRepeater.itemAt(i).open()
        }
    }
    function close()
    {
        isOpen = false
        openCloseButton.width = activeButtonSize
        openCloseButton.x = positionLeft ? closedXOffset : theMultiMenueButton.width - activeButtonSize - closedXOffset
        openCloseButton.y = -activeButtonSize - theMultiMenueButton.closedXOffset

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
        model: 7
        MultiMenueItem
        {
            anchors.centerIn: openCloseButton
            position: index
            maxPosition: theItemsRepeater.count
            activeButtonSize: theMultiMenueButton.activeButtonSize
            inactiveButtonSize: theMultiMenueButton.inactiveButtonSize
        }
    }
    Rectangle
    {
        id: openCloseButton
        y: -height - theMultiMenueButton.closedXOffset
        x: theMultiMenueButton.positionLeft ? -width : theMultiMenueButton.width + width
        radius: width / 2
        width: activeButtonSize
        height: width
        color: "white"
        border.width: 1
        border.color: "grey"
        Text
        {
            anchors.centerIn: parent
            text: "+"
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
