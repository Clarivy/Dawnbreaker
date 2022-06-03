#ifndef GAMEOBJECTS_H__
#include "GameObjects.h"
#endif

#ifndef UTILS_H__
#include "utils.h"
#endif

#define Y_EXCEED_DESTROY { \
    if (GetY() < 0 || GetY() >= WINDOW_HEIGHT) {\
        return SetDestroyed();\
    }\
}

#define CHECK_IS_DESTROYED { \
    if (IsDestroyed()) {\
        return ;\
    }\
}

#define CRASH_SKIP { \
    if (CheckCollision()) {\
        return ;\
    }\
}

GameObject::GameObject(int imageID, int x, int y, int direction, int layer, double size, int health) : ObjectBase(imageID, x, y, direction, layer, size) {
    this->health_points = health;
}

bool GameObject::IsDestroyed() const {
    return health_points == 0;
}

bool GameObject::IsEnemy() const {
    return false;
}

bool GameObject::IsAlliance() const {
    return false;
}

int GameObject::IsGoodie() const {
    return false;
}

int GameObject::GetHealthPoints() const {
    return health_points;
}

void GameObject::SetHealthPoints(int new_health_points) {
    health_points = new_health_points;
}

void GameObject::SetDestroyed() {
    health_points = 0;
}

Star::Star(int x, int y, double size) : GameObject(IMGID_STAR, x, y, 0, 4, size, 1) {
}

void Star::Update() {
    CHECK_IS_DESTROYED;
    Y_EXCEED_DESTROY;
    MoveTo(GetX(), GetY() - 1);
}

Explosion::Explosion(int x, int y) : GameObject(IMGID_EXPLOSION, x, y, 0, 3, 4.5, 20) {
}

void Explosion::Update() {
    CHECK_IS_DESTROYED;
    SetHealthPoints(GetHealthPoints() - 1);
    SetSize(GetSize() - 0.2);
}

PhysicalObject::PhysicalObject(int imageID, int x, int y, int direction, int layer, double size, int health, GameWorld *_game_world, int _damage) : GameObject(imageID, x, y, direction, layer, size, health) {
    game_world = _game_world;
    damage = _damage;
}

int PhysicalObject::GetDamage() const {
    return damage;
}

bool Alliance::IsAlliance() const {
    return true;
}

Goodie::Goodie(int imageID, int x, int y, GameWorld *game_world) : PhysicalObject(imageID, x, y, 0, 2, 0.5, 1, game_world, 0) {
}

void Goodie::Update() {
    CHECK_IS_DESTROYED;
    Y_EXCEED_DESTROY;

    CollisionCheck();

    MoveTo(GetX(), GetY() - 2);

    CollisionCheck();
}


int Goodie::GetReward() const {
    return 20;
}

bool Enemy::IsEnemy() const {
    return true;
}

Dawnbreaker::Dawnbreaker(GameWorld * game_world) : Alliance(IMGID_DAWNBREAKER, 300, 300, 0, 0, 1.0, 100, game_world, 0) {
    lives = 3;
    meteors_number = 0;
    level = 0;
    energy = 10;
}

int Dawnbreaker::GetLives() const {
    return lives;
}

int Dawnbreaker::GetMeteorsNumber() const {
    return meteors_number;
}

int Dawnbreaker::GetLevel() const {
    return level;
}

int Dawnbreaker::GetDestroyedEnemy() const {
    return destroyed_enemy;
}

void Dawnbreaker::IncreaseEnergy() {
    if (energy < 10) {
        energy++;
    }
}

void Dawnbreaker::IncreaseMeteorsNumber() {
    ++meteors_number;
}

void Dawnbreaker::IncreaseLevel() {
    ++level;
}

void Dawnbreaker::DecreaseLives() {
    --lives;
}

void Dawnbreaker::IncreaseDestroyedEnemy() {
    ++destroyed_enemy;
}

BlueBullet::BlueBullet(int x, int y, double size, GameWorld * game_world, int damage) : Alliance(IMGID_BLUE_BULLET, x, y, 0, 1, size, 1, game_world, damage) {
}

void BlueBullet::Update() {
    CHECK_IS_DESTROYED;
    Y_EXCEED_DESTROY;

    CHECK_CRASH;
    
    MoveTo(GetX(), GetY() + 6);
}

Meteor::Meteor(int x, int y, GameWorld *game_world) : Alliance(IMGID_METEOR, x, y, 0, 1, 2.0, 1, game_world, 0) {
}

void Meteor::Update() {
    CHECK_IS_DESTROYED;
    Y_EXCEED_DESTROY;

    CHECK_CRASH;

    MoveTo(GetX(), GetY() + 2);
    SetDirection(GetDirection() + 5);
}

RedBullet::RedBullet(int x, int y, int direction, GameWorld *game_world, int damage) : Enemy(IMGID_RED_BULLET, x, y, direction, 1, 0.5, 1, game_world, damage) {
}

void RedBullet::Update() {
    CHECK_IS_DESTROYED;
    Y_EXCEED_DESTROY;

    CHECK_CRASH;

    if (GetDirection() == 180) {
        MoveTo(GetX(), GetY() - 6);
    } else if (GetDirection() == 162) {
        MoveTo(GetX() + 2, GetY() - 6);
    } else if (GetDirection() == 198) {
        MoveTo(GetX() - 2, GetY() - 6);
    }

    CHECK_CRASH;
}

SpaceShip::SpaceShip(int imageID, int x, int y, int health_points, GameWorld *game_world, int damage, int _energy, int _speed, int _energy_limit) : Enemy (imageID, x, y, 180, 0, 1.0, health_points, game_world, damage) {
    energy = _energy;
    energy_limit = _energy_limit;
    speed = _speed;
}

void SpaceShip::EngeryRegeneration() {
    if (GetEnergy() < energy_limit) {
        SetEnergy(GetEnergy() + 1);
    }
}

bool SpaceShip::CollisionCheck() {
    CHECK_CRASH;
}

void SpaceShip::Move() {
    if (GetStrategyLength() == 0) {
        SetStrategy(randInt(1, 3));
        SetStrategyLength(randInt(10, 50));
    }
    if (GetX() < 0) {
        SetStrategy(StrategyDirection::RIGHTDOWN);
        SetStrategyLength(randInt(10, 50));
    }
    if (GetX() >= WINDOW_WIDTH) {
        SetStrategy(StrategyDirection::RIGHTDOWN);
        SetStrategyLength(randInt(10, 50));
    }
    SetStrategyLength(GetStrategyLength() - 1);
    if (GetStrategy() == StrategyDirection::LEFTDOWN) {
        MoveTo(GetX() - speed, GetY() - speed);
    } else if (GetStrategy() == StrategyDirection::RIGHTDOWN) {
        MoveTo(GetX() + speed, GetY() - speed);
    } else if (GetStrategy() == StrategyDirection::STRAIGHT) {
        MoveTo(GetX(), GetY() - speed);
    }
}

void SpaceShip::Update() {
    CHECK_IS_DESTROYED;
    Y_EXCEED_DESTROY;
    CRASH_SKIP;
    TryAttack();
    EngeryRegeneration();
    Move();
    CRASH_SKIP;
}

int SpaceShip::GetEnergy() const {
    return energy;
}

StrategyDirection SpaceShip::GetStrategy() const {
    return strategy;
}

int SpaceShip::GetStrategyLength() const {
    return strategy_length;
}

int SpaceShip::GetSpeed() const {
    return speed;
}

void SpaceShip::SetEnergy(int x) {
    energy = x;
}

void SpaceShip::SetStrategy(int x) {
    if (x == 1) {
        SetStrategy(StrategyDirection::STRAIGHT);
    }
    else if (x == 2) {
        SetStrategy(StrategyDirection::LEFTDOWN);
    }
    else if (x == 3) {
        SetStrategy(StrategyDirection::RIGHTDOWN);
    }
}

void SpaceShip::SetStrategy(StrategyDirection x) {
    strategy = x;
}

void SpaceShip::SetStrategyLength(int x) {
    strategy_length = x;
}

void SpaceShip::SetSpeed(int x) {
    speed = x;
}

HPRestoreGoodie::HPRestoreGoodie(int x, int y, GameWorld *game_world) : Goodie(IMGID_HP_RESTORE_GOODIE, x, y, game_world) {
}

int HPRestoreGoodie::IsGoodie() const {
    return 1;
}

PowerUpGoodie::PowerUpGoodie(int x, int y, GameWorld *game_world) : Goodie(IMGID_POWERUP_GOODIE, x, y, game_world) {
}

int PowerUpGoodie::IsGoodie() const {
    return 2;
}

MeteorGoodie::MeteorGoodie(int x, int y, GameWorld *game_world) : Goodie(IMGID_METEOR_GOODIE, x, y, game_world) {
}

int MeteorGoodie::IsGoodie() const {
    return 3;
}


Alphatron::Alphatron(int x, int y, int health_points, GameWorld *game_world, int damage, int _speed) : SpaceShip(IMGID_ALPHATRON, x, y, health_points, game_world, damage, 25, _speed, 25) {
}

void Alphatron::TryAttack() {
    TRY_ATTACK;
}

Sigmatron::Sigmatron(int x, int y, int health_points, GameWorld *game_world, int _speed) : SpaceShip(IMGID_SIGMATRON, x, y, health_points, game_world, 0, 0, _speed, 0) {
}

void Sigmatron::TryAttack() {
    TRY_ATTACK;
}

Sigmatron::Sigmatron(int x, int y, int health_points, GameWorld *game_world, int _speed) : SpaceShip(IMGID_OMEGATRON, x, y, health_points, game_world, damage, 50, _speed, 50) {
}

void Sigmatron::TryAttack() {
    TRY_ATTACK;
}