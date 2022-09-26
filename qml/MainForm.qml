import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import com.yacapp.globalprojectconfig 1.0
import com.yacapp.parsedconfig 1.0
import com.yacapp.menueconfig 1.0
import "items"


Column
{
    id: mainForm
    property MenueConfig menue: yacApp.getMenueConfig(config.menueFilename)
    property ParsedConfig config: null
    signal currentItemChanged(ParsedConfig config)
    function openFilename(filename)
    {
        theStackView.push("SuperForm.qml", {
                              "config": yacApp.getConfig(filename),
                              "stackView": theStackView
                          }
                          )
    }

    YACHeader
    {
        id: header
    }
    StackView {
        id: theStackView
        initialItem:
            SuperForm
        {
            config: mainForm.config
            stackView: theStackView
        }
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: footer.top
        onCurrentItemChanged: {
            header.headerConfig = currentItem.config.header
            footer.footerConfig = currentItem.config.footer
            mainForm.currentItemChanged(currentItem.config)
        }
    }
    YACFooter
    {
        id: footer
    }
    YACDefaultMenue
    {
        visible: menue.type === "" || menue.type === "default"
        stackView: theStackView
    }

    YACImage
    {
        anchors.top: parent.top
        anchors.left: parent.left
        visible: source != ""
        source: yacApp.globalConfig.logoUrl
        width: parent.width * yacApp.globalConfig.logoWidth
        height: parent.height * yacApp.globalConfig.logoHeight
    }
}
