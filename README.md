# Compound
C++ written 2D multiplayer/LAN game engine sprinkled with some Lua extensibility magic.

## Why Compound?
- Cross-platform
- Controller-compatible
- Multiplayer
- Scriptable
- Lightweight
- Simple and easy to use

## How does it work?
- It connects to a compound server
- It downloads the gamemode ( scripts + assets )
- It lets you play with your friends

Every server is different. If you use different scripts you get different gamemodes. You will be able to play different games within the same client without having to install anything else manually.

## What happens when you connect to a server?
- It downloads the necessary files
- It downloads the game state
- Sends some confirmation packets back and forth
- Lets you join the action

## What is the "Awesome event system" ?
Usually when you have a multiplayer game you have to define the protocol yourself. Moving bytes around in certain order gets messy sometimes and this is why Compound uses events for this.

The mechanic works like this : Let's say you want the server to get players' nicknames when they connect. It is easy to define an event and to trigger it so you actually get that information over the network.

Compound is made of two parts, so let's start with the server :

```
RegisterServerEvent( "onPlayerSendNickname" )
AddEventHandler( "onPlayerSendNickname", function( nickname )
  print ( "Player " .. nickname .. " has joined!" ) -- this outputs to the server's console
end)
```
The client, of course, has to send it with something like this:
```
TriggerServerEvent("onPlayerSendNickname","John Doe")
```
Compound is programmed so that you can pass up to 8 arguments at a time with the event system. You can mix lua strings and numbers, they will arrive in the right order. ( The value of 8 comes from the fact that you need 1 byte to encode 8 variable types of either strings or numbers ).

You can also trigger an event with no arguments, and you won't need the encoding byte anymore.

## What is the structure of the engine?
- The server - serves the files and handles connections
- The client - runs Lua scripts to create the gameplay
- There is a cool debugging console on the client activated by pressing on BACKTICK
- There are objects, which get sync'd automatically and have:
  - Position
  - Velocity
  - Rotation
  - Texture
  - Friction
- Events, Event Handlers and Triggers

## Any more great powers?
If you apply "RegisterAsServerObject( objectID )" on an object, any modification applied ( such as SetPosition or SetTexture ) will be _**AUTOMATICALLY**_ synchronized with the server.

You can also make a server that serves game files for single-player games. ( and eventually also use the server for scoreboards, data storage, etc .. )

## Things that aren't yet implemented, but will be part of Compound
Time is a precious thing to have, and Compound is currently still in alpha. We're doing our best to develop it, but we will add these :
- Master servers ( So you can just press a button and see a list of available servers )
- LAN server broadcasting ( If you create a server, members of the Local Area Network will be able to join without having to ask for the ip )
- A built-in synthesizer ( at least the traditional 4 basic soundwaves )
- Basic means of storing information ( for the server ) such as SQL connections or writing to files
- Utility functions such as lua table encoding and decoding
- More API functions like HTTP Requests
- RCON compatibility for the server console
- Config files
- Auth servers
- Server migration techniques ( in case one server fails suddenly, redirect the players and eventually the game state to another shard )
- Chat system
- Audio streaming

## Examples of the API functions
```
objectID = CreateObject() -- Creates default object and returns the ID

SetPosition( objectID, x, y )

x, y = GetPosition( objectID )

SetRotation( objectID, rotation )

SetTexture( objectID, textureID ) -- the textureID is bound to a texture depending on the servers' settings

SetVelocity( objectID, vx, vy )

SetFriction( objectID, friction ) -- Compound also has a friction/velocity physics system, so you can make smooth moving objects

KillObject( objectID ) -- Destroy the object

labelID = CreateLabel(x,y,text) -- A label is an object that draws text on screen

SetLabelPosition, GetBounds, SetText, SetFontSize, SetColor, DestroyLabel

width, height = GetWindowSize() -- Convenient function that returns window
dimensions

RegisterAsServerObject( objectID ) -- This turns sync on for the specified object

RegisterClientEvent( eventName ) -- Part of the awesome event system

AddEventHandler( eventName, handlerFunction ) -- Also part of the awesome event system

TriggerServerEvent( eventName, arg1, arg2, ... , arg8 )
```

## Examples of the API ( built-in ) events
```
  Serverside :
  "onPlayerConnect"
  "onPlayerDrop"

  Clientside :
  "onKeyPress" ( with a keyCode as argument )
  "onFrame"
  "onTextEntered" ( with a character as argument )
```
## How do I test it?
This build comes packed with a simple game called __Virus Invaders__.

You have to download the server and client ( we will put a link here ). Then :
- Start the server
- Start client 1, open the debugging console by pressing BACKTICK and type : connect("127.0.0.1") and hit Enter
- Do the same thing with a second client. ( Eventually you can put a different ip there if you actually want to test it on different computers )
- Hope that the brittle protocol doesn't leak anything. It is still early alpha and at the moment it is just proof of concept. Don't expect it to work like a 3 year old project developed by a big company. If it crashes whatsoever, retry these steps from the beggining.
- If there aren't any problems, close the debugging console by pressing BACKTICK and you should be able to see viruses appear onscreen with some hexcodes above their head. You have a text input in the bottom-left corner. Type the names above a virus' head and hit Enter. You will give the turret a command to exterminate that virus. ( Your friend over the network can also do that in the same time ).

## Used Libraries
<a href="https://www.sfml-dev.org/"><img src="https://www.sfml-dev.org/download/goodies/sfml-logo-small.png" height="50" hspace="20" /></a>
<br><br>
<a href="http://www.lua.org/"><img src="http://www.andreas-rozek.de/Lua/Lua-Logo_128x128.png" height="100" hspace="100" /></a>
