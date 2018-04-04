#include "Rubie.h"

#include "GameRegistry.h"
#include "Display.h"

#include "Mechanics.h"
#include "StageManager.h"
#include "SyncManager.h"

#define _USE_MATH_DEFINES
#include "math.h"

#include <stdlib.h>
#include <string.h>
#include <iostream>

#define MAX_LASERS 8

const float Rubie::acceleration = 0.3f;
const float Rubie::friction = 0.1f;
const int Rubie::abilityCooldown = 60;

Rubie::Rubie(){
    sf::Texture * texture = GameRegistry::getResource("rubie.png",ResourceType::Texture).texture;
    sprite.setTexture( *texture, false );
    sprite.setScale(2.f,2.f);
    sprite.setOrigin(8.f,8.f);
    sprite.setTextureRect(sf::IntRect(sf::Vector2i(0,0),sf::Vector2i(16,16)));

    texture = GameRegistry::getResource("shard_rubie.png",ResourceType::Texture).texture;
    shootParticles = new Particle[16];
    for ( int i = 0; i < 16; i++ ) shootParticles[i].setTexture(texture);

    velocity.x = velocity.y = 0;
    w = a = s = d = false;
    cooldown = 0;
    animationTimer = 0;
    syncable = false;
    dead = false;

    syncer = -1;

    lasers = new RubieLaser[MAX_LASERS];

    healthBar.setCrystal(this);
    hp = 100;
}

void Rubie::draw()
{
    for ( int i = 0; i < 16; i++ )
        if ( !shootParticles[i].isDead() )
            shootParticles[i].draw();
    for ( int i = 0; i < MAX_LASERS; i++ ){
        if ( !lasers[i].isDead() ){
            lasers[i].draw();
        }
    }
    Display::window->draw(sprite);
    healthBar.draw();
}

sf::Vector2f Rubie::getPosition()
{
    return sprite.getPosition();
}

void Rubie::update(float dt){
    for ( int i = 0; i < 16; i++ )
        shootParticles[i].update(dt);

    for ( int i = 0; i < MAX_LASERS; i++ ){
        if ( !lasers[i].isDead() ){
            lasers[i].update( dt );
        }
    }
    if ( Mechanics::getSpeed(velocity) < 5.0f ){
        Mechanics::applyAcceleration(velocity,d-a,s-w,acceleration);
        Mechanics::applyMaxSpeed(velocity,5.0f);
    }
    Mechanics::applyFriction(velocity,friction);

    animationTimer++;
    if ( animationTimer >= 25 )
        animationTimer = 0;

    sprite.setTextureRect(sf::IntRect(sf::Vector2i(16*(animationTimer/5),0),sf::Vector2i(16,16)));

    sprite.move(velocity.x,velocity.y);

    if (cooldown) cooldown--;

    //if ( syncable )
        //SyncManager::sendPosition(sprite.getPosition());

    for ( int laser = 0; laser < MAX_LASERS; laser++ ){
        if ( !lasers[laser].isDead() ){
            for ( int player = 0; player < MAX_PLAYERS; player++ ){
                if ( SyncManager::players[player] && player != getSyncer() ){
                    if ( lasers[laser].checkCollision( SyncManager::crystals[player] )){
                        std::cout << "Collision on player no." << player << " laser no. " << laser << std::endl;
                        lasers[laser].dead = true;
                        if ( syncable )
                            SyncManager::sendLaserCollision(player);
                    }
                }
            }
        }
    }

    for ( int particle = 0; particle < 16; particle++ ){
        if ( !shootParticles[particle].isDead() ){
            for ( int player = 0; player < MAX_PLAYERS; player++ ){
                if ( SyncManager::players[player] && player != getSyncer() ){
                    if ( shootParticles[particle].checkCollision( SyncManager::crystals[player] )){
                        std::cout << "Basic attack on player no." << player << std::endl;
                        shootParticles[particle].lifetime = 0;
                        if ( syncable )
                            SyncManager::sendParticleCollision(player);
                    }
                }
            }
        }
    }

    healthBar.update(dt);
}

void Rubie::jetAway(){
    if ( Mechanics::getSpeed(velocity) < 1.f )
        return;
    if ( !cooldown )
        cooldown = abilityCooldown;
    else
        return;

    float angle = atan2( velocity.y, velocity.x );
    SyncManager::sendUltimate(angle);

    velocity.x = cos(angle)*30.f;
    velocity.y = sin(angle)*30.f;

    int count = 0;
    for ( int i = 0; i < MAX_PLAYERS; i++ ){
        if ( lasers[i].isDead() ){
            float x = cos ( count*M_PI/14.f - M_PI*5.f/4.f + angle );
            float y = sin ( count*M_PI/14.f - M_PI*5.f/4.f + angle );
            lasers[i].reset(sprite.getPosition(),sf::Vector2f(x,y)*30.f);
            count++;
            if ( count == 8 )
                break;
        }
    }
}

void Rubie::shoot(){
    if ( Mechanics::getSpeed(velocity) < 2.f )
        return;
    for ( int i = 0; i < 16; i++ ){
        if ( shootParticles[i].isDead() ){
            shootParticles[i].reset(sprite.getPosition(),velocity*15.f,30);
            SyncManager::sendShoot(velocity);
            break;
        }
    }
}

void Rubie::input( const sf::Event & event )
{
    if ( event.type == sf::Event::LostFocus ){
        w = a = s = d = false;
    }else
    if ( event.type == sf::Event::KeyPressed ){
        switch (event.key.code){
            case sf::Keyboard::A :
                a = true;
                break;
            case sf::Keyboard::D :
                d = true;
                break;
            case sf::Keyboard::W :
                w = true;
                break;
            case sf::Keyboard::S :
                s = true;
                break;
            case sf::Keyboard::G :
                jetAway();
                break;
            case sf::Keyboard::F :
                shoot();
                break;
            default:
                break;
        }
    }
    else if( event.type == sf::Event::KeyReleased ){
        switch (event.key.code){
            case sf::Keyboard::A :
                a = false;
                break;
            case sf::Keyboard::D :
                d = false;
                break;
            case sf::Keyboard::W :
                w = false;
                break;
            case sf::Keyboard::S :
                s = false;
                break;
            default:
                break;
        }
    }
}

void Rubie::setPosition(const sf::Vector2f& position){
    sprite.setPosition(position);
}

bool Rubie::isDead()
{
    return dead;
}


void Rubie::move()
{

}

void Rubie::attack(){
    sf::Vector2f _velocity;
    memcpy(&_velocity.x,SyncManager::options,4);
    memcpy(&_velocity.y,SyncManager::options+4,4);
    for ( int i = 0; i < 16; i++ ){
        if ( shootParticles[i].isDead() ){
            shootParticles[i].reset(sprite.getPosition(),_velocity*15.f,30);
            break;
        }
    }
}

void Rubie::ultimate(){
    float angle = ((float*)SyncManager::options)[0];

    velocity.x = cos(angle)*30.f;
    velocity.y = sin(angle)*30.f;

    float count = 0;
    for ( int i = 0; i < MAX_LASERS; i++ ){
        if ( lasers[i].isDead() ){
            float x = cos ( count*M_PI/14.f - M_PI*5.f/4.f + angle );
            float y = sin ( count*M_PI/14.f - M_PI*5.f/4.f + angle );
            lasers[i].reset(sprite.getPosition(),sf::Vector2f(x,y)*30.f);
            count++;
            if ( count == 8 )
                break;
        }
    }
}

void Rubie::setSyncable( bool _syncable ){
    syncable = _syncable;
}

float Rubie::getRadius(){
    return 16.f;
}

bool Rubie::isCollidable(){
    return true;
}

void Rubie::markDead(){
    dead = true;
}

void Rubie::setSyncer( int _syncer ){
    syncer = _syncer;
}
int Rubie::getSyncer(){
    return syncer;
}

void Rubie::pushCrystal(const sf::Vector2f& _velocity){
    velocity = _velocity;
}

int Rubie::getHP()
{
    return hp;
}

void Rubie::setHP( int newHP )
{
    hp = newHP;
}
