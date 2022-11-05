import QtQuick 2.0
import Qt.labs.platform 1.0
import com.yacapp.menueconfig 1.0
import com.yacapp.globalprojectconfig 1.0
import "../items"

Rectangle
{
    id: menueEditor
    property MenueConfig config: null

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
                    headerText: "Type"
                    currentIndex: config ? find(config.type) : 0
                    onCurrentTextChanged: config.type = currentText
                    model: config ? config.typeOptions : null
                }
                Column
                {
                    id: columnItems
                    width: parent.width
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
