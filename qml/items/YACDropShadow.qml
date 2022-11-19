import QtQuick 2.0
import QtGraphicalEffects 1.15

Item
{
    id: theYACDropShadow
    enabled: Constants.shadowActive
    property var source: null
    property int shadowHorizontalOffset: enabled ? Constants.shadowHorizontalOffset : 0
    property int shadowVerticalOffset: enabled ? Constants.shadowVerticalOffset : 0
    Component
    {
        id: theShadow
        DropShadow
        {
            anchors.fill: parent
            horizontalOffset: theYACDropShadow.shadowHorizontalOffset
            verticalOffset: theYACDropShadow.shadowVerticalOffset
            radius: Math.floor(samples)
            samples: Constants.shadowSamples
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
