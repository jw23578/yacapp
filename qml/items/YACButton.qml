import QtQuick 2.0

Item
{
    id: theButton
    signal clicked()
    property alias text: theText.text
    property alias source: theImage.source
    property alias shadow: theRectangle.shadow
    property color baseColor: "lightgrey"
    width: theText.contentWidth + theText.contentHeight * 2
    height: theRectangle.height
    property double captionStartX: theRectangle.x + theRectangle.radius
    YACRectangle
    {
        id: theRectangle
        x: shadow.shadowHorizontalOffset
        width: theButton.width
        implicitHeight: theText.contentHeight * 2 + shadow.shadowVerticalOffset * 2
        color: mouseArea.pressed ? Qt.darker(baseColor, 1.1) :
                                   mouseArea.containsMouse ? Qt.lighter(baseColor, 1.1) : baseColor
        shadow.visible: !mouseArea.pressed
        radius: height / 4
        radiusTopLeft: true
        radiusBottomRight:  true
        radiusTopRight: true
        radiusBottomLeft: true
        YACImage
        {
            id: theImage
            visible: source != ""
            anchors.margins: Constants.shadowHorizontalOffset
            anchors.top: parent.theInnerRectangle.top
            anchors.left: parent.theInnerRectangle.left
            anchors.right: parent.theInnerRectangle.horizontalCenter
            anchors.bottom: parent.theInnerRectangle.bottom
            width: parent.width / 2
            height: parent.height
        }

        YACText
        {
            id: theText
            anchors.top: parent.theInnerRectangle.top
            anchors.bottom: parent.theInnerRectangle.bottom
            anchors.left: theImage.visible ? theImage.right : parent.theInnerRectangle.left
            anchors.right: parent.theInnerRectangle.right
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            clip: true
        }

        MouseArea
        {
            id: mouseArea
            anchors.fill: parent
            onClicked: theButton.clicked()
            hoverEnabled: true
        }
    }
}
