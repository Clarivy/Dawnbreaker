#ifndef GAMEOBJECTS_H__
#define GAMEOBJECTS_H__

#ifndef GAMEWORLD_H__
#include "GameWorld.h"
#endif

#include "ObjectBase.h"

enum class StrategyDirection {
    STRAIGHT,
    LEFTDOWN,
    RIGHTDOWN
};

class GameObject : public ObjectBase {

public:

    GameObject(int imageID, int x, int y, int direction, int layer, double size, int health);

    virtual ~GameObject();

    bool IsDestroyed() const;

    int GetHealthPoints() const;

    virtual int IsEnemy() const;

    virtual bool IsAlliance() const;

    virtual int IsGoodie() const;

    void SetHealthPoints(int new_health_points);

    void SetDestroyed();

protected:

    virtual int CheckCollision() const;


private:
    
    int health_points;

};


class Star : public GameObject {

public:

    Star(int x, int y, double size);

    virtual void Update() override;

};

class Explosion : public GameObject {

    Explosion(int x, int y);

    virtual void Update() override;

};

class PhysicalObject : public GameObject {

public:

    PhysicalObject(int imageID, int x, int y, int direction, int layer, double size, int health, GameWorld *_game_world, int _damage);

    virtual bool CollisionCheck() = 0;

    int GetDamage() const;

protected:

    GameWorld* game_world;
    int damage;

};

class Alliance: public PhysicalObject {

public:

    using PhysicalObject::PhysicalObject;

    virtual bool IsAlliance() const override;

};

class Goodie: public PhysicalObject {

public:

    Goodie(int imageID, int x, int y, GameWorld *game_world);

    virtual void Update() override;

    int GetReward() const;

};

class Enemy: public PhysicalObject {

public:

    using PhysicalObject::PhysicalObject;

    virtual int IsEnemy() const override;

};

class Dawnbreaker: public Alliance {

public:

    Dawnbreaker(GameWorld *game_world);

    virtual void Update() override;

    virtual bool CollisionCheck() override;

    int GetLives() const;

    int GetMeteorsNumber() const;

    int GetLevel() const;

    int GetDestroyedEnemy() const;

    void IncreaseEnergy();

    void IncreaseMeteorsNumber();

    void IncreaseLevel();

    void DecreaseLives();

    void IncreaseDestroyedEnemy();

private:

    int lives;
    int meteors_number;
    int level;
    int energy;
    int destroyed_enemy;
};

class BlueBullet: public Alliance {
    
public:

    BlueBullet(int x, int y, double size, GameWorld *game_world, int damage);

    virtual bool CollisionCheck() override;

    virtual void Update() override;

};

class Meteor: public Alliance {

public:

    Meteor(int x, int y, GameWorld *game_world);

    virtual void Update() override;

    virtual bool CollisionCheck() override;

};

class RedBullet: public Enemy {

public:

    RedBullet(int x, int y, int direction, GameWorld *game_world, int damage);

    virtual bool CollisionCheck() override;

    virtual void Update() override;

};

class SpaceShip: public Enemy {

public:

    SpaceShip(int imageID, int x, int y, int health_points, GameWorld *game_world, int damage, int _energy, int _speed, int _energy_limit);

protected:

    void Move();

    virtual void TryAttack() = 0;

    virtual int IsEnemy() const override;

    void EngeryRegeneration();

    virtual bool CollisionCheck() override;

    virtual void Update() override;

    int GetEnergy() const;

    StrategyDirection GetStrategy() const;

    int GetStrategyLength() const;

    int GetSpeed() const;

    void SetEnergy(int x);

    void SetStrategy(int x);
 
    void SetStrategy(StrategyDirection x);
 
    void SetStrategyLength(int x);
 
    void SetSpeed(int x);

private:

    int energy, energy_limit;
    StrategyDirection strategy;
    int strategy_length;
    int speed;

};

class HPRestoreGoodie: public Goodie {

public:

    HPRestoreGoodie(int x, int y, GameWorld *game_world);

    virtual bool CollisionCheck() override;

    virtual int IsGoodie() const override;

};

class PowerUpGoodie: public Goodie {

public:

    PowerUpGoodie(int x, int y, GameWorld *game_world);

    virtual bool CollisionCheck() override;

    virtual int IsGoodie() const override;

};

class MeteorGoodie: public Goodie {

public:

    MeteorGoodie(int x, int y, GameWorld *game_world);

    virtual bool CollisionCheck() override;

    virtual int IsGoodie() const override;

};

class Alphatron: public SpaceShip {

public:

    Alphatron(int x, int y, int health_points, GameWorld *game_world, int damage, int _speed);

    virtual void TryAttack() override;

};

class Sigmatron: public SpaceShip {

public:

    Sigmatron(int x, int y, int health_points, GameWorld *game_world, int _speed);

    virtual void TryAttack() override;
};


class Omegatron: public SpaceShip {

public:

    Omegatron(int x, int y, int health_points, GameWorld *game_world, int damage, int _speed);

    virtual void TryAttack() override;
};

#endif // GAMEOBJECTS_H__