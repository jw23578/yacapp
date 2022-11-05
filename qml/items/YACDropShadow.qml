import QtQuick 2.0
import QtGraphicalEffects 1.15

Item
{
    id: theYACDropShadow
    enabled: Constants.shadowActive
    property var source: null
    Component
    {
        id: theShadow
        DropShadow
        {
            anchors.fill: parent
            horizontalOffset: Constants.shadowHorizontalOffset
            verticalOffset: Constants.shadowVerticalOffset
            radius: 8.0
            samples: 9
            color: "#80000000"
            source: theYACDropShadow.source
        }
    }

    Loader
    {
        anchors.fill: parent
        active: Constants.shadowActive
        sourceComponent: theShadow
    }
}
