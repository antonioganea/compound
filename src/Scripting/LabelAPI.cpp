// CreateLabel( x, y, string )
int l_CreateLabel(lua_State* L){

    float x,y;
    char mystr[256];

    x = lua_tonumber(L,1);
    y = lua_tonumber(L,2);
    strcpy(mystr,lua_tostring(L,3));

    sf::Uint16 id = StageManager::gameState->addLabel( x,y, mystr );
    lua_pushnumber( L, id );

    return 1;
}


// x, y, w, h = GetBounds( labelID )
int l_GetBounds(lua_State* L){
    sf::Uint16 id;
    id = lua_tonumber(L,1);

    Label * label = StageManager::gameState->m_labels[id];

    sf::FloatRect bounds = label->text.getGlobalBounds();

    lua_pushnumber(L, bounds.left );
    lua_pushnumber(L, bounds.top );
    lua_pushnumber(L, bounds.width );
    lua_pushnumber(L, bounds.height );

    return 4;
}

// SetLabelPosition( labelID, x, y )
int l_SetLabelPosition(lua_State* L){
    sf::Uint16 id;
    id = lua_tonumber(L,1);
    float x,y;
    x = lua_tonumber(L,2);
    y = lua_tonumber(L,3);

    Label * label = StageManager::gameState->m_labels[id];

    label->setPosition(x,y);

    return 1;
}

// SetText( labelID, text )
int l_SetText(lua_State* L){
    sf::Uint16 id;
    id = lua_tonumber(L,1);
    char buffer[256];
    strcpy(buffer,lua_tostring(L,2));

    Label * label = StageManager::gameState->m_labels[id];
    label->setText(buffer);

    return 1;
}

// DestroyLabel( id )
int l_DestroyLabel(lua_State* L) {
    sf::Uint16 id;
    id = lua_tonumber(L,1);

    Label * label = StageManager::gameState->m_labels[id];
    label->destroy();

    return 0;
}

// SetFontSize( id, size )
int l_SetFontSize(lua_State* L) {
    sf::Uint16 id;
    id = lua_tonumber(L,1);

    int size;
    size = lua_tonumber(L,2);

    Label * label = StageManager::gameState->m_labels[id];
    label->setSize(size);

    return 0;
}

// SetColor( id, colorID )
int l_SetColor(lua_State* L) {
    sf::Uint16 id;
    id = lua_tonumber(L,1);

    int colorID;
    colorID = lua_tonumber(L,2);

    Label * label = StageManager::gameState->m_labels[id];
    label->setColor(colorID);

    return 0;
}
