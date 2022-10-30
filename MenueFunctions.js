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
    if (target == "clear")
    {
        stackView.pop(null)
    }

    stackView.push("qrc:/qml/SuperForm.qml", {
                       "config": yacApp.getConfig(target),
                       "stackView": stackView,
                       "theMenue": theMenue
                   }
                   )
}
