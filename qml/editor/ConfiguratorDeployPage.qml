import QtQuick 2.15
import "../items"
import ".."

ConfiguratorPage
{
    id: deployPage
    anchors.fill: parent
    YACText
    {
        visible: Constants.isDesktop
        anchors.left: parent.left
        anchors.top: parent.top
        text: "ServerURL: " + configurator.getYacappServerUrl()
    }

    Column
    {
        id: theColumn
        anchors.centerIn: parent.contentItem
        spacing: 1
        width: parent.width * 3 / 4
        YACButton
        {
            width: parent.width
            id: templateAppButton
            text: yacApp.globalConfig.isTemplateApp ? qsTr("APP is offered as Startingpoint for others") : qsTr("APP is not offered as Startingpoint for others")
            onClicked: yacApp.globalConfig.isTemplateApp = !yacApp.globalConfig.isTemplateApp
        }

        YACButton
        {
            text: qsTr("Upload")
            width: parent.width
            onClicked:
            {
                if (configurator.activeProjectData.yacappServerLoginToken == "")
                {
                    CPPQMLAppAndConfigurator.badMessage(qsTr("Please login first"), null, null)
                    return
                }
                configurator.deploy(function()
                                    {
                                        CPPQMLAppAndConfigurator.goodMessage(qsTr("Deployment successful"), null, null)
                                    },
                                    function(message)
                                    {
                                        CPPQMLAppAndConfigurator.badMessage(message, null, null)
                                    })
            }
        }
        YACButton
        {
            text: qsTr("Abort")
            onClicked: deployPage.hidePage()
            width: parent.width
        }
        YACButton
        {
            text: qsTr("APP-Images")
            onClicked: appImages.showPage()
            width: parent.width
        }
    }
    ProgressPage
    {
        id: pp
    }
    ConfiguratorAppImagesPage
    {
        id: appImages
    }
}
