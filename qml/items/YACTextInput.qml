import QtQuick 2.15
import "../../JSConstants.js" as JSConstants

TextInput
{
    property alias emptyText: theEmptyText
    selectByMouse: Constants.isDesktop
    focus: true
    font.pixelSize: Constants.defaultFontPixelSize
    font.family: JSConstants.urbanistMedium
    Text
    {
        z: -1
        id: theEmptyText
        visible: opacity > 0
        opacity: parent.displayText == "" ? 1 : 0
        Behavior on opacity {
            NumberAnimation {
                duration: Constants.fastAnimationDuration
            }
        }

        anchors.fill: parent
        font: parent.font
        horizontalAlignment: parent.horizontalAlignment
        verticalAlignment: parent.verticalAlignment
        color: "#EEEEEE"
    }
}
