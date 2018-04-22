print("Script loaded - green circle");

local width, height = GetWindowSize()

--[[
RegisterClientEvent("invaders:clientTest")

AddEventHandler("invaders:clientTest",function(arg1, arg2, arg3, arg4)
    print("Server to client event triggering test successful!" .. tostring(arg1) .. " " .. tostring(arg2) .. " " .. tostring(arg3) .. " " .. tostring(arg4) )
end)
]]

RegisterClientEvent("invaders:spawnCritter")
AddEventHandler("invaders:spawnCritter",function(id, texture, code, x)
    --print("invaders:spawnCritter  " .. tostring(math.floor(id)) .. " " .. tostring(texture) .. " 0x" .. tostring(code) .. " " .. tostring(math.floor(x)) )
    if type(code) == "number" then
        code = math.floor(code)
    end
    spawnCritter( math.floor(id), texture, code, math.floor(x) )
end)

function e()
    TriggerServerEvent( "invaders:virus", 500 )
end

local critters = {}

function spawnCritter( id, texture, code, x )
    newCritter = CreateObject()
    SetTexture( newCritter, texture )
    newLabel = CreateLabel( 0, 0, "0x" .. tostring(code) )
    SetFontSize(newLabel,32)
    SetColor(newLabel,1)

    SetPosition ( newCritter, x, -50 )
    SetVelocity ( newCritter, 0, 20 )
    SetFriction ( newCritter, 0.2 )

    critters[id] = { ["object"] = newCritter, ["code"] = "0X"..code, ["label"] = newLabel, ["timer"] = 0 }
end

local it = 1

function killCritter(k)
    KillObject(critters[k].object)
    DestroyLabel(critters[k].label)
    critters[k] = nil
end

local projectiles = {}
local projectileID = 1

function shootProjectile( turret, targetCritter )
    local tx, ty = GetPosition( turret )

    local newProjectile = CreateObject()
    SetPosition(newProjectile,tx,ty)
    SetTexture(newProjectile,"projectile.png")
    SetVelocity(newProjectile,0,-5)

    projectiles[projectileID] = { ["object"] = newProjectile, ["target"] = targetCritter }
    projectileID = projectileID + 1
end

-- Turret & projectiles --
local myTurret = CreateObject()
SetTexture( myTurret, "12.png" )
SetPosition( myTurret, width/2-30, height-100 )

local turretQueue = {}

-- Game Console ( "Hacker" console ) --

local hackerLabel = CreateLabel(10,10,"Command   ")
SetLabelPosition(hackerLabel,30,height-30)
SetFontSize(hackerLabel,16)
SetColor(hackerLabel,1) -- make it green
local hackerInput = ""
local justEnteredCommand = false

function updateHackerLabel() SetText( hackerLabel, "Command   " .. hackerInput); justEnteredCommand = false; SetColor(hackerLabel,1) end

local queueNext = 1
function enqueueForKilling(k)
    SetColor(critters[k].label,2)
    turretQueue[queueNext] = k
    queueNext = queueNext + 1
end

RegisterClientEvent("invaders:enqueue")
AddEventHandler("invaders:enqueue", function( critterID )
    enqueueForKilling(critterID)
end)


function submitCommand()
    if justEnteredCommand then
        hackerInput = ""
        updateHackerLabel()
        return
    end

    local hitsomething = false

    for k,v in pairs( critters ) do
        print ("Comparing " .. hackerInput .. " and " .. v.code )
        if ( hackerInput == v.code ) then
            --enqueueForKilling(k)
            TriggerServerEvent("invaders:commandKill",k)
            hitsomething = true
        end
    end

    if hitsomething then
        hackerInput = ""
        updateHackerLabel()
    else
        SetColor(hackerLabel,2)
    end

    justEnteredCommand = true
end

AddEventHandler( "onTyped", function( character )
    hackerInput = hackerInput .. string.upper(character)
    updateHackerLabel()
end)

AddEventHandler( "onKeyPress", function( keyCode )
    if keyCode == 59 then -- Backspace : 59
        hackerInput = hackerInput:sub(1, -2)
        updateHackerLabel()
    elseif keyCode == 58 then -- Return : 58
        submitCommand()
    end
end)




local spawnTimer = 0
local clocker = false

RegisterClientEvent("invaders:setClock")
AddEventHandler("invaders:setClock", function()
    clocker = true
    print("HEY")
end)

TriggerServerEvent("invaders:askForClock")

AddEventHandler( "onFrame", function( )

    if clocker then
        spawnTimer = spawnTimer + 1
        if spawnTimer >= 180 then
            TriggerServerEvent("invaders:spawnTimer")
            spawnTimer = 0
        end
    end

    -- Critter update
    for k,v in pairs ( critters ) do
        local cx,cy = GetPosition(v.object)
        if ( cy > height ) then
            KillObject(v.object)
            DestroyLabel(v.label)
            critters[k] = nil
            print("Destroyed critter")
        else
            SetLabelPosition(v.label,cx,cy-50)
            v.timer = v.timer + 1
            if ( v.timer == 180 ) then
                v.timer = 0
                SetVelocity(v.object,0,15)
            end
        end
    end

    -- Projectile update
    for k,v in pairs ( projectiles ) do
        local px,py = GetPosition(v.object)
        local tx,ty = GetPosition(critters[v.target].object)
        if ( py-75 < ty ) then
            killCritter(v.target)
            KillObject(v.object)
            projectiles[k] = nil
            print("Shot critter")
        end
    end

    if turretNextTarget == nil then
        for k,v in pairs ( turretQueue ) do
            turretNextTarget = v
            turretQueue[k] = nil
            break
        end
    end

    if turretNextTarget ~= nil then
        local tx, ty = GetPosition ( critters[turretNextTarget].object )
        local mx, my = GetPosition ( myTurret )
        if ( tx < mx-5 ) then
            SetPosition( myTurret, mx-3, my )
        elseif ( tx > mx+5 ) then
            SetPosition( myTurret, mx+3, my )
        else
            shootProjectile( myTurret, turretNextTarget )
            turretNextTarget = nil;
        end
    end


end)

print("loaded!")

--[[
local speed = 10

local myObject = CreateObject()
SetPosition( myObject, 560, 500 )

local myObject2 = CreateObject()
SetPosition( myObject2, 500, 500 )

local x = 100
local y = 100

-- W - 22
-- A - 0
-- S - 18
-- D - 3
-- X - 23


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
]]
