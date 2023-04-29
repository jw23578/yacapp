import QtQuick 2.15
import "qrc:/qml/items"

DialogBase
{
    id: dialogAddFile
    onAbortClicked: close()
    property var okCallback: null
    onOkClicked:
    {
        if (fileName.displayText == "")
        {
            CPPQMLAppAndConfigurator.badMessage(qsTr("Please enter the Fileame"), fileName, null)
            return
        }
        close()
        if (okCallback != null)
        {
            okCallback(fileName.displayText)
        }
    }
    Column
    {
        anchors.centerIn: parent
        width: parent.width * 4 / 5
        YACLineEditWithHeader
        {
            id: fileName
            focus: true
            headerText: qsTr("Filename")
        }
    }

}
