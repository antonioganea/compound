// RegisterServerEvent ( eventName, eventCode )
// This is left for debug
/*
int l_RegisterServerEvent(lua_State* L) {

    char eventName[256];

    strcpy(eventName,lua_tostring(L,-2));
    int eventCode = lua_tonumber(L,-1);

    SyncManager::registerServerEvent(eventName,eventCode);

    return 0;
}*/

// TriggerServerEvent ( eventName )
int l_TriggerServerEvent(lua_State* L) {

    char eventName[256];
    strcpy(eventName,lua_tostring(L,1));

    int args = lua_gettop(L);

    if ( args == 1){
        SyncManager::triggerServerEvent(eventName);
    }else{
        sf::Packet newPacket;

        float floatBuffer;
        char stringBuffer[256];

        sf::Uint16 eventCode = SyncManager::getServerEventCode(eventName);
        newPacket << eventCode;
        sf::Uint8 encoding = 0;
        sf::Uint8 index = 1;
        for ( int i = 2; i <= args; i++ ){
            if ( !lua_isnumber(L,i) ){//0 for numbers, 1 for strings
                encoding = encoding | index;
            }
            index = index << 1;
        }

        newPacket << encoding;
        for ( int i = 2; i <= args; i++ ){
            if ( lua_isnumber(L,i) ){
                floatBuffer = lua_tonumber(L,i);
                newPacket << floatBuffer;
            }else{
                strcpy(stringBuffer,lua_tostring(L,i));
                newPacket << stringBuffer;
            }
        }

        SyncManager::sendTCPMessage(newPacket);

    }

    return 0;
}

// RegisterClientEvent ( eventName )
int l_RegisterClientEvent(lua_State* L) {
    char eventName[256];

    strcpy(eventName,lua_tostring(L,-1));

    SyncManager::requestClientEvent(eventName);

    return 0;
}

// AddEventHandler ( eventName, eventHandler )
int l_AddEventHandler(lua_State* L) {

    //char eventName[256];
    //strcpy(eventName,lua_tostring(L,-2));

    //TODO : checktypes

    lua_settable(L, LUA_REGISTRYINDEX);

    return 0;
}
