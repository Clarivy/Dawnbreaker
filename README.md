## Design Instruction

### GameWorld: WorldBase

* Init -> void
  * New Dawnbreaker
  * 30 star

* Update -> LevelStatus{ONGOING, DAWNBREAKER_DESTROYED, LEVEL_CLEARED}
  * New star
  * New Enemy?
  * New enemy
  * Iterate all GameObject and call their Update()
  * Check if DB is destroyed 
  * Check if win
  * Check to delete
  * Status board
    * HP, Meteors, Lives, Level, enemies, score  of DB, 3 whitespace deliminated  

* CleanUp -> void
  * Delete all element

* IsGameOver -> bool
  * If Lost al lives

* CreateBlueBullet()
* CreateRedBullet()
* CreateExplosion()



### GameObject: ObjectBase

#### Default members

* Constructor
  * ObjectBase(imageID, x, y, direction, layer,size)
* Basic accessor and mutator
  * GetX(), GetY()->int, MoveTo(x, y), GetDirection()->int， GetLayer(), GetSize(). SetSize()

* Update(**pure-virtual**) -> void

#### Customized members

* IsDestroyed(): bool
  * Check heath_point == 0?
* IsEnemy(**virtual**)->int
  * default return false
* IsAlliance(**virtual**)->int
  * default return false
* IsGoodie(**virtual**)->int
  * default return false
* CheckCollision(**virtual**)->void
  * default nothing
* health_point: int
  * determine whether the object should be destroyed



### NonPhysicalObject: GameObject



### Star: NonPhysicalObject



### Explosion: NonPhysicalObject



### PhysicalObject: GameObject

* game_world: GameWorld*



### Alliance: PhysicalObject

* override-CheckCollision()
* override-IsAlliance



### Goodie: PhysicalObject

* reward = 20



### Enemy: PhysicalObject

* override-CheckCollision
* damage
* override: IsEnemy



### Dawnbreaker: Alliance

* lives
* meteors
* level



### BlueBullet: Alliance



### Meteor: Alliance



### RedBullet: Enemy



### SpaceShip: Enemy

* energy: int



### PowerUpGoodie: Goodie

* override-IsGoodie()->int:
  * return 1



### MeteorGoodie: Goodie

* override-IsGoodie()->int:
  * return 2



### Alphatron: SpaceShip



### Sigmatron: SpaceShip



### Omegatron: SpaceShip

