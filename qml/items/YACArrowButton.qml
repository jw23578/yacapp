import QtQuick 2.15

YACRectangle
{
    id: theButton
    signal clicked;
    property color baseColor: Constants.buttonColor
    color: mouseArea.pressed ? Qt.darker(baseColor, 1.1) :
                               mouseArea.containsMouse ? Qt.lighter(baseColor, 1.1) : baseColor
    width: buttonHeightCalc.height + Constants.radius / 2
    height: width
    radius: width == height ? height / 2 : height / 4
    radiusTopLeft: true
    radiusBottomRight:  true
    radiusTopRight: true
    radiusBottomLeft: true
    property alias imageRotation: theImage.rotation
    YACImage
    {
        id: theImage
        anchors.centerIn: parent
        source: "qrc:/images/images/up-arrow.svg"
        Behavior on rotation {
            NumberAnimation {
                duration: Constants.fastAnimationDuration
            }
        }
        height: buttonHeightCalc.height
        width: height
        MouseArea
        {
            id: mouseArea
            anchors.fill: parent
            onClicked:
            {
                theButton.clicked()
            }
        }

        YACTextEditWithBackground
        {
            id: buttonHeightCalc
            visible: false
            font.pixelSize: Constants.defaultFontPixelSize * Constants.x4largerTextFactor
        }
    }
}
