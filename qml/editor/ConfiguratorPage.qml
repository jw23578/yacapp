import QtQuick 2.15
import "qrc:/qml/items"

Rectangle
{
    layer.enabled: true;
    function showPage()
    {
        opacity = 1
    }
    function hidePage()
    {
        opacity = 0
    }
    opacity: 0
    visible: opacity > 0
    Behavior on opacity {
        NumberAnimation {
            duration: Constants.fastAnimationDuration
        }
    }

    MouseArea
    {
        anchors.fill: parent
    }
    anchors.fill: parent
    color: ConfiguratorConstants.backgroundPrimaryColor
    property alias contentItem: theContentItem
    Item
    {
        id: theContentItem
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: logoutButton.top
    }

    YACButton
    {
        id: logoutButton
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        text: qsTr("Logout")
        visible: configurator.yacappServerLoginToken.length > 0
        onClicked:
        {
            configurator.yacserverLogout(configurator.deployUser,
                                        configurator.yacappServerLoginToken,
                                        function ()
                                        {
                                        },
                                        function (message)
                                        {
                                            CPPQMLAppAndConfigurator.badMessage(message, null, null)
                                        }
                                        )
        }
    }
}
