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
    property MenueConfig menue: yacApp.getMenueConfig(config.menueFilename)
    property ParsedConfig config: null
    signal currentItemChanged(ParsedConfig config)
    function openFilename(filename)
    {
        theStackView.push("SuperForm.qml", {
                              "config": yacApp.getConfig(filename),
                              "stackView": theStackView,
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
        }
    }
    YACDefaultMenue
    {
        visible: menue.type === "" || menue.type === "default"
        stackView: theStackView
    }

    YACImage
    {
        anchors.top: parent.top
        anchors.topMargin: parent.height * yacApp.globalConfig.logoOffsetY
        anchors.left: parent.left
        anchors.leftMargin: parent.width * yacApp.globalConfig.logoOffsetX
        visible: source != ""
        source: yacApp.globalConfig.logoUrl
        width: parent.width * yacApp.globalConfig.logoWidth
        height: parent.height * yacApp.globalConfig.logoHeight
    }
}
