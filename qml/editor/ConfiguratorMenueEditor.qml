import QtQuick 2.0
import Qt.labs.platform 1.0
import com.yacapp.menueconfig 1.0
import com.yacapp.globalprojectconfig 1.0
import "../items"

Rectangle
{
    id: menueEditor
    property MenueConfig config: null

    Column
    {
        id: topColumn
        width: parent.width
        YACText
        {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("Menues")
            font.bold: true
        }
        Row
        {
            width: parent.width
            spacing: 1
            YACComboBoxWithHeader
            {
                id: fileCombobox
                headerText: qsTr("Files")
                onActivated: menueEditor.config = yacApp.getMenueConfig(text)
                model: yacApp.knownMenueFiles
                width: parent.width - addFileButton.width - 1 - deleteCurrentFileButton.width - 1
            }
            YACButton
            {
                y: fileCombobox.comboBox.y
                id: addFileButton
                text: qsTr("Add File")
                onClicked:
                {
                    configurator.addFileSignal(function(fileName)
                    {
                        configurator.addMenueFile(fileName)
                    })
                }
            }
            YACButton
            {
                y: fileCombobox.comboBox.y
                id: deleteCurrentFileButton
                text: qsTr("Delete File")
//                onClicked: yacApp.badMessage(qsTr("Not yet implemented"), null, null)
            }
        }
    }
    Column
    {
        anchors.top: topColumn.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        Rectangle
        {
            width: parent.width
            height: 1
            color: "black"
        }
        Flickable
        {
            clip: true
            width: parent.width
            height: parent.height
            contentHeight: editorSections.height
            Column
            {
                id: editorSections
                width: parent.width
                spacing: 10

                EditorSection
                {
                    sectionCaption: "Menue"
                    YACComboBoxWithHeader
                    {
                        id: menueType
                        parent: parent.column
                        headerText: "Type"
                        currentIndex: config ? find(config.type) : 0
                        onCurrentTextChanged: config.type = currentText
                        model: config ? config.typeOptions : null
                    }
                    Column
                    {
                        id: columnItems
                        width: parent.width
                        parent: parent.column
                        Repeater
                        {
                            model: config ? config.itemCount : null
                            delegate: Column
                            {
                                width: columnItems.width
                                YACLineEditWithHeader
                                {
                                    headerText: "Caption"
                                    doubleEdit: true
                                    text: config.items[index].caption
                                    onDisplayTextChanged: config.items[index].caption = text
                                }
                                YACLineEditWithHeader
                                {
                                    id: target
                                    headerText: "Target"
                                    text: config.items[index].target
                                    onDisplayTextChanged: config.items[index].target = text
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
