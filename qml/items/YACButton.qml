import QtQuick 2.0

Item
{
    id: theButton
    signal clicked()
    property bool useInConfigurator: false
    property alias radius: theRectangle.radius
    property alias font: theText.font
    property alias text: theText.text
    property alias emptyText: theEmptyText.text
    property alias source: theImage.source
    property alias showLoadingImage: theImage.showLoadingImage
    property alias shadow: theRectangle.shadow
    property color baseColor: Constants.buttonColor
    width: theText.contentWidth + theText.contentHeight * 2
    height: theText.contentHeight * 2 + shadow.shadowVerticalOffset * 2
    property double captionStartX: theRectangle.x + theRectangle.radius
    YACRectangle
    {
        id: theRectangle
        x: shadow.shadowHorizontalOffset
        width: theButton.width
        implicitHeight: theButton.height
        color: mouseArea.pressed ? Qt.darker(baseColor, 1.1) :
                                   mouseArea.containsMouse ? Qt.lighter(baseColor, 1.1) : baseColor
        shadow.visible: !mouseArea.pressed && Constants.shadowActive
        radius: width == height ? height / 2 : height / 4
        radiusTopLeft: true
        radiusBottomRight:  true
        radiusTopRight: true
        radiusBottomLeft: true

        YACText
        {
            id: theText
            useInConfigurator: theButton.useInConfigurator
            visible: text != ""
            anchors.top: parent.theInnerRectangle.top
            anchors.bottom: parent.theInnerRectangle.bottom
            anchors.left: theImage.visible ? theImage.right : parent.theInnerRectangle.left
            anchors.right: parent.theInnerRectangle.right
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            clip: true
        }
        YACText
        {
            useInConfigurator: theButton.useInConfigurator
            visible: theText.text == "" && text != ""
            id: theEmptyText
            anchors.fill: theText
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            clip: true
        }
        YACImage
        {
            id: theImage
            visible: source != ""
            anchors.centerIn: parent
            anchors.margins: Constants.shadowHorizontalOffset + parent.radius / 2
            width: height
            height: parent.height * 3 / 4
            showLoadingImage: true
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
