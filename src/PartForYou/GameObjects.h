#ifndef GAMEOBJECTS_H__
#define GAMEOBJECTS_H__

#ifndef GAMEWORLD_H__
#include "GameWorld.h"
#endif

#include "ObjectBase.h"

class GameObject : public ObjectBase {

public:

    GameObject(int imageID, int x, int y, int direction, int layer, double size, int health);

    virtual ~GameObject();

protected:

    bool IsDestroyed() const;

    virtual bool IsEnemy() const;

    virtual bool IsAlliance() const;

    virtual int IsGoodie() const;

    virtual int CheckCollision() const;

    int health_points;

private:
    

};


class Star : public GameObject {

    Star(int x, int y, int size);

    virtual void Update() override;

};

class Explosion : public GameObject {

    Explosion(int x, int y);

    virtual void Update() override;

};

class PhysicalObject : public GameObject {

    PhysicalObject(int imageID, int x, int y, int direction, int layer, double size, int health, GameWorld *_game_world, int _damage);

protected:

    GameWorld* game_world;
    int dagame;

};

class Alliance: public PhysicalObject {

    Alliance(int imageID, int x, int y, int direction, int layer, double size, int health, GameWorld *_game_world, int _damage);

    virtual bool IsAlliance() const override;

};

class Goodie: public PhysicalObject {

    Goodie(int imageID, int x, int y, GameWorld *_game_world);

    virtual int IsGoodie() const;

    void GetReward() const;

};

class Enemy: public PhysicalObject {

    Enemy(int imageID, int x, int y, int direction, int layer, double size, int health, GameWorld *_game_world, int _damage);

    virtual bool IsEnemy() const override;

};

class Dawnbreaker: public Alliance {

public:

    Dawnbreaker();

    virtual void Update() override;

protected:

    int lives;
    int meteors_number;
    int level;

};

class BlueBullet: public Alliance {
    
public:

    BlueBullet(int x, int y, int size, GameWorld *_game_world, int _damage);

    virtual void Update() override;

};

class Meteor: public Alliance {

public:

    Meteor(int x, int y, GameWorld *_game_world, int _damage);

    virtual void Update() override;

};

class RedBullet: public Enemy {

public:

    RedBullet(int x, int y, int direction, GameWorld *_game_world, int _damage);

    virtual void Update() override;

};

class SpaceShip: public Enemy {

public:

    SpaceShip(int imageID, int x, int y, int health_points, GameWorld *_game_world, int _damage, int _energy, int _speed);

protected:

    int GetEnergy() const;

    int GetStategy() const;

    int GetStategyLength() const;

    int GetSpeed() const;

    int SetEnergy() const;
 
    int SetStategy() const;
 
    int SetStategyLength() const;
 
    int SetSpeed() const;

private:

    int energy;
    int strategy;
    int strategy_length;
    int speed;

};

class HPRestoreGoodie: public Goodie {

public:

    HPRestoreGoodie(int x, int y, GameWorld *_game_world);

    virtual void Update() override;

    virtual int IsGoodie() const override;

};

class PowerUpGoodie: public Goodie {

public:

    PowerUpGoodie(int x, int y, GameWorld *_game_world);

    virtual void Update() override;

    virtual int IsGoodie() const override;

};

class MeteorGoodie: public Goodie {

public:

    MeteorGoodie(int x, int y, GameWorld *_game_world);

    virtual void Update() override;

    virtual int IsGoodie() const override;

};

class Alphatron: public SpaceShip {

public:

    Alphatron(int x, int y, int health_points, GameWorld *_game_world, int _damage, int _speed);

    virtual void Update() override;

};

class Sigmatron: public SpaceShip {

public:

    Sigmatron(int x, int y, int health_points, GameWorld *_game_world, int _speed);

    virtual void Update() override;

};


class Omegatron: public SpaceShip {

public:

    Omegatron(int x, int y, int health_points, GameWorld *_game_world, int _damage, int _speed);

    virtual void Update() override;

};

#endif // GAMEOBJECTS_H__