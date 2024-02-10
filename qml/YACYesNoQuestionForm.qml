import QtQuick 2.15
import "items"

Rectangle
{
    anchors.fill: parent
    visible: false
    id: theYesNoQuestionForm
    property var itemToFocus: null
    property var yesCallback: null
    property var noCallback: null
    function show(question, itemToFocus, yesCallback, noCallback)
    {
        questionText.text = question
        theYesNoQuestionForm.itemToFocus = itemToFocus
        theYesNoQuestionForm.yesCallback = yesCallback
        theYesNoQuestionForm.noCallback = noCallback
        visible = true
    }
    function hide()
    {
        visible = false
        if (itemToFocus != null)
        {
            itemToFocus.forceActiveFocus()
        }
    }

    function yes()
    {
        if (yesCallback != null)
        {
            yesCallback()
        }
        hide()
    }
    function no()
    {
        if (noCallback != null)
        {
            noCallback()
        }
        hide()
    }

    YACRectangle
    {
        anchors.centerIn: parent
        color: "#A9CEA5"
        radius: width / 10
        radiusTopLeft: true
        radiusBottomRight: radiusTopLeft
        width: parent.width * 3 / 4
        height: parent.height * 3 / 5
        YACText
        {
            id: questionText
            anchors.centerIn: parent
            width: parent.width * 3 / 4
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Row
    {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: height
        height: yesButton.height
        YACButton
        {
            id: yesButton
            text: qsTr("Yes")
            onClicked: yes()
        }
        Item
        {
            height: 1
            width: yesButton.width / 2
        }
        YACButton
        {
            text: qsTr("No")
            width: yesButton.width
            onClicked: no()
        }
    }

}
