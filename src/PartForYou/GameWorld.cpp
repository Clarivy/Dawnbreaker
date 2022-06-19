#include "GameWorld.h"
#include "GameObjects.h"
#include "utils.h"
#include <sstream>
// #define __CHEAT_MODE__

GameWorld::GameWorld() {
    current_enemy_num = 0;
    lives = 3;
}

GameWorld::~GameWorld() {
    CleanUp();
}

void GameWorld::Init() {
    current_enemy_num = 0;
    player = new Dawnbreaker(this);

    // Create 30 stars
    for(int i = 1; i <= 30; ++i) {
        game_objects.push_back(new Star(-1, -1, -1));
    }
}

#define PUSH_OBJECT(obj) { \
    game_objects.push_back(obj); \
    ++current_enemy_num; \
}

LevelStatus GameWorld::Update() {

    // New star
    if (randInt(1, 30) == 1) {
        game_objects.push_back(new Star(-1, WINDOW_HEIGHT - 1, -1));
    }

    // New Spaceship
#define level GetLevel()
    int required = 3 * level;
    int destroyed = player->GetDestroyedEnemy();
    int toDestroy = required - destroyed;
    int maxOnScreen = (5 + level) / 2;
    int allowed = std::min(maxOnScreen, toDestroy);
    int &onScreen = current_enemy_num;
    if (allowed > onScreen && randInt(1, 100) <= (allowed - onScreen)) {
        int P1 = 6;
        int P2 = 2 * std::max(level - 1, 0);
        int P3 = 3 * std::max(level - 2, 0);
        int sum = P1 + P2 + P3;
        int R = randInt(1, sum);
        if (R <= P1) {
            PUSH_OBJECT(new Alphatron(-1, WINDOW_HEIGHT - 1, 20 + level * 2, this, 4 + level, 2 + level / 5));
        }
        else if (R <= P1 + P2) {
            PUSH_OBJECT(new Sigmatron(-1, WINDOW_HEIGHT - 1, 25 + level * 5, this, 2 + level / 5));
        } else {
            PUSH_OBJECT(new Omegatron(-1, WINDOW_HEIGHT - 1, 20 + level, this, 2 + 2 * level, 3 + level / 4));
        }
    }

    destroyed = player->GetDestroyedEnemy();

#undef level

    // Update all objects
    player->Update();
    for (auto it = game_objects.begin(); it != game_objects.end(); ++it) {
        GameObject *obj = *it;
        obj->Update();
    }

    if (player->IsDestroyed()) {
#ifdef __CHEAT_MODE__
        player->SetHealthPoints(100);
#else
        DecreaseLives();
        return LevelStatus::DAWNBREAKER_DESTROYED;
#endif
    }

    if (destroyed == required) {
        player->IncreaseLevel();
        return LevelStatus::LEVEL_CLEARED;
    }

    // Remove destroyed objects
    for (auto it = game_objects.begin(); it != game_objects.end();) {
        GameObject *obj = *it;
        if (obj->IsDestroyed()) {
            delete obj;
            it = game_objects.erase(it);
        } else {
            ++it;
        }
    }
    // HP: X/100 Meteors: X Lives: X Level: X Enemies: X/X Score: X
    std::ostringstream os;
    os << "HP: " << player->GetHealthPoints() << "/100   Meteors: " << player->GetMeteorsNumber() << "   Lives: " << GetLives() << "   Level: " << GetLevel() << "   Enemies: " << player->GetDestroyedEnemy() << "/" << required << "   Score: " << GetScore();
    SetStatusBarMessage(os.str());
    return LevelStatus::ONGOING;
}

void GameWorld::CleanUp() {
    for (auto game_object : game_objects) {
        delete game_object;
    }
    delete player;
    game_objects.clear();
    current_enemy_num = 0;
}

bool GameWorld::IsGameOver() const {
    return GetLives() <= 0;
}

void GameWorld::DecreaseCurrentEnemyNum() {
    --current_enemy_num;
}

int GameWorld::GetLives() const {
    return lives;
}

void GameWorld::DecreaseLives() {
    --lives;
}