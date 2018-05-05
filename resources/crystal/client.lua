print("Script loading - Virus Invaders 2.0");

local width, height = GetWindowSize()

--[[
RegisterClientEvent("invaders:clientTest")

AddEventHandler("invaders:clientTest",function(arg1, arg2, arg3, arg4)
    print("Server to client event triggering test successful!" .. tostring(arg1) .. " " .. tostring(arg2) .. " " .. tostring(arg3) .. " " .. tostring(arg4) )
end)
]]

local playerSide

RegisterClientEvent("invaders:spawnCritter")
AddEventHandler("invaders:spawnCritter",function(id, texture, code, x, your)
    --print("invaders:spawnCritter  " .. tostring(math.floor(id)) .. " " .. tostring(texture) .. " 0x" .. tostring(code) .. " " .. tostring(math.floor(x)) )
    if type(code) == "number" then
        code = math.floor(code)
    end
    spawnCritter( math.floor(id), texture, code, math.floor(x), your )
end)

local critters = {}

function spawnCritter( id, texture, code, x, your )
    newCritter = CreateObject()
    SetTexture( newCritter, texture )
    newLabel = CreateLabel( 0, 0, tostring(code) )
    SetFontSize(newLabel,16)
    SetColor(newLabel,1)

    SetPosition ( newCritter, x, -50 )
    SetVelocity ( newCritter, 0, 20 )
    SetFriction ( newCritter, 0.2 )

    critters[id] = { ["object"] = newCritter, ["code"] = code, ["label"] = newLabel, ["timer"] = 0, ["your"] = your }
end

function killCritter(k)
    if ( critters ) then
        if critters[k] then
            KillObject(critters[k].object)
            DestroyLabel(critters[k].label)
            critters[k] = nil
            PlaySound("dead.wav")
        end
    end
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

    PlaySound("shoot.wav")
end

-- Turret & projectiles --
local myTurret = CreateObject()
SetTexture( myTurret, "12.png" )
SetPosition( myTurret, width/4-30, height-100 )

local otherTurret = CreateObject()
SetTexture( otherTurret, "12.png" )
SetPosition( otherTurret, width*3/4-30, height-100)


local myTurretQueue = {}
local otherTurretQueue = {}

-- Game Console ( "Hacker" console ) --

local hackerLabel = CreateLabel(10,10,"Command   ")
SetLabelPosition(hackerLabel,30,height-30)
SetFontSize(hackerLabel,16)
SetColor(hackerLabel,1) -- make it green
local hackerInput = ""
local justEnteredCommand = false

function updateHackerLabel() SetText( hackerLabel, "Command   " .. hackerInput); justEnteredCommand = false; SetColor(hackerLabel,1) end

local myQueueNext = 1
function enqueueMineForKilling(k)
    if critters[k] then
        SetColor(critters[k].label,2)
        myTurretQueue[myQueueNext] = k
        myQueueNext = myQueueNext + 1
    end
end

local otherQueueNext = 1
function enqueueOtherForKilling(k)
    if critters[k] then
        SetColor(critters[k].label,2)
        otherTurretQueue[otherQueueNext] = k
        otherQueueNext = otherQueueNext + 1
    end
end

RegisterClientEvent("invaders:enqueue")
AddEventHandler("invaders:enqueue", function( critterID, turret )
    if turret == 1 then
        enqueueMineForKilling(critterID)
        print ("Enqueued mine for killing " .. critterID )
    else
        enqueueOtherForKilling(critterID)
        print ("Enqueued other for killing " .. critterID )
    end
end)


function submitCommand()
    if justEnteredCommand then
        hackerInput = ""
        updateHackerLabel()
        return
    end

    local hitsomething = false

    for k,v in pairs( critters ) do
        if v.your == 1 then
            print ("Comparing " .. hackerInput .. " and " .. v.code )
            if ( hackerInput == v.code ) then
                --enqueueMineForKilling(k)
                TriggerServerEvent("invaders:commandKill",k, playerSide)
                print ( "HIT critter" .. k )
                hitsomething = true
            end
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

local MYTURRET

RegisterClientEvent("invaders:playerSide")
AddEventHandler("invaders:playerSide",function( playerside )
    playerSide = playerside
    if playerside == 1 then
        MYTURRET = myTurret
    else
        MYTURRET = otherTurret
    end
    TriggerServerEvent("invaders:nickname", playerside, GetNickname())
end)

updateTimerMax = 180

RegisterClientEvent("invaders:updateTimer")
AddEventHandler("invaders:updateTimer",function( timer )
    updateTimerMax = timer
end)


local announceBoard = nil
local announceTimer = 0

RegisterClientEvent("invaders:announce")
AddEventHandler("invaders:announce",function( stringToAnnounce )
    announceBoard = CreateLabel( width/2, height-50, stringToAnnounce )
    SetFontSize(announceBoard,128)
    _,_,labelWidth,labelHeight = GetBounds( announceBoard )
    --SetColor(announceBoard,1)
    SetLabelPosition(announceBoard,width/2-labelWidth/2, height/2-labelHeight)
    announceTimer = 120
end
)

AddEventHandler( "onFrame", function( )

    if announceTimer > 0 then
        announceTimer = announceTimer-1
        if announceTimer < 10 then
            SetColor(announceBoard,2)
        elseif announceTimer < 5 then
            SetColor(announceBoard,1)
        end
    elseif announceBoard ~= nil then
        DestroyLabel(announceBoard)
        announceBoard=nil
    end

    if clocker then
        spawnTimer = spawnTimer + 1
        if spawnTimer >= 10 then
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
            if ( v.timer == updateTimerMax ) then
                v.timer = 0
                SetVelocity(v.object,0,15)
            end
        end
    end

    -- Projectile update
    for k,v in pairs ( projectiles ) do
        local px,py = GetPosition(v.object)
        if critters[v.target] then
            local tx,ty = GetPosition(critters[v.target].object)
            if ( py-75 < ty ) then
                killCritter(v.target)
                KillObject(v.object)
                projectiles[k] = nil
                print("Shot critter")
            end
        end
    end

    if myTurretNextTarget == nil then
        for k,v in pairs ( myTurretQueue ) do
            myTurretNextTarget = v
            myTurretQueue[k] = nil
            break
        end
    end

    if otherTurretNextTarget == nil then
        for k,v in pairs ( otherTurretQueue ) do
            otherTurretNextTarget = v
            otherTurretQueue[k] = nil
            break
        end
    end

    if myTurretNextTarget ~= nil then
        if critters[myTurretNextTarget] then
            local tx, ty = GetPosition ( critters[myTurretNextTarget].object )
            local mx, my = GetPosition ( myTurret )
            if ( tx < mx-5 ) then
                SetPosition( myTurret, mx-3, my )
            elseif ( tx > mx+5 ) then
                SetPosition( myTurret, mx+3, my )
            else
                shootProjectile( myTurret, myTurretNextTarget )
                myTurretNextTarget = nil;
            end
        end
    end

    if otherTurretNextTarget ~= nil then
        if critters[otherTurretNextTarget] then
            local tx, ty = GetPosition ( critters[otherTurretNextTarget].object )
            local mx, my = GetPosition ( otherTurret )
            if ( tx < mx-5 ) then
                SetPosition( otherTurret, mx-3, my )
            elseif ( tx > mx+5 ) then
                SetPosition( otherTurret, mx+3, my )
            else
                shootProjectile( otherTurret, otherTurretNextTarget )
                otherTurretNextTarget = nil;
            end
        end
    end


end)

local scoreLabels = {}

scoreLabels[1] = CreateLabel( width/4, height-50, "PLAYER 1" )
SetFontSize(newLabel,16)
SetColor(newLabel,1)

scoreLabels[2] = CreateLabel( width*3/4, height-50, "PLAYER 2" )
SetFontSize(newLabel,16)
SetColor(newLabel,1)

RegisterClientEvent("invaders:score")
AddEventHandler("invaders:score",function( tag, playerside )
    SetText(scoreLabels[playerside], tag)
end)

print("Script loaded! Virus Invaders 2.0")