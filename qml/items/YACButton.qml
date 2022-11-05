import QtQuick 2.0

YACRectangle
{
    id: button
    signal clicked()
    property alias text: theText.text
    property color baseColor: "lightgrey"
    width: theText.contentWidth + theText.contentHeight * 2
    implicitHeight: theText.contentHeight * 2
    color: mouseArea.pressed ? Qt.darker(baseColor, 1.1) :
                               mouseArea.containsMouse ? Qt.lighter(baseColor, 1.1) : baseColor
    shadow.visible: !mouseArea.pressed
    radius: height / 4
    radiusTopLeft: true
    radiusBottomRight:  true
    radiusTopRight: true
    radiusBottomLeft: true
    YACText
    {
        id: theText
        anchors.centerIn: parent
    }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        onClicked: button.clicked()
        hoverEnabled: true
    }
}
