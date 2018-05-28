// SetPosition( object, x, y )
int l_SetPosition(lua_State* L) {
    sf::Uint16 objectID;
    float x,y;

    if ( luaL_checkinteger(L,-3) ){
        objectID = lua_tonumber(L,-3);
    }

    x = lua_tonumber(L,-2);
    y = lua_tonumber(L,-1);

    Object * obj = StageManager::gameState->getObject(objectID);
    //std::cout << "getObject : " << obj << std::endl;
    if ( obj != 0 ){
        sf::Packet newPacket = obj->setPosition(x,y);
        if ( obj->getSynced() )
            SyncManager::sendTCPMessage( newPacket );
    }


    return 0;
}

// GetPosition( object )
int l_GetPosition(lua_State* L) {
    sf::Uint16 objectID;

    if ( luaL_checkinteger(L,-1) ){
        objectID = lua_tonumber(L,-1);
    }

    Object * obj = StageManager::gameState->getObject(objectID);
    //std::cout << "getObject : " << obj << std::endl;
    if ( obj != 0 ){
        float x, y;
        obj->getPosition(x,y);
        lua_pushnumber(L,x);
        lua_pushnumber(L,y);
        return 2;
    }

    return 0;
}

// SetVelocity( object, x, y )
int l_SetVelocity(lua_State* L) {

    sf::Uint16 objectID;
    float x,y;

    if ( luaL_checkinteger(L,-3) ){
        objectID = lua_tonumber(L,-3);
    }

    x = lua_tonumber(L,-2);
    y = lua_tonumber(L,-1);

    Object * obj = StageManager::gameState->getObject(objectID);
    if ( obj != 0 ){
        sf::Packet newPacket = obj->setVelocity(x,y);
        if ( obj->getSynced() )
            SyncManager::sendTCPMessage( newPacket );
    }


    return 0;
}

// SetRotation( object, rot )
int l_SetRotation(lua_State* L) {

    sf::Uint16 objectID;
    float rot;

    if ( luaL_checkinteger(L,-2) ){
        objectID = lua_tonumber(L,-2);
    }

    rot = lua_tonumber(L,-1);

    Object * obj = StageManager::gameState->getObject(objectID);
    if ( obj != 0 ){
        sf::Packet newPacket = obj->setRotation(rot);
        if ( obj->getSynced() )
            SyncManager::sendTCPMessage( newPacket );
    }


    return 0;
}

// SetFriction( object, friction )
int l_SetFriction(lua_State* L) {

    sf::Uint16 objectID;
    float friction;

    if ( luaL_checkinteger(L,-2) ){
        objectID = lua_tonumber(L,-2);
    }

    friction = lua_tonumber(L,-1);

    Object * obj = StageManager::gameState->getObject(objectID);
    if ( obj != 0 ){
        sf::Packet newPacket = obj->setFriction(friction);
        if ( obj->getSynced() )
            SyncManager::sendTCPMessage( newPacket );
    }

    return 0;
}

/*
// SetTexture( object, textureID )
int l_SetTexture(lua_State* L) {

    sf::Uint16 objectID;
    sf::Uint16 textureID;

    if ( luaL_checkinteger(L,-2) ){
        objectID = lua_tonumber(L,-2);
    }

    textureID = lua_tonumber(L,-1);

    Object * obj = StageManager::gameState->getObject(objectID);
    if ( obj != 0 ){
        sf::Packet newPacket = obj->setTextureID(textureID);
        if ( obj->getSynced() )
            SyncManager::sendTCPMessage( newPacket );
    }

    return 0;
}*/

// SetTexture( object, textureName )
int l_SetTexture(lua_State* L) {

    sf::Uint16 objectID;
    char textureName[256];

    if ( luaL_checkinteger(L,1) ){
        objectID = lua_tonumber(L,1);
    }

    strcpy(textureName,lua_tostring(L,2));

    Object * obj = StageManager::gameState->getObject(objectID);
    if ( obj != 0 ){
        obj->setTexture(textureName);
    }

    return 0;
}

// KillObject( object )
int l_KillObject(lua_State* L) {

    sf::Uint16 objectID;

    if ( luaL_checkinteger(L,-1) ){
        objectID = lua_tonumber(L,-1);
    }

    Object * obj = StageManager::gameState->getObject(objectID);
    if ( obj != 0 ){
        sf::Packet newPacket = obj->kill();
        if ( obj->getSynced() )
            SyncManager::sendTCPMessage( newPacket );
    }

    return 0;
}

// RegisterAsServerObject( object )
int l_RegisterAsServerObject(lua_State* L) {
    std::cout << "Register as server object" << std::endl;
    sf::Uint16 objectID;

    if ( luaL_checkinteger(L,-1) ){
        objectID = lua_tonumber(L,-1);
    }

    std::cout << objectID << std::endl;

    Object * obj = StageManager::gameState->getObject(objectID);
    //std::cout << obj << std::endl;
    if ( obj != 0 ){
        //std::cout << "REGISTERING" << std::endl;
        SyncManager::registerObjectToServer( obj );
    }

    return 0;
}

// CreateObject()
int l_CreateObject(lua_State* L){
    Object * newObject = new Object();
    sf::Uint16 id = StageManager::gameState->registerClientObject( newObject );
    lua_pushnumber( L, id );

    return 1;
}

