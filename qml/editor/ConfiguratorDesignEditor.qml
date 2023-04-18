import QtQuick 2.0
import Qt.labs.platform 1.0
import com.yacapp.menueconfig 1.0
import com.yacapp.globalprojectconfig 1.0
import "../items"

Rectangle
{
    id: designEditor
    signal closeClicked()
    property GlobalProjectConfig global: null
    Column
    {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width
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
                    id: theMenueEditorSection
                    sectionCaption: "Design"
                    Column
                    {
                        id: columnItems
                        width: parent.width
                        YACLineEditWithHeader
                        {
                            headerText: qsTr("APP-Info-Url")
                            text: global.appInfoUrl
                            onDisplayTextChanged: global.appInfoUrl = text
                        }
                        YACLineEditWithHeader
                        {
                            headerText: qsTr("Search-Code")
                            text: global.searchCode
                            onDisplayTextChanged: global.searchCode = text
                        }
                        YACLineEditWithHeader
                        {
                            headerText: qsTr("Installation-Code")
                            text: global.installationCode
                            onDisplayTextChanged: global.installationCode = text
                        }
                        YACText
                        {
                            text: qsTr("Font PointSize")
                        }
                        YACIntEdit
                        {
                            width: parent.width
                            value: Constants.globalDesignConfig.defaultFontPointSize
                            onValueChanged: Constants.globalDesignConfig.defaultFontPointSize = value
                            minValue: 8
                            maxValue: 30
                        }
                        YACText
                        {
                            text: qsTr("smallElemCount")
                        }
                        YACDoubleEdit
                        {
                            width: parent.width
                            value: Constants.mainMenueConfig.smallElemCount
                            onValueChanged: Constants.mainMenueConfig.smallElemCount = value
                            minValue: 1
                            maxValue: 30
                            step: 0.1
                        }
                        YACText
                        {
                            text: qsTr("smallElemHeightFactor")
                        }
                        YACDoubleEdit
                        {
                            width: parent.width
                            value: Constants.mainMenueConfig.smallElemHeightFactor
                            onValueChanged: Constants.mainMenueConfig.smallElemHeightFactor = value
                            minValue: 0.06
                            maxValue: 0.2
                            step: 0.002
                            precision: 4
                        }
                        YACColorDialogWithHeader
                        {
                            headerText: qsTr("smallElemColor")
                            color: Constants.mainMenueConfig.smallElemColor
                            onAccepted: Constants.mainMenueConfig.smallElemColor = color
                        }
                    }
                }

                YACButton
                {
                    text: qsTr("Close")
                    onClicked: designEditor.closeClicked()
                }
            }
        }
    }
}
