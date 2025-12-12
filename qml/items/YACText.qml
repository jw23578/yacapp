import QtQuick 2.15
import "../../JSConstants.js" as JSConstants

Text
{
    property bool useInConfigurator: false
    wrapMode: Text.WordWrap
    font.pixelSize: useInConfigurator ? Constants.configuratorDefaultFontPixelSize : Constants.defaultFontPixelSize
    font.family: JSConstants.urbanistMedium
    color: Constants.textPrimaryColor
}
