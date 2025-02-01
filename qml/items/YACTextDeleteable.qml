import QtQuick 2.15

YACRectangle
{
    property alias text: theText.text
    property alias font: theText.font
    property alias textColor: theText.color
    radius: Constants.radius
    radiusTopLeft: true
    radiusBottomLeft: true
    radiusTopRight: true
    radiusBottomRight: true
    border.color: theText.color
    border.width: 1
    width: theText.width + theImage.width
    height: theText.height
    signal deleteClicked()
    YACText
    {
        id: theText
        width: contentWidth + Constants.defaultMargin
        height: contentHeight
        horizontalAlignment: Text.AlignHCenter
    }
    YACImage
    {
        id: theImage
        anchors.left: theText.right
        anchors.leftMargin: - Constants.defaultMargin / 4
        anchors.verticalCenter: parent.verticalCenter
        height: theText.height
        width: height
        source: "qrc:/images/images/delete.svg"
        MouseArea
        {
            anchors.fill: parent
            onClicked: deleteClicked()
        }
    }
}
