import QtQuick 2.15
import com.yacapp.parsedconfig 1.0
import com.yacapp.globalprojectconfig 1.0
import com.yacapp.headerconfig 1.0
import com.yacapp.contentconfig 1.0
import QtQuick.Dialogs
import "qrc:/qml/items"
import "qrc:/qml/dialogs"

Rectangle
{
    id: editorForm

    property ParsedConfig config: null
    property GlobalProjectConfig global: null
    signal loadConfig(string filename)
    signal menueFilenameChanged(string filename)
    Column
    {
        id: topColumn
        width: parent.width
        YACText
        {
            useInConfigurator: true
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("Pages")
            font.bold: true
        }

        Row
        {
            width: parent.width
            spacing: 1
            YACComboBoxWithHeader
            {
                useInConfigurator: true
                id: fileCombobox
                headerText: qsTr("Files")
                onActivated: editorForm.loadConfig(text)
                model: yacApp.knownFiles
                width: parent.width - addFileButton.width - 1 - deleteCurrentFileButton.width - 1
            }
            YACButton
            {
                useInConfigurator: true
                y: fileCombobox.comboBox.y
                id: addFileButton
                text: qsTr("Add File")
                onClicked:
                {
                    configurator.addFileSignal(function(fileName)
                    {
                        configurator.addFormFile(fileName)
                    })
                }
            }
            YACButton
            {
                useInConfigurator: true
                y: fileCombobox.comboBox.y
                id: deleteCurrentFileButton
                text: qsTr("Delete File")
                onClicked: CPPQMLAppAndConfigurator.badMessage(qsTr("Not yet implemented"), null, null)
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
                spacing: 1

                YACComboBoxWithHeader
                {
                    useInConfigurator: true
                    id: menuefileCombobox
                    currentIndex: find(config.menueFilename)
                    headerText: qsTr("Menuefile")
                    onActivated:
                    {
                        config.menueFilename = text
                        menueFilenameChanged(text)
                    }
                    model: yacApp.knownMenueFiles
                    width: parent.width
                }
                EditorSection
                {
                    sectionCaption: qsTr("AppUser")
                    YACButton
                    {
                        useInConfigurator: true
                        width: parent.width
                        text: qsTr("APP-User Profile (create/login) enabled: ") + (global.appUserEnabled ? qsTr("Yes") : qsTr("No"))
                        onClicked: {
                            global.appUserEnabled = !global.appUserEnabled
                            yacApp.superMenueChanged()
                        }
                    }
                    YACButton
                    {
                        useInConfigurator: true
                        width: parent.width
                        text: qsTr("APP-User Messages enabled: ") + (global.appUserMessagesEnabled ? qsTr("Yes") : qsTr("No"))
                        onClicked: {
                            global.appUserMessagesEnabled = !global.appUserMessagesEnabled
                            yacApp.superMenueChanged()
                        }
                    }
                    YACButton
                    {
                        useInConfigurator: true
                        width: parent.width
                        text: qsTr("APP-User News enabled: ") + (global.appUserNewsEnabled ? qsTr("Yes") : qsTr("No"))
                        onClicked: {
                            global.appUserNewsEnabled = !global.appUserNewsEnabled
                            yacApp.superMenueChanged()
                        }
                    }
                    YACButton
                    {
                        useInConfigurator: true
                        width: parent.width
                        text: qsTr("APP-User Worktime: ") + (global.appUserWorktimeEnabled ? qsTr("Yes") : qsTr("No"))
                        onClicked: {
                            global.appUserWorktimeEnabled = !global.appUserWorktimeEnabled
                            yacApp.superMenueChanged()
                        }
                    }
                    YACButton
                    {
                        useInConfigurator: true
                        width: parent.width
                        text: qsTr("APP-User Appointments: ") + (global.appUserAppointmentsEnabled ? qsTr("Yes") : qsTr("No"))
                        onClicked: {
                            global.appUserAppointmentsEnabled = !global.appUserAppointmentsEnabled
                            yacApp.superMenueChanged()
                        }
                    }
                    YACButton
                    {
                        useInConfigurator: true
                        width: parent.width
                        text: qsTr("APP-User Documents: ") + (global.appUserDocumentsEnabled ? qsTr("Yes") : qsTr("No"))
                        onClicked: {
                            global.appUserDocumentsEnabled = !global.appUserDocumentsEnabled
                            yacApp.superMenueChanged()
                        }
                    }
                    YACButton
                    {
                        useInConfigurator: true
                        width: parent.width
                        text: qsTr("APP-User Spaces: ") + (global.appUserSpacesEnabled ? qsTr("Yes") : qsTr("No"))
                        onClicked: {
                            global.appUserSpacesEnabled = !global.appUserSpacesEnabled
                            yacApp.superMenueChanged()
                        }
                    }
                    YACButton
                    {
                        useInConfigurator: true
                        width: parent.width
                        text: qsTr("APP-User Spaces: ") + (global.appUserRightsEnabled ? qsTr("Yes") : qsTr("No"))
                        onClicked: {
                            global.appUserRightsEnabled = !global.appUserRightsEnabled
                            yacApp.superMenueChanged()
                        }
                    }
                }

                EditorSection
                {
                    sectionCaption: qsTr("Logo")
                    YACLineEditWithHeader
                    {
                        useInConfigurator: true
                        headerText: qsTr("LogoUrl")
                        text: global.logoUrl
                        onDisplayTextChanged: global.logoUrl = text
                    }
                    YACLineEditWithHeader
                    {
                        useInConfigurator: true
                        headerText: qsTr("Logo-Width per thousand")
                        intEdit: true
                        text: global.logoWidthPerThousand
                        onDisplayTextChanged: global.logoWidthPerThousand = text
                    }
                    YACLineEditWithHeader
                    {
                        useInConfigurator: true
                        headerText: qsTr("Logo-Offset-X per thousand")
                        intEdit: true
                        text: global.logoOffsetXPerThousand
                        onDisplayTextChanged: global.logoOffsetXPerThousand = text
                    }
                    YACLineEditWithHeader
                    {
                        useInConfigurator: true
                        headerText: qsTr("Logo-Height per thousand")
                        intEdit: true
                        text: global.logoHeightPerThousand
                        onDisplayTextChanged: global.logoHeightPerThousand = text
                    }
                    YACLineEditWithHeader
                    {
                        useInConfigurator: true
                        headerText: qsTr("Logo-Offset-Y per thousand")
                        intEdit: true
                        text: global.logoOffsetYPerThousand
                        onDisplayTextChanged: global.logoOffsetYPerThousand = text
                    }
                }

                EditorSection
                {
                    sectionCaption: qsTr("Header")
                    YACLineEditWithHeader
                    {
                        useInConfigurator: true
                        headerText: qsTr("Title")
                        text: config.header.title
                        onDisplayTextChanged: config.header.title = text
                    }
                    YACLineEditWithHeader
                    {
                        useInConfigurator: true
                        headerText: qsTr("Height per thousand")
                        doubleEdit: true
                        text: config.header.heightPerThousand
                        onDisplayTextChanged: config.header.heightPerThousand = doubleValue
                    }
                    YACColorDialogWithHeader
                    {
                        useInConfigurator: true
                        headerText: qsTr("Color")
                        color: config.header.color
                        onColorChanged: config.header.color = color
                    }
                }
                EditorSection
                {
                    sectionCaption: qsTr("Background")
                    YACColorDialogWithHeader
                    {
                        useInConfigurator: true
                        headerText: qsTr("Color")
                        color: config.background.color
                        onColorChanged: config.background.color = color
                    }
                }

                EditorSection
                {
                    sectionCaption: qsTr("Content")
                    YACSwitch
                    {
                        useInConfigurator: true
                        text: qsTr("Login needed")
                        checked: config.content.loginNeeded
                        onCheckedChanged: config.content.loginNeeded = checked
                    }
                    YACComboBoxWithHeader
                    {
                        useInConfigurator: true
                        id: contentType
                        headerText: qsTr("Type")
                        currentIndex: find(config.content.type)
                        onCurrentTextChanged: config.content.type = currentText
                        model: config.content.typeOptions
                    }
                    YACLineEditWithHeader
                    {
                        useInConfigurator: true
                        showColumn: config.content.type == "grid"
                        intEdit: true
                        headerText: qsTr("Columns")
                        text: config.content.columns
                        onDisplayTextChanged: config.content.columns = text
                    }
                    YACLineEditWithHeader
                    {
                        useInConfigurator: true
                        showColumn: config.content.type == "grid"
                        intEdit: true
                        headerText: qsTr("Rows")
                        text: config.content.rows
                        onDisplayTextChanged: config.content.rows = text
                    }
                    YACLineEditWithHeader
                    {
                        useInConfigurator: true
                        showColumn: config.content.type == "webview"
                        headerText: qsTr("Url")
                        text: config.content.url
                        onDisplayTextChanged: config.content.url = text
                    }
                    Column
                    {
                        id: columnItems
                        width: parent.width
                        property bool showRow: ["row", "grid"].includes(config.content.type)
                        property bool showColumn: ["column", "grid"].includes(config.content.type)
                        Repeater
                        {
                            model: parent.showColumn || parent.showRow ? config.content.itemCount : 0
                            delegate: Column
                            {
                                id: itemColumn
                                property var currentItem: config.content.items[index]
                                property bool tileType: ["tile"].includes(currentItem.type)
                                property bool slidertileType: ["slidertile"].includes(currentItem.type)
                                property bool flipableType: ["flipable"].includes(currentItem.type)
                                property bool imageType: ["image"].includes(currentItem.type)
                                property bool animatedimageType: ["animatedimage"].includes(currentItem.type)
                                width: columnItems.width
                                Item
                                {
                                    height: contentItemType.height / 2
                                    width: 1
                                }

                                Row
                                {
                                    YACComboBoxWithHeader
                                    {
                                        useInConfigurator: true
                                        id: contentItemType
                                        width: itemColumn.width - removeItem.width
                                        headerText: qsTr("Type")
                                        currentIndex: find(config.content.items[index].type)
                                        onCurrentTextChanged: config.content.items[index].type = currentText
                                        model: config.content.items[index].typeOptions
                                    }
                                    YACButton
                                    {
                                        useInConfigurator: true
                                        id: removeItem
                                        text: qsTr("Remove Item")
                                        onClicked: CPPQMLAppAndConfigurator.yesNoQuestion(qsTr("Remove Item?")
                                                                        , null
                                                                        , function()
                                                                        {
                                                                            config.content.removeItem(index)
                                                                        }
                                                                        , function()
                                                                        {
                                                                        }

                                            )
                                    }
                                }
                                YACLineEditWithHeader
                                {
                                    useInConfigurator: true
                                    id: theTarget
                                    headerText: qsTr("Target")
                                    text: config.content.items[index].target
                                    onDisplayTextChanged: config.content.items[index].target = text
                                }
                                YACLineEditWithHeader
                                {
                                    useInConfigurator: true
                                    id: theText
                                    headerText: qsTr("Text")
                                    text: currentItem.text
                                    onDisplayTextChanged: currentItem.text = text
                                    visible: tileType || slidertileType || flipableType
                                }
                                YACColorDialogWithHeader
                                {
                                    useInConfigurator: true
                                    headerText: qsTr("Color")
                                    color: currentItem.color
                                    onAccepted: currentItem.color = color
                                    visible: tileType || slidertileType || flipableType
                                }
                                YACLineEditWithHeader
                                {
                                    useInConfigurator: true
                                    id: theFlipableFilename
                                    headerText: qsTr("FlipableFilename")
                                    text: currentItem.flipableFilename
                                    onDisplayTextChanged: currentItem.flipableFilename = text
                                    visible: slidertileType || flipableType
                                }
                                YACLineEditWithHeader
                                {
                                    useInConfigurator: true
                                    id: theText2
                                    headerText: qsTr("Text2")
                                    text: currentItem.text2
                                    onDisplayTextChanged: currentItem.text2 = text
                                    visible: slidertileType || flipableType
                                }
                                YACColorDialogWithHeader
                                {
                                    useInConfigurator: true
                                    headerText: qsTr("Color2")
                                    color: currentItem.color2
                                    onAccepted: currentItem.color2 = color
                                    visible: slidertileType || flipableType
                                }
                                YACLineEditWithHeader
                                {
                                    useInConfigurator: true
                                    id: theFlipableFilename2
                                    headerText: qsTr("FlipableFilename2")
                                    text: currentItem.flipableFilename2
                                    onDisplayTextChanged: currentItem.flipableFilename2 = text
                                    visible: slidertileType || flipableType
                                }

                                YACLineEditWithHeader
                                {
                                    useInConfigurator: true
                                    id: theUrl
                                    headerText: qsTr("Url")
                                    text: config.content.items[index].url
                                    onDisplayTextChanged: config.content.items[index].url= text
                                    visible: ["webview", "image"].includes(config.content.items[index].type) || animatedimageType
                                }
                                YACComboBoxWithHeader
                                {
                                    useInConfigurator: true
                                    id: theFillMode
                                    headerText: qsTr("Fillmode")
                                    visible: imageType || animatedimageType
                                    initialText: config.content.items[index].fillModeString
                                    onCurrentTextChanged: config.content.items[index].fillModeString = currentText
                                    model: ["Image.Stretch"
                                        , "Image.PreserveAspectFit"
                                        , "Image.PreserveAspectCrop"
                                        , "Image.Tile"
                                        , "Image.TileVertically"
                                        , "Image.TileHorizontally"
                                        , "Image.Pad"]
                                }

                                Row
                                {
                                    visible: (columnItems.showColumn || columnItems.showRow) && config.content.items[index].type == "file"
                                    width: parent.width
                                    YACLineEditWithHeader
                                    {
                                        useInConfigurator: true
                                        id: theFilename
                                        headerText: qsTr("Filename")
                                        text: config.content.items[index].filename
                                        onDisplayTextChanged: config.content.items[index].filename = text
                                        width: parent.width - editFileButton.width
                                    }
                                    YACButton
                                    {
                                        useInConfigurator: true
                                        id: editFileButton
                                        text: qsTr("Edit")
                                        onClicked: editorForm.loadConfig(config.content.items[index].filename)
                                    }
                                }
                                YACLineEditWithHeader
                                {
                                    useInConfigurator: true
                                    headerText: qsTr("Width per thousand")
                                    doubleEdit: true
                                    text: config.content.items[index].widthPerThousand
                                    onDisplayTextChanged: config.content.items[index].widthPerThousand = text
                                    visible: columnItems.showRow
                                }
                                YACLineEditWithHeader
                                {
                                    useInConfigurator: true
                                    headerText: qsTr("Height per thousand")
                                    doubleEdit: true
                                    text: config.content.items[index].heightPerThousand
                                    onDisplayTextChanged: config.content.items[index].heightPerThousand = text
                                    visible: columnItems.showColumn
                                }
                                YACLineEditWithHeader
                                {
                                    useInConfigurator: true
                                    headerText: qsTr("Left Margin per Thousand")
                                    intEdit: true
                                    text: config.content.items[index].leftMarginPerThousand
                                    onDisplayTextChanged: config.content.items[index].leftMarginPerThousand = text
                                }
                                YACLineEditWithHeader
                                {
                                    useInConfigurator: true
                                    headerText: qsTr("Right Margin per Thousand")
                                    intEdit: true
                                    text: config.content.items[index].rightMarginPerThousand
                                    onDisplayTextChanged: config.content.items[index].rightMarginPerThousand = text
                                }
                                YACLineEditWithHeader
                                {
                                    useInConfigurator: true
                                    headerText: qsTr("Top Margin per Thousand")
                                    intEdit: true
                                    text: config.content.items[index].topMarginPerThousand
                                    onDisplayTextChanged: config.content.items[index].topMarginPerThousand = text
                                }
                                YACLineEditWithHeader
                                {
                                    useInConfigurator: true
                                    headerText: qsTr("Bottom Margin per Thousand")
                                    intEdit: true
                                    text: config.content.items[index].bottomMarginPerThousand
                                    onDisplayTextChanged: config.content.items[index].bottomMarginPerThousand = text
                                }
                            }
                        }
                        YACButton
                        {
                            useInConfigurator: true
                            visible: parent.showColumn || parent.showRow
                            text: qsTr("Add Item")
                            onClicked: config.content.addItem(-1)
                        }
                    }
                }
            }
        }
    }
}
