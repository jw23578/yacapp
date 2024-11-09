import QtQuick 2.0
import QtQuick.Effects

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
        MultiEffect
        {
            anchors.fill: parent
            shadowEnabled: Constants.shadowActive
            shadowHorizontalOffset: theYACDropShadow.shadowHorizontalOffset
            shadowVerticalOffset: theYACDropShadow.shadowVerticalOffset
            shadowBlur: 0.6
            shadowColor: "#80000000"
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
