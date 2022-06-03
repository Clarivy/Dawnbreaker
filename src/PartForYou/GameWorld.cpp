#include "GameWorld.h"

GameWorld::GameWorld() {

}

GameWorld::~GameWorld() {

}

void GameWorld::Init() { 

}

LevelStatus GameWorld::Update() {
  return LevelStatus::DAWNBREAKER_DESTROYED;
}

void GameWorld::CleanUp() {

}


bool GameWorld::IsGameOver() const {
  return false;
}
