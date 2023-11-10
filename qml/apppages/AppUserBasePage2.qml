import QtQuick 2.15
import "../items"
import "../menue"

Rectangle
{
    MouseArea
    {
        anchors.fill: parent
    }
    Component.onCompleted:
    {
        yacApp.minimizeMenue()
    }

    anchors.fill: parent
    signal closeClicked();
    signal leftClicked();
    property alias content: theContent
    property alias backImage: theBackImage
    property alias bottomRectangle: theBottomRectangle
    property alias heightInfoTextEdit: theHeightInfoTextEdit
    property alias multiMenueButton: theMultiMenueButton
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
        color: "orange"
    }

    YACImage
    {
        id: theBackImage
        source: "qrc:/images/images/up-arrow.svg"
        rotation: yacApp.superMenueMinized ? 0 : 180
        Behavior on rotation {
            NumberAnimation {
                duration: Constants.fastAnimationDuration
            }
        }

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: Constants.radius / 4
        anchors.top: theBottomRectangle.top
        width: height
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                if (yacApp.superMenueMinized)
                {
                    yacApp.restoreMenue()

                    closeClicked()
                }
                else
                {
                    yacApp.minimizeMenue()
                }

            }
        }
    }
}
