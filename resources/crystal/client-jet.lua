print("Script loading - Virus Invaders 2.0");

local width, height = GetWindowSize()

--[[
RegisterClientEvent("invaders:clientTest")

AddEventHandler("invaders:clientTest",function(arg1, arg2, arg3, arg4)
    print("Server to client event triggering test successful!" .. tostring(arg1) .. " " .. tostring(arg2) .. " " .. tostring(arg3) .. " " .. tostring(arg4) )
end)
]]

RegisterClientEvent("invaders:spawnCritter")
AddEventHandler("invaders:spawnCritter",function(id, texture, x, y)
    --print("invaders:spawnCritter  " .. tostring(math.floor(id)) .. " " .. tostring(texture) .. " 0x" .. tostring(code) .. " " .. tostring(math.floor(x)) )
    if type(code) == "number" then
        code = math.floor(code)
    end
    spawnCritter( math.floor(id), texture, code, math.floor(x) )
end)

local critters = {}

function spawnCritter( id, texture, x, y )
    newCritter = CreateObject()
    SetTexture( newCritter, texture )
    SetPosition ( newCritter, x, y )
    SetFriction ( newCritter, 0.2 )

    critters[id] = {
    ["object"] = newCritter,
    ["timer"] = 0,
    ["behaviour"] = math.random(1,3)
    }
end

local it = 1

function killCritter(k)
    KillObject(critters[k].object)
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
SetTexture( myTurret, "spaceship.png" )
SetPosition( myTurret, width/2, height/2 )
SetFriction( myTurret, 0.2 )
RegisterAsServerObject( myTurret )
turretDirection = 90

function rotateTurret( angle )
    turretDirection = angle
    SetRotation( myTurret, 90-turretDirection )
end

function computeVelocity( multiplier )
    local rad_dir = math.rad(turretDirection)
    local xVelocity = math.cos(rad_dir) * multiplier
    local yVelocity = math.sin(rad_dir) * multiplier
    return xVelocity, -yVelocity
end

function thrustTurret( multiplier )
    local rad_dir = math.rad(turretDirection)
    local xVelocity = math.cos(rad_dir) * multiplier
    local yVelocity = math.sin(rad_dir) * multiplier
    SetVelocity(myTurret, xVelocity, -yVelocity)
    --print ( "Thrust : direction " .. turretDirection .. " vel : " .. xVelocity .. " " .. yVelocity )
end

rotationAcceleration = 0
rotationSpeed = 0



-- W 22
-- A 0
-- S 18
-- D 3
-- Space 57

Keys = { W=22, A=0, S=18, D=3, Space=57 }

PressedKeys = {}

AddEventHandler( "onKeyPress", function( keyCode )
    PressedKeys[keyCode] = true
    --[[
    if keyCode == Keys.W then
        thrustTurret(10)
    elseif keyCode == Keys.S then
        thrustTurret(-10)
    elseif keyCode == Keys.D then
        rotationAcceleration = 1
    elseif keyCode == Keys.A then
        rotationAcceleration = -1
    end]]

    if keyCode == Keys.Space then
        spawnCritter(it,math.random(1,8)..".png",math.random(0,width),math.random(0,height))
        it = it + 1
    end
end)

AddEventHandler( "onKeyRelease", function( keyCode )
    PressedKeys[keyCode] = false
    --[[
    if keyCode == Keys.W then
        thrustTurret(10)
    elseif keyCode == Keys.S then
        thrustTurret(-10)
    elseif keyCode == Keys.D then
        rotationAcceleration = 1
    elseif keyCode == Keys.A then
        rotationAcceleration = -1
    end]]
end)


local spawnTimer = 0
local clocker = false

RegisterClientEvent("invaders:setClock")
AddEventHandler("invaders:setClock", function()
    clocker = true
    print("HEY")
end)

TriggerServerEvent("invaders:askForClock")

extraBordering = 100 -- extra space for out of borders deletions

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
        if ( cy > height + extraBordering or cy < -extraBordering or cx > width+extraBordering or cx < -extraBordering ) then
            killCritter(k)
            print("Critter out of borders")
        else
            v.timer = v.timer + 1
            if ( v.timer == 60 ) then
                v.timer = 0
                SetVelocity(v.object,0,5)
            end
        end
    end


    -- Turret Direction
    local newdir = turretDirection
    rotationSpeed = rotationSpeed + rotationAcceleration
    newdir = newdir + rotationSpeed
    rotationSpeed = rotationSpeed - rotationSpeed*0.2
    rotateTurret( newdir )

    -- Controls
    if PressedKeys[Keys.W] then
        thrustTurret(5)
    elseif PressedKeys[Keys.S] then
        thrustTurret(-5)
    end
    if PressedKeys[Keys.A] then
        rotationAcceleration = 1.1
    elseif PressedKeys[Keys.D] then
        rotationAcceleration = -1.1
    else
        rotationAcceleration = 0
    end
end)

-- Scoreboard
newLabel = CreateLabel( 0, 0, "Antonio Ganea" )
SetFontSize(newLabel,32)
SetColor(newLabel,1)

newLabel = CreateLabel( 0, 50, "Radu Tache" )
SetFontSize(newLabel,32)
SetColor(newLabel,1)

print("Script loaded! Virus Invaders 2.0")