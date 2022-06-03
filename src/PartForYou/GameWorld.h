#ifndef GAMEWORLD_H__
#define GAMEWORLD_H__

#ifndef GAMEOBJECTS_H__
#include "GameObjects.h"
#endif

#include <list>

#include "WorldBase.h"

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
