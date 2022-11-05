import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import com.yacapp.globalprojectconfig 1.0
import com.yacapp.parsedconfig 1.0
import com.yacapp.menueconfig 1.0
import "items"


Item
{
    id: theItem
    anchors.fill: parent
    focus: true
    property MenueConfig menue: yacApp.getMenueConfig(config.menueFilename)
    property ParsedConfig config: yacApp.mainConfig
    signal currentItemChanged(ParsedConfig config)
    function openFilename(filename)
    {
        theStackView.push("SuperForm.qml", {
                              "config": yacApp.getConfig(filename),
                              "stackView": theStackView,
                              "theMenue": theRealMenue
                          })
    }
    Column
    {
        width: parent.width
        id: mainForm
        YACHeader
        {
            id: header
            width: parent.width
        }
        StackView
        {
            id: theStackView
            initialItem:
                SuperForm
            {
                config: theItem.config
                stackView: theStackView
                theMenue: theRealMenue
            }
            height: theItem.height - header.height - footer.height
            width: parent.width
            onCurrentItemChanged: {
                header.headerConfig = currentItem.config.header
                footer.footerConfig = currentItem.config.footer
                theItem.currentItemChanged(currentItem.config)
            }
        }

        YACFooter
        {
            id: footer
            minimumHeight: theRealMenue.openCloseButtonHeight
        }
    }
    YACDefaultMenue
    {
        id: theRealMenue
        visible: menue.type === "" || menue.type === "default"
        stackView: theStackView
    }

    YACImage
    {
        anchors.top: parent.top
        anchors.topMargin: parent.height * yacApp.globalConfig.logoOffsetYPerThousand / 1000.0
        anchors.left: parent.left
        anchors.leftMargin: parent.width * yacApp.globalConfig.logoOffsetXPerThousand / 1000.0
        visible: source != ""
        source: yacApp.globalConfig.logoUrl
        width: parent.width * yacApp.globalConfig.logoWidthPerThousand / 1000.0
        height: parent.height * yacApp.globalConfig.logoHeightPerThousand / 1000.0
    }
}
