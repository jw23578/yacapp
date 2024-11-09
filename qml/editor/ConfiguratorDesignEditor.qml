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
                    useInConfigurator: true
                    sectionCaption: "Design"
                    Column
                    {
                        id: columnItems
                        width: parent.width
                        YACLineEditWithHeader
                        {
                            useInConfigurator: true
                            headerText: qsTr("APP-Info-Url")
                            text: global.appInfoUrl
                            onDisplayTextChanged: global.appInfoUrl = text
                        }
                        YACLineEditWithHeader
                        {
                            useInConfigurator: true
                            headerText: qsTr("Search-Code")
                            text: global.searchCode
                            onDisplayTextChanged: global.searchCode = text
                        }
                        YACLineEditWithHeader
                        {
                            useInConfigurator: true
                            headerText: qsTr("Installation-Code")
                            text: configurator.activeProjectData.installationCode
                            onDisplayTextChanged: configurator.activeProjectData.installationCode = text
                        }
                        YACComboBoxWithHeader
                        {
                            useInConfigurator: true
                            headerText: qsTr("Fremdsystem (Loginsteuerung)")
                            initialText: yacApp.globalConfig.third
                            onCurrentTextChanged: yacApp.globalConfig.third = currentText
                            model: [""
                                , "aidoo"]
                        }
                        YACLineEditWithHeader
                        {
                            useInConfigurator: true
                            headerText: qsTr("Fremdsystem-Mandant")
                            text: yacApp.globalConfig.mandant
                            onDisplayTextChanged: yacApp.globalConfig.mandant = text
                        }
                        YACText
                        {
                            text: qsTr("Font PointSize")
                            useInConfigurator: true
                        }
                        YACIntEdit
                        {
                            useInConfigurator: true
                            width: parent.width
                            value: Constants.globalDesignConfig.defaultFontPointSize
                            onValueChanged: Constants.globalDesignConfig.defaultFontPointSize = value
                            minValue: 8
                            maxValue: 30
                        }
                        YACText
                        {
                            text: qsTr("smallElemCount")
                            useInConfigurator: true
                        }
                        YACDoubleEdit
                        {
                            useInConfigurator: true
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
                            useInConfigurator: true
                        }
                        YACDoubleEdit
                        {
                            useInConfigurator: true
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
                            useInConfigurator: true
                            headerText: qsTr("smallElemColor")
                            color: Constants.mainMenueConfig.smallElemColor
                            onAccepted: Constants.mainMenueConfig.smallElemColor = color
                        }
                    }
                }

                YACButton
                {
                    useInConfigurator: true
                    text: qsTr("Close")
                    onClicked: designEditor.closeClicked()
                }
            }
        }
    }
}
