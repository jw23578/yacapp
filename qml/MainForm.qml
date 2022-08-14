import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import com.yacapp.parsedconfig 1.0


Column
{
    id: mainForm
    property var config: null
    signal currentItemChanged(ParsedConfig config)
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
        height: parent.height - footer.height - header.height
        anchors.left: parent.left
        anchors.right: parent.right
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
}
