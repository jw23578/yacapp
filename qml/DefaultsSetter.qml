import QtQuick 2.15
import "../JSConstants.js" as JSConstants

Item
{
    FontLoader
    {
        id: urbanistMedium
        source: "qrc:/fonts/urbanist/Urbanist-Medium.ttf"
        onStatusChanged: {
            if (status == FontLoader.Ready)
            {
                Helper.jsLog("FontLoader: font loaded")
            }
        }
    }
    Item
    {
        id: theDefaultSetItem
        Text
        {
            id: configuratorText
            font
            {
                family: urbanistMedium.name
            }
        }

        Text
        {
            font
            {
                pointSize: Constants.globalDesignConfig.defaultFontPointSize
                family: urbanistMedium.name
            }
            onFontChanged: function(font)
            {
                Helper.jsLog("font changed: " + font.family)
                Constants.defaultFontPixelSize = Constants.isDesktop ? font.pixelSize : font.pixelSize
                Constants.configuratorDefaultFontPixelSize = Constants.isDesktop ? configuratorText.font.pixelSize : configuratorText.font.pixelSize
                Helper.jsLog("Constants.configuratorDefaultFontPixelSize: " + Constants.configuratorDefaultFontPixelSize)
            }

            id: theDefaultTextAndFont
            Component.onCompleted:
            {
                Constants.defaultFontPixelSize = Constants.isDesktop ? font.pixelSize : font.pixelSize
                Constants.configuratorDefaultFontPixelSize = Constants.isDesktop ? configuratorText.font.pixelSize : configuratorText.font.pixelSize
                JSConstants.urbanistMedium = urbanistMedium.name
                Helper.jsLog("font: " + urbanistMedium)
            }
        }
        Component.onCompleted:
        {
            Constants.defaultMargin = parent.width / 20
        }
    }


}
