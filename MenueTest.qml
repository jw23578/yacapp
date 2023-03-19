import QtQuick 2.15
import "qml/items"

Item
{
    id: theMenu
    anchors.fill: parent
    property int animationDuration: 400
    property int animationSlowdown: 30
    property int animactionVelocity: 600
    property int smallElemSize: parent.width / 6
    property int largeElemHeight: parent.height / 4.5
    property var captions: [qsTr("Messages"), qsTr("Worktime"), qsTr("Messages"), qsTr("Messages"), qsTr("Messages"), qsTr("Messages"), qsTr("Messages")]
    property var colors: ["red", "green", "lightgreen", "blue", "orange", "cyan", "yellow"]
    property bool isOpen: false
    function open()
    {
        isOpen = true
        waitBeforeSetClosed.stop()
        horizontalFlickable.y = 0
        horizontalFlickable.contentX = 0
        horizontalFlickable.interactive = false
        verticalFlickable.interactive = true
        verticalFlickable.contentHeight = theRepeater.count * largeElemHeight
        verticalFlickable.contentY = 0 // verticalFlickable.contentHeight - theMenu.height
        for(var i = 0; i < theRepeater.count; ++i)
        {
            var rect = theRepeater.itemAt(i)
            rect.y = i * largeElemHeight // (theRepeater.count - 1) * largeElemHeight - i * largeElemHeight
            rect.x = 0
            rect.width = parent.width
            rect.height = largeElemHeight
        }
    }

    PauseAnimation
    {
        id: waitBeforeSetClosed
        duration: animationDuration
        onFinished: isOpen = false
    }
    function close()
    {
        waitBeforeSetClosed.start()
        horizontalFlickable.y = theMenu.height - smallElemSize
        horizontalFlickable.contentX = 0
        horizontalFlickable.interactive = true
        verticalFlickable.interactive = false
        verticalFlickable.contentY = 0
        for(var i = 0; i < theRepeater.count; ++i)
        {
            var rect = theRepeater.itemAt(i)
            rect.y = 0 // theMenu.height - smallElemSize
            rect.x = i * smallElemSize
            rect.width = smallElemSize
            rect.height = smallElemSize
        }
    }

    Flickable
    {
        id: horizontalFlickable
        y: theMenu.height - smallElemSize
        width: parent.width
        height: parent.height
        contentWidth: theRepeater.count * smallElemSize
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
                model: 7
                id: theRepeater
                Rectangle
                {
                    x: width * index
                    y: 0 // theMenu.height - smallElemSize
                    width: smallElemSize
                    height: smallElemSize
                    color: colors[index]
                    YACText
                    {
                        text: captions[index]
                        font.pixelSize: Constants.defaultFontPixelSize * parent.height / smallElemSize
                        anchors.horizontalCenter: parent.horizontalCenter
                        y: smallElemSize / 2 - contentHeight / 2
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignHCenter
                    }
                    Behavior on x
                    {
                        SmoothedAnimation {
                            duration: animationDuration + index * animationSlowdown
                            velocity: animactionVelocity
                        }
                    }
                    Behavior on y
                    {
                        SmoothedAnimation {
                            duration: animationDuration + index * animationSlowdown
                            velocity: animactionVelocity
                        }
                    }
                    Behavior on width
                    {
                        SmoothedAnimation {
                            duration: animationDuration + index * animationSlowdown
                            velocity: animactionVelocity
                        }
                    }
                    Behavior on height
                    {
                        SmoothedAnimation {
                            duration: animationDuration + index * animationSlowdown
                            velocity: animactionVelocity
                        }
                    }
                }
            }
        }

    }

    Rectangle
    {
        width: 100
        height: 100
        color: "white"
        anchors.horizontalCenter: parent.horizontalCenter
        MouseArea
        {
            anchors.fill: parent
            onClicked: if (isOpen) {close()} else {open()}
        }
    }
}
