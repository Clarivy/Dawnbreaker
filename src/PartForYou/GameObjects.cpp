#include "GameObjects.h"
#include "utils.h"

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

double GameObject::operator ^ (const GameObject &other) const {
    double distance_2 = (GetX() - other.GetX()) * (GetX() - other.GetX()) + (GetY() - other.GetY()) * (GetY() - other.GetY());
    double size_sum = GetSize() + other.GetSize();
    double size_2 = size_sum * size_sum;
    return size_2 * 900 - distance_2;
}

bool GameObject::IsDestroyed() const {
    return health_points == 0;
}

int GameObject::IsEnemy() const {
    return false;
}

int GameObject::IsAlliance() const {
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

Star::Star(int x, int y, double size) : GameObject(IMGID_STAR, 0, 0, 0, 4, 0.1, 1) {
    if (x == -1) {
        x = randInt(0, WINDOW_WIDTH - 1);
    }
    if (y == -1) {
        y = randInt(0, WINDOW_HEIGHT - 1);
    }
    if (size < 0) {
        size = randInt(10, 40) / 100.0;
    }
    MoveTo(x, y);
    SetSize(size);
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

Goodie::Goodie(int imageID, int x, int y, GameWorld *game_world) : PhysicalObject(imageID, x, y, 0, 2, 0.5, 1, game_world, 0) {
}

void Goodie::Update() {
    CHECK_IS_DESTROYED;
    Y_EXCEED_DESTROY;

    CRASH_SKIP;

    MoveTo(GetX(), GetY() - 2);

    CRASH_SKIP;
}

bool Goodie::CollisionCheck() {
    Dawnbreaker *player = game_world->player;

    double distance = (*this) ^ (*player);
    if (distance <= 0) return false;

    CollisionEvent();

    game_world->IncreaseScore(20);
    SetDestroyed();
    return true;
}

Dawnbreaker::Dawnbreaker(GameWorld * game_world) : PhysicalObject(IMGID_DAWNBREAKER, 300, 100, 0, 0, 1.0, 100, game_world, 0) {
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

bool Alliance::CollisionCheck() {
    for (auto &object : game_world->game_objects) {
        if (object->IsEnemy() == 2) {
            double distance = (*this) ^ (*object);
            if(distance > 0) {
                CollisionEvent(object);
                return true;
            }
        }
    }
    return false;
}

BlueBullet::BlueBullet(int x, int y, double size, GameWorld * game_world, int damage) : Alliance(IMGID_BLUE_BULLET, x, y, 0, 1, size, 1, game_world, damage) {
}

void BlueBullet::CollisionEvent(GameObject *object) {
    object->SetHealthPoints(object->GetHealthPoints() - GetDamage());

    if (object->IsDestroyed()) {
        object->DeathEvent();
    }

    SetDestroyed();
}

void BlueBullet::Update() {
    CHECK_IS_DESTROYED;
    Y_EXCEED_DESTROY;

    CollisionCheck();
    
    MoveTo(GetX(), GetY() + 6);
}

Meteor::Meteor(int x, int y, GameWorld *game_world) : Alliance(IMGID_METEOR, x, y, 0, 1, 2.0, 1, game_world, 0) {
}

void Meteor::Update() {
    CHECK_IS_DESTROYED;
    Y_EXCEED_DESTROY;

    CheckCollision();

    MoveTo(GetX(), GetY() + 2);
    SetDirection(GetDirection() + 5);

    CheckCollision();
}

void Meteor::CollisionEvent(GameObject *object) {
    object->SetDestroyed();
    object->DeathEvent();
}

RedBullet::RedBullet(int x, int y, int direction, GameWorld *game_world, int damage) : Enemy(IMGID_RED_BULLET, x, y, direction, 1, 0.5, 1, game_world, damage) {
}

void RedBullet::Update() {
    CHECK_IS_DESTROYED;
    Y_EXCEED_DESTROY;
    CRASH_SKIP;

    if (GetDirection() == 180) {
        MoveTo(GetX(), GetY() - 6);
    } else if (GetDirection() == 162) {
        MoveTo(GetX() + 2, GetY() - 6);
    } else if (GetDirection() == 198) {
        MoveTo(GetX() - 2, GetY() - 6);
    }

    CRASH_SKIP;
}

bool RedBullet::CollisionCheck() {
    Dawnbreaker *player = game_world->player;
    double distance = (*this) ^ (*player);
    if(distance > 0) {
        player->SetHealthPoints(player->GetHealthPoints() - GetDamage());
        this->SetDestroyed();
        return true;
    }
    return false;
}

SpaceShip::SpaceShip(int imageID, int x, int y, int health_points, GameWorld *game_world, int damage, int _energy, int _speed, int _energy_limit) : Enemy (imageID, x, y, 180, 0, 1.0, health_points, game_world, damage) {
    if (x == -1) {
        x = randInt(0, WINDOW_WIDTH - 1);
    }
    if (y == -1) {
        y = randInt(0, WINDOW_HEIGHT - 1);
    }
    MoveTo(x, y);

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
    Dawnbreaker *player = game_world->player;
    double distance = (*this) ^ (*player);
    if(distance > 0) {
        player->SetHealthPoints(player->GetHealthPoints() - 20);
        this->SetDestroyed();
        return true;
    }
    for (auto &object : game_world->game_objects) {
        int type = object->IsAlliance();
        if (type == 1) {
            double distance = (*this) ^ (*object);
            if(distance > 0) {
                SetHealthPoints(GetHealthPoints() - object->GetDamage());
                object->SetDestroyed();
                break;
            }
        }
        else if (type == 2) {
            double distance = (*this) ^ (*object);
            if(distance > 0) {
                SetDestroyed();
                break;
            }
        }
    }
    if (IsDestroyed()) {
        game_world->game_objects.push_back(new Explosion(GetX(), GetY()));
        DeathEvent();
        player->IncreaseDestroyedEnemy();
        return true;
    }
    return false;
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

void HPRestoreGoodie::CollisionEvent() {
    Dawnbreaker *player = game_world->player;
    player->SetHealthPoints(player->GetHealthPoints() + 50);
    if (player->GetHealthPoints() > 100) {
        player->SetHealthPoints(100);
    }
}

int HPRestoreGoodie::IsGoodie() const {
    return 1;
}

PowerUpGoodie::PowerUpGoodie(int x, int y, GameWorld *game_world) : Goodie(IMGID_POWERUP_GOODIE, x, y, game_world) {
}

void PowerUpGoodie::CollisionEvent() {
    Dawnbreaker *player = game_world->player;
    player->IncreaseLevel();
}

int PowerUpGoodie::IsGoodie() const {
    return 2;
}

MeteorGoodie::MeteorGoodie(int x, int y, GameWorld *game_world) : Goodie(IMGID_METEOR_GOODIE, x, y, game_world) {
}

int MeteorGoodie::IsGoodie() const {
    return 3;
}

void MeteorGoodie::CollisionEvent() {
    Dawnbreaker *player = game_world->player;
    player->IncreaseMeteorsNumber();
}


Alphatron::Alphatron(int x, int y, int health_points, GameWorld *game_world, int damage, int _speed) : SpaceShip(IMGID_ALPHATRON, x, y, health_points, game_world, damage, 25, _speed, 25) {
}

void Alphatron::TryAttack() {
    Dawnbreaker *player = game_world->player;
    double distance_x = std::abs(GetX() - player->GetX());
    if (distance_x > 10) return ;
    if (GetEnergy() < 25) return ;
    if (randInt(1, 100) > 25) return ;
    SetEnergy(GetEnergy() - 25);
    game_world->game_objects.push_back(new RedBullet(GetX(), GetY() - 50, 180, game_world, GetDamage()));
}

void Alphatron::DeathEvent() {
    game_world->IncreaseScore(50);
}

Sigmatron::Sigmatron(int x, int y, int health_points, GameWorld *game_world, int _speed) : SpaceShip(IMGID_SIGMATRON, x, y, health_points, game_world, 0, 0, _speed, 0) {
}

void Sigmatron::TryAttack() {
    Dawnbreaker *player = game_world->player;
    double distance_x = std::abs(GetX() - player->GetX());
    if (distance_x > 10) return ;
    SetStrategy(StrategyDirection::STRAIGHT);
    SetStrategyLength(WINDOW_HEIGHT);
    SetSpeed(10);
}

void Sigmatron::DeathEvent() {
    game_world->IncreaseScore(100);
    if (randInt(1, 100) > 20) return ;
    game_world->game_objects.push_back(new HPRestoreGoodie(GetX(), GetY(), game_world));
}

Omegatron::Omegatron(int x, int y, int health_points, GameWorld *game_world, int damage, int _speed) : SpaceShip(IMGID_OMEGATRON, x, y, health_points, game_world, damage, 50, _speed, 50) {
}

void Omegatron::TryAttack() {
    Dawnbreaker *player = game_world->player;
    if (GetEnergy() < 50) return ;
    SetEnergy(GetEnergy() - 50);
    game_world->game_objects.push_back(new RedBullet(GetX(), GetY() - 50, 162, game_world, GetDamage()));
    game_world->game_objects.push_back(new RedBullet(GetX(), GetY() - 50, 198, game_world, GetDamage()));
}

void Omegatron::DeathEvent() {
    game_world->IncreaseScore(200);
    if (randInt(1, 100) > 40) return ;
    int R = randInt(1, 100);
    Goodie* new_goodie;
    if (R <= 40) {
        new_goodie = new HPRestoreGoodie(GetX(), GetY(), game_world);
    }
    else {
        new_goodie = new PowerUpGoodie(GetX(), GetY(), game_world);
    }
    game_world->game_objects.push_back(new_goodie);

}