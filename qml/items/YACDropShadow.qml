import QtQuick 2.0
import QtGraphicalEffects 1.15

Item
{
    id: theYACDropShadow
    enabled: Constants.shadowActive
    property var source: null
    property int shadowRadius: enabled ? 8.0 : 0
    Component
    {
        id: theShadow
        DropShadow
        {
            anchors.fill: parent
            horizontalOffset: Constants.shadowHorizontalOffset
            verticalOffset: Constants.shadowVerticalOffset
            radius: theYACDropShadow.shadowRadius
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
