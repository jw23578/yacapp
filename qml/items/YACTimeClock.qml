import QtQuick 2.15

Item
{
    PathView
    {
        id: theOuterPathView
        interactive: false
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        delegate: YACText
        {
            text: index == 0 ? 0 : index + 12
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        model: 12
        property double theMiddle: width / 2
        property double theRadius: width / 3
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
}
