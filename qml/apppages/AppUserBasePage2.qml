import QtQuick 2.15
import "../items"
import "../menue"

Rectangle
{
    clip: true
    property var theFocusObject: null
    color: Constants.backgroundColor
    width: parent.width
    MouseArea
    {
        anchors.fill: parent
    }
    Component.onCompleted:
    {
        waitTimer.start()
    }

    function closePage()
    {
        if (enableBack)
        {
            closeClicked()
            return
        }

        if (yacApp.superMenueMinized)
        {
            yacApp.restoreMenue()
        }
        else
        {
            yacApp.minimizeMenue()
        }
    }

    signal closeClicked();
    signal leftClicked();
    property bool enableBack: false
    property alias content: theContent
    property alias bottomRectangle: theBottomRectangle
    property alias heightInfoTextEdit: theHeightInfoTextEdit
    property alias multiMenueButton: theMultiMenueButton
    property alias arrowButton: theArrowButton
    MouseArea
    {
        anchors.fill: parent
    }
    Item
    {
        id: theContent
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: theBottomRectangle.top
    }

    MultiMenueButton
    {
        id: theMultiMenueButton
        anchors.left: parent.left
        y: parent.height - theHeightInfoTextEdit.height// - theSuperMenue.smallElemHeight
        visible: false
        z: 1
        openedCenterY: theArrowButton.height / 2 - Constants.radius / 4
        openedCenterX: theArrowButton.x + theArrowButton.width / 2

        closedCenterY: openedCenterY - theArrowButton.height * 2
        closedCenterX: openedCenterX
    }

    YACTextEditWithBackground
    {
        id: theHeightInfoTextEdit
        visible: false
        radius: Constants.radius / 2
        focus: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Constants.radius / 4
        anchors.bottom: bottomRectangle.bottom
        wrapMode: Text.WordWrap
        font.pixelSize: Constants.defaultFontPixelSize * Constants.x4largerTextFactor
        textFocus: false
    }

    Rectangle
    {
        id: theBottomRectangle
        anchors.left: parent.left
        anchors.right: parent.right
        height: theHeightInfoTextEdit.height + Constants.radius / 2
        anchors.bottom: parent.bottom
        color: Constants.buttonPrimaryColor
    }

    Timer
    {
        id: waitTimer
        running: false
        interval: 10
        repeat: false
    }

    YACArrowButton
    {
        id: theArrowButton
        rotation: waitTimer.running ? 90 : enableBack ? 270 : yacApp.superMenueMinized ? 0 : 180
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.verticalCenter: theBottomRectangle.verticalCenter
        onClicked: closePage()
    }
}
