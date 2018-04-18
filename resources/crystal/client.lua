print("Script loaded - green circle");

local speed = 10

local myObject = CreateObject()
SetPosition( myObject, 560, 500 )

local myObject2 = CreateObject()
SetPosition( myObject2, 500, 500 )

local x = 100
local y = 100

--[[
W - 22
A - 0
S - 18
D - 3
X - 23
]]

local color = 1

SetTexture( myObject, 5 )
SetFriction( myObject, 0.1 )
SetFriction( myObject2, 0.1 )

local rot = 0

AddEventHandler( "onKeyPress", function( keyCode )
    x = 0
    y = 0
    local rot1 = rot
    if keyCode == 22 then -- W
        y = -speed
        rot = 0
    elseif keyCode == 18 then -- S
        y = speed
        rot = 180
    elseif keyCode == 0 then -- A
        x = -speed
        rot = 270
    elseif keyCode == 3 then -- D
        x = speed
        rot = 90
    elseif keyCode == 23 then -- X
        SetTexture( myObject2, color )
        color = color + 1
        if color > 5 then color = 1 end
        SetTexture ( myObject, color )
        --KillObject( myObject )
    end

    --SetPosition( myObject, x, y )
    SetVelocity( myObject, x, y )
    SetVelocity( myObject2, -x, -y )
    if rot1 ~= rot then
        SetRotation( myObject, rot )
        SetRotation( myObject2, 180+rot )
    end
end
)

print("loaded!")
