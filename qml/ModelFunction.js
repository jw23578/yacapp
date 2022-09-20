.pragma library

function find(model, needle)
{
    for (var i = 0; i < model.length; ++i)
    {
        if (model[i] == needle)
        {
            return i;
        }
    }
    return -1;
}
