import QtQuick 2.15
import "../items"

Rectangle
{
    id: theElem
    signal clicked()
    property alias iconUrl: icon.source
    property int animationDuration: 0
    property int animationSlowdown: 0
    property int animactionVelocity: 0
    property double smallElemWidth: Constants.appTotalWidth / Constants.mainMenueConfig.smallElemCount
    property double smallElemHeight: Constants.appTotalHeight * Constants.mainMenueConfig.smallElemHeightFactor
    property double largeElemHeight: Constants.appTotalHeight * Constants.mainMenueConfig.largeElemHeightFactor
    property alias text: theText.text
    color: Constants.mainMenueConfig.smallElemColor
    Behavior on color {
        ColorAnimation {
            duration: animationDuration + animationSlowdown
        }
    }

    x: 0
    y: 0
    width: smallElemWidth
    height: smallElemHeight
    function goLarge()
    {
        width = Constants.appTotalWidth
        height = largeElemHeight
        color = Constants.mainMenueConfig.largeElemColorRight
        leftRectangle.opacity = 1
        leftRectangle.width = Constants.appTotalWidth * Constants.mainMenueConfig.largeElemLeftWidthFactor
        theText.color = Constants.mainMenueConfig.largeFontColor
        moveIcon.opacity = 1
    }
    function goSmall()
    {
        width = smallElemWidth
        height = smallElemHeight
        color = Constants.mainMenueConfig.smallElemColor
        leftRectangle.opacity = 0
        leftRectangle.width = smallElemWidth
        theText.color = Constants.mainMenueConfig.smallFontColor
        moveIcon.opacity = 0
    }

    Rectangle
    {
        id: leftRectangle
        opacity: 0
        x: 0
        y: 0
        height: parent.height
        width: smallElemWidth
        color: Constants.mainMenueConfig.largeElemColorLeft
        Behavior on width
        {
            SmoothedAnimation {
                duration: animationDuration + animationSlowdown
                velocity: animactionVelocity
            }
        }
        Behavior on opacity
        {
            SmoothedAnimation {
                duration: animationDuration + animationSlowdown
                velocity: animactionVelocity
            }
        }
    }
    YACImage
    {
        id: icon
        width: Constants.appTotalWidth * Constants.mainMenueConfig.iconSizeFactor
        height: width
        anchors.centerIn: leftRectangle
        anchors.verticalCenterOffset: -height / 3
    }

    YACText
    {
        id: theText
        font.pixelSize: Constants.defaultFontPixelSize // * parent.height / smallElemHeight
        width: leftRectangle.width
        anchors.top: icon.bottom
        anchors.topMargin: icon.height / 4
        wrapMode: Text.Wrap
        horizontalAlignment: Text.AlignHCenter
        color: Constants.mainMenueConfig.smallFontColor
        Behavior on color {
            ColorAnimation {
                duration: animationDuration + animationSlowdown
            }
        }
    }

    Behavior on x
    {
        SmoothedAnimation {
            duration: animationDuration + animationSlowdown
            velocity: animactionVelocity
        }
    }
    Behavior on y
    {
        SmoothedAnimation {
            duration: animationDuration + animationSlowdown
            velocity: animactionVelocity
        }
    }
    Behavior on width
    {
        SmoothedAnimation {
            duration: animationDuration + animationSlowdown
            velocity: animactionVelocity
        }
    }
    Behavior on height
    {
        SmoothedAnimation {
            duration: animationDuration + animationSlowdown
            velocity: animactionVelocity
        }
    }
    MouseArea
    {
        anchors.fill: parent
        onClicked: theElem.clicked()
    }
    YACImage
    {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: width / 2
        width: Constants.appTotalWidth * 30.0 / 430.0
        height: width
        id: moveIcon
        source: "qrc:/images/images/menue_move_icon.svg"
        opacity: 0
        visible: opacity > 0
        Behavior on opacity
        {
            SmoothedAnimation {
                duration: animationDuration + animationSlowdown
                velocity: animactionVelocity
            }
        }
        MouseArea
        {
            enabled: parent.visible
            anchors.fill: parent
            drag.axis: Drag.YAxis
            onReleased: {
                Helper.jsLog("end drag")
                drag.target = null
                theElem.Drag.active = false
            }

            onPressAndHold:
            {
                drag.target = theElem
                theElem.Drag.active = true
            }
        }
    }
}
