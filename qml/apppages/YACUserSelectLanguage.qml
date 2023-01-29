import QtQuick 2.15
import "../items"

Rectangle
{
    id: selectLanguage
    anchors.fill: parent
    signal closeClicked();
    function goLanguage(language)
    {
        yacApp.switchLanguage(language)
        closeClicked()
    }

    YACPageColumn
    {
        YACButton
        {
            text: qsTr("English")
            onClicked: goLanguage("")
        }
        YACButton
        {
            text: qsTr("German")
            onClicked: goLanguage("de_DE")
        }
        YACButton
        {
            text: qsTr("Close")
            onClicked: selectLanguage.closeClicked()
        }
    }
}
