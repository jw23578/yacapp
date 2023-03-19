import QtQuick 2.15

Loader
{
    id: pageLoader
    property bool loginNeeded: false
    property var theComponent: null
    property var parentSuperMenue: null
    property var parentCurrentOpenedLoader: null
    signal opened()
    width: 0
    height: parent.height
    x: parent.width / 2
    y: -height
    property int aniDuration: Constants.fastAnimationDuration
    Behavior on width
    {
        SmoothedAnimation
        {
            duration: pageLoader.aniDuration
        }
    }
    Behavior on x
    {
        SmoothedAnimation
        {
            duration: pageLoader.aniDuration
        }
    }
    Behavior on y
    {
        SmoothedAnimation
        {
            duration: pageLoader.aniDuration
        }
    }
    function open()
    {
        if (loginNeeded)
        {
            if (!yacApp.appUserConfig.loggedIn)
            {
                yacApp.badMessage(qsTr("Please login first."), null, null);
                return
            }
        }
        if (parentCurrentOpenedLoader != null)
        {
            parentCurrentOpenedLoader.close()
        }
        parentSuperMenue.close()
        sourceComponent = theComponent
        x = 0
        width =  parent.width
        y = 0
        opened()
    }

    function close()
    {
        x = theItem.width / 2
        width = 0
        y = -height
    }

    onSourceComponentChanged: Constants.superMenueClicked = sourceComponent != null
}
