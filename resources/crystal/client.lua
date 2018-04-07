local myobject = CreateObject()
local mytexture= CreateTexture( "data/textures/emeraldo2.png" )
SetTexture(myobject,mytexture)
SetPosition(myobject,0,0)

TriggerServerEvent("onClientDone")