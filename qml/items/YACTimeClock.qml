import QtQuick 2.15

Item
{
    id: timeItem
    property date theTime: new Date()
    property int theHour: Helper.getHour(theTime)
    property int theMinute: Helper.getMinute(theTime)
    property alias hourItemVisible: hourItem.visible
    YACText
    {
        id: sizeText
        text: "23"
        visible: false
    }
    function set(datetime)
    {
        theTime = datetime
        hourTranslate.y = Helper.getHour(datetime) < 12 ? -theOuterPathView.theRadius : -theInnerPathView.theRadius
        hourRotation.angle = (Helper.getHour(datetime) % 12) * 30
        minuteRotation.angle = Helper.getMinute(datetime) * 6
    }

    Rectangle
    {
        id: hourItem
        color: "grey"
        anchors.centerIn: parent
        width: parent.width
        height: width
        Rectangle
        {
            id: hourMarker
            radius: width / 2
            width: sizeText.contentWidth * 2
            height: width
            color: "lightgrey"
            anchors.centerIn: theOuterPathView
            transform: [
                Translate
                {
                    id: hourTranslate
                    x: 0
                    y: -theOuterPathView.theRadius
                    Behavior on y
                    {
                        NumberAnimation {
                            duration: Constants.fastAnimationDuration
                        }
                    }
                },

                Rotation
                {
                    id: hourRotation
                    origin.x: hourMarker.width / 2
                    origin.y: hourMarker.height / 2
                    angle: 0
                    function setAngle(newAngle)
                    {
                        while (newAngle < angle)
                        {
                            newAngle += 360
                        }
                        angle = Math.abs(angle - newAngle) > 180 ? newAngle - 360 : newAngle
                    }

                    Behavior on angle
                    {
                        NumberAnimation {
                            duration: Constants.fastAnimationDuration
                        }
                    }
                }]
        }
        Rectangle
        {
            x: theOuterPathView.x + theOuterPathView.width / 2
            y: theOuterPathView.y + theOuterPathView.height / 2
            width: 1
            height: -hourTranslate.y
            color: hourMarker.color
            rotation: hourRotation.angle + 180
            transformOrigin: Item.TopLeft
        }

        PathView
        {
            id: theOuterPathView
            interactive: false
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
            delegate: YACText
            {
                text: index
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            model: 12
            property double theMiddle: width / 2
            property double theRadius: width / 2.8
            path: Path {
                startX: theOuterPathView.theMiddle
                startY: theOuterPathView.theMiddle - theOuterPathView.theRadius

                PathArc
                {
                    x: theOuterPathView.theMiddle
                    y: theOuterPathView.theMiddle + theOuterPathView.theRadius; radiusX: theOuterPathView.theRadius; radiusY: theOuterPathView.theRadius; useLargeArc: true }
                PathArc
                {
                    x: theOuterPathView.theMiddle
                    y: theOuterPathView.theMiddle - theOuterPathView.theRadius
                    radiusX: theOuterPathView.theRadius; radiusY: theOuterPathView.theRadius; useLargeArc: true }
            }
        }
        PathView
        {
            id: theInnerPathView
            interactive: false
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
            delegate: YACText
            {
                text: index + 12
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            model: 12
            property double theMiddle: width / 2
            property double theRadius: width / 4
            path: Path {
                startX: theInnerPathView.theMiddle
                startY: theInnerPathView.theMiddle - theInnerPathView.theRadius

                PathArc
                {
                    x: theInnerPathView.theMiddle
                    y: theInnerPathView.theMiddle + theInnerPathView.theRadius; radiusX: theInnerPathView.theRadius; radiusY: theInnerPathView.theRadius; useLargeArc: true }
                PathArc
                {
                    x: theInnerPathView.theMiddle
                    y: theInnerPathView.theMiddle - theInnerPathView.theRadius
                    radiusX: theInnerPathView.theRadius; radiusY: theInnerPathView.theRadius; useLargeArc: true }
            }
        }
        MouseArea
        {
            anchors.fill: parent
            onMouseXChanged: calculate(mouseX, mouseY)
            onMouseYChanged: calculate(mouseX, mouseY)
            function calculate(x, y)
            {
                x -= width / 2
                y -= height / 2
                var squareRadius = x * x + y * y
                var radius = Math.sqrt(squareRadius)
                var innerSquareRadius = theInnerPathView.theRadius * theInnerPathView.theRadius
                var outerSquareRadius = theOuterPathView.theRadius * theOuterPathView.theRadius
                var border = innerSquareRadius + (outerSquareRadius - innerSquareRadius) / 2
                hourTranslate.y = squareRadius < border ? -theInnerPathView.theRadius : -theOuterPathView.theRadius
                var angle = Math.trunc(Math.trunc((Math.atan2(y, x) * 180 / Math.PI + 90 + 360) % 360 + 15) / 30) * 30
                hourRotation.setAngle(angle)
                var hour = Math.trunc(angle / 30);
                if (squareRadius < border)
                {
                    hour += 12
                }
                else
                {
                    if (hour == 12)
                    {
                        hour = 0
                    }
                }

                if (hour == 24)
                {
                    hour = 0
                }

                theTime = Helper.setHour(theTime, hour)
            }
            onReleased: hourItem.visible = false
        }
    }
    Rectangle
    {
        color: "grey"
        id: minuteItem
        anchors.centerIn: parent
        width: parent.width
        height: width
        visible: !hourItem.visible
        Rectangle
        {
            id: minuteMarker
            radius: width / 2
            width: sizeText.contentWidth * 2
            height: width
            color: "lightgrey"
            anchors.centerIn: theMinutePathView
            transform: [
                Translate
                {
                    id: minuteTranslate
                    x: 0
                    y: -theMinutePathView.theRadius
                    Behavior on y
                    {
                        NumberAnimation {
                            duration: Constants.fastAnimationDuration
                        }
                    }
                },

                Rotation
                {
                    id: minuteRotation
                    origin.x: minuteMarker.width / 2
                    origin.y: minuteMarker.height / 2
                    angle: 0
                    function setAngle(newAngle)
                    {
                        while (newAngle < angle)
                        {
                            newAngle += 360
                        }
                        angle = Math.abs(angle - newAngle) > 180 ? newAngle - 360 : newAngle
                    }

                    Behavior on angle
                    {
                        NumberAnimation {
                            duration: Constants.fastAnimationDuration
                        }
                    }
                }]
        }
        Rectangle
        {
            x: theMinutePathView.x + theMinutePathView.width / 2
            y: theMinutePathView.y + theMinutePathView.height / 2
            width: 1
            height: -minuteTranslate.y
            color: minuteMarker.color
            rotation: minuteRotation.angle + 180
            transformOrigin: Item.TopLeft
        }

        PathView
        {
            id: theMinutePathView
            interactive: false
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
            delegate: YACText
            {
                text: index * 5
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            model: 12
            property double theMiddle: width / 2
            property double theRadius: width / 2.8
            path: Path {
                startX: theMinutePathView.theMiddle
                startY: theMinutePathView.theMiddle - theMinutePathView.theRadius

                PathArc
                {
                    x: theMinutePathView.theMiddle
                    y: theMinutePathView.theMiddle + theMinutePathView.theRadius
                    radiusX: theMinutePathView.theRadius
                    radiusY: theMinutePathView.theRadius; useLargeArc: true }
                PathArc
                {
                    x: theMinutePathView.theMiddle
                    y: theMinutePathView.theMiddle - theMinutePathView.theRadius
                    radiusX: theMinutePathView.theRadius
                    radiusY: theMinutePathView.theRadius; useLargeArc: true }
            }
        }
        MouseArea
        {
            anchors.fill: parent
            onMouseXChanged: calculate(mouseX, mouseY)
            onMouseYChanged: calculate(mouseX, mouseY)
            function calculate(x, y)
            {
                x -= width / 2
                y -= height / 2
                var squareRadius = x * x + y * y
                var radius = Math.sqrt(squareRadius)
                var angle = Math.trunc(Math.trunc((Math.atan2(y, x) * 180 / Math.PI + 90 + 360) % 360 + 3) / 6) * 6
                minuteRotation.setAngle(angle)
                var minute = Math.trunc(angle / 6);
                if (minute == 60)
                {
                    minute = 0
                }
                theTime = Helper.setMinute(theTime, minute)
            }
        }
    }
    Row
    {
        YACText
        {
            id: hourText
            width: timeItem.width / 2
            text: timeItem.theHour
            horizontalAlignment: Text.AlignHCenter
            MouseArea
            {
                anchors.fill: parent
                onClicked: hourItem.visible = true
            }
        }
        YACText
        {
            id: minuteText
            width: timeItem.width / 2
            text: timeItem.theMinute
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
