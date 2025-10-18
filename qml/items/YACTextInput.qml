import QtQuick 2.15
import "../../JSConstants.js" as JSConstants

TextInput
{
    property alias emptyText: theEmptyText
    property bool useInConfigurator: false
    selectByMouse: Constants.isDesktop
    focus: true
    font.pixelSize: useInConfigurator ? Constants.configuratorDefaultFontPixelSize : Constants.defaultFontPixelSize
    font.family: JSConstants.urbanistMedium
    color: Constants.textInputFontColor
    Text
    {
        z: -1
        id: theEmptyText
        visible: opacity > 0
        opacity: parent.displayText == "" ? 1 : 0.1
        Behavior on opacity {
            NumberAnimation {
                duration: Constants.slowAnimationDuration
            }
        }

        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.family: parent.font.family
        font.pixelSize: parent.font.pixelSize * 0.8
        color: Qt.darker(parent.color)
    }
}
