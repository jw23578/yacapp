import QtQuick 2.15
import "../../JSConstants.js" as JSConstants

TextInput
{
    selectByMouse: Constants.isDesktop
    focus: true
    font.pixelSize: Constants.defaultFontPixelSize
    font.family: JSConstants.urbanistMedium
}
