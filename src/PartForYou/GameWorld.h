#ifndef GAMEWORLD_H__
#define GAMEWORLD_H__

#include <list>
#include "WorldBase.h"

class GameObject;
class Spaceship;
class Dawnbreaker;
class Star;
class Explosion;
class Blue_Bullet;
class Enemy;
class Meteor;
class Red_Bullet;
class Alphatron;
class Sigmatron;
class Omegatron;
class Goodie;
class HP_Restore_Goodie;
class Power_Up_Goodie;
class Meteor_Goodie;

class GameWorld : public WorldBase {

public:

    GameWorld();

    virtual ~GameWorld();

    virtual void Init() override;

    virtual LevelStatus Update() override;

    virtual void CleanUp() override;

    virtual bool IsGameOver() const override;

    std::list<GameObject *> game_objects;

    Dawnbreaker *player;

private:

    int current_enemy_num;

};

#endif // !GAMEWORLD_H__
