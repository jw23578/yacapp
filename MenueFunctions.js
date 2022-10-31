.pragma library

function openTarget(yacApp, stackView, target, theMenue)
{
    if (target == "back")
    {
        stackView.pop()
        return
    }
    if (target == "home")
    {
        stackView.pop(null)
        return
    }
    if (target == "logout")
    {
        yacApp.yesNoQuestion(qsTr("really logout?"), null,
                             function()
                             {
                                 yacApp.logout()
                             },
                             function() {});
        return
    }

    if (target == "clear")
    {
        stackView.pop(null)
        return
    }

    stackView.push("qrc:/qml/SuperForm.qml", {
                       "config": yacApp.getConfig(target),
                       "stackView": stackView,
                       "theMenue": theMenue
                   }
                   )
}
