#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const int HIT_BY_SHIP = 0;
const int HIT_BY_PROJECTILE = 1;

class StudentWorld;

///////////////////////////////////
//Actor's Declarations and Inlines
///////////////////////////////////
class Actor : public GraphObject
{
public:
    
    Actor(StudentWorld* world, double startX, double startY, int imageID, int dir, double size, int depth);
   
    //virtual destructor for the actor class, so all classes derived from actor can be destructed properly when a level is completed or when the game ends
    virtual ~Actor();
    
    //for actors that cannot suffer damage, the function just returns and do nothing
    virtual void sufferDamage(double amt, int cause);

    virtual void doSomething() = 0; //make Actor an ABC
    
    //check if the actor is an alien (smallgon, smoregon, snagglegon)
    virtual bool isAliens() const;
    
    //check if actor is still alive
    bool alive() const;
    
    //if the actor died, set itself dead
    void setDead();
    
    //returns a pointer to the world the actor lives in
    StudentWorld* getWorld();
    
    //checks if the actor went off the screen
    bool isInBound(double x, double y) const;
    
private:
    StudentWorld* wp;
    bool m_alive;
    
};

///////////////////////////////////////////
//Ship's Declaration and Inlines
///////////////////////////////////////////

class Ship : public Actor
{
public:
    
    Ship(StudentWorld* world, double startX, double startY, int imageID, double hitpoints, int dir, double size, int depth);
    virtual ~Ship();
    
    //returns the number of hitpoints
    double getHitPoints() const;
    
    //increase or decrease number of hitpoints by the "hitpoints" passed into the parameter
    void setHitPoints (double hitpoints);
    
private:
    double m_hit;
};

///////////////////////////////////////////
//Nachenblaster's Declaration and Inlines
///////////////////////////////////////////

class Nachenblaster : public Ship
{
public:
    
    Nachenblaster(StudentWorld* world);
    virtual ~Nachenblaster();
    virtual void doSomething();
    virtual void sufferDamage(double amt, int cause);
    
    //Accessors:
    //return the number of torpedoes player has
    int numTorpedoes() const;
    
    // Get the player's health percentage.
    int healthPct() const;
    
    // Get the player's cabbage energy percentage.
    int cabbagePct() const;

    //Mutators:
    //increases player's hitpoints by "num" in parameter
    void increaseHitPoints(double num);
    
    // Incease the number of torpedoes the player(Nachenblaster) has by amt.
    void increaseTorpedoes(int amt);
    
private:
    int m_cabEnergy;
    int m_torpedoes;
};


///////////////////////////////////////////
//Alien's Declaration and Inlines
///////////////////////////////////////////

class Aliens : public Ship
{
public:
    
    Aliens(StudentWorld* world, int imageID, double hitpoints, double damageAmt, double speed, unsigned int scoreValue, double startX, double startY);
    
    virtual ~Aliens();
    
    virtual void doSomething();

    virtual bool isAliens() const;
    virtual void sufferDamage(double amt, int cause);
    

    //determine if aliens will fire projectiles or perform some other form of attack to Nachenblaster
    virtual bool attack();
    
    //damages Nachenblaster and suffer damage from Nachenblaster if collided with Nachenblaster
    void collideWNach();
    // If this alien collided with the player, damage the player and return
    // true; otherwise, return false.
    bool damageCollidingPlayer();
    //impose damage and carry out actions caused by death of alien
    void alienDied();
    
    //maybe drop a goodie
    virtual void possiblyDropGoodie();
    
    //sets a new flightplan if alien has reached top or bottom of screen or flight plan length has reached zero
    virtual void setNewFlight();
    
    //move in current travel direction
    virtual void moveCurrent();

    //Accessors:
    
    //return travel speed
    double getSpeed() const;
    
    //return amount of damage alien would cause if collided with Nachenblaster
    double damageAmt() const;
    
    //return amount of score that shoud be increased if alien collided with Nachenblaster
    int scoreValue() const;
    
    //return current travel direction of the alien
    char getTravDir() const;

    //Mutators:
    
    //changes travel speed to "speed" in parameter
    void setSpeed(double speed);
    
    //changes flight length to "fl" in parameter
    void setFlightLength(double fl);
    
    //changed travel direction to "dir" in parameter
    void setTravDir(char dir);
    
    //returns a random travel direction (U, D, OR, L)
    char randomDir();
    
private:
    
    double m_damageAmt;
    double m_travelspeed;
    double m_flightlength;
    char m_travdir;
    unsigned int m_scoreValue; 
};


///////////////////////////////////////////
//Smallgon's Declaration and Inlines
///////////////////////////////////////////

class Smallgon : public Aliens
{
public:
    Smallgon(StudentWorld* world);
    virtual ~Smallgon();
    
private:
};

///////////////////////////////////////////
//Smoregon's Declaration and Inlines
///////////////////////////////////////////

class Smoregon :public Aliens
{
public:
    Smoregon(StudentWorld* world);
    virtual ~Smoregon();
    //returns true if a Smoregon attack has been made
    virtual bool attack();
    //possibly drop a repair or flatulence torpedo goodie
    virtual void possiblyDropGoodie();

private:
    
};

///////////////////////////////////////////
//Snagglegon's Declaration and Inlines
///////////////////////////////////////////

class Snagglegon : public Aliens
{
public:
    Snagglegon(StudentWorld* world);
    virtual ~Snagglegon();
    virtual bool attack();
    virtual void setNewFlight();
    virtual void possiblyDropGoodie();
    virtual void moveCurrent();
};


///////////////////////////////////////////
//Goodie's Declaration and Inlines
///////////////////////////////////////////

class Goodie : public Actor
{
public:
    Goodie(StudentWorld* world, double startX, double startY, int imageID);
    virtual ~Goodie();
    virtual void doSomething();
    
    //different goodies do different things when collided with Nachenblaster
    virtual Nachenblaster* collideWNach();
    
private:
};

/////////////////////////////////////////////
//ExtraLifeGoodie's Declaration and Inlines
/////////////////////////////////////////////

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* world, double startX, double startY);
    virtual ~ExtraLifeGoodie();
    
    //give Nachenblaster one extra life if collided
    virtual Nachenblaster* collideWNach();
private:
    
};

/////////////////////////////////////////////
//RepairGoodie's Declaration and Inlines
/////////////////////////////////////////////

class RepairGoodie : public Goodie
{
public:
    RepairGoodie(StudentWorld* world, double startX, double startY);
    virtual ~RepairGoodie();
    virtual Nachenblaster* collideWNach();
    
private:
    
};

////////////////////////////////////////////////////////
//FlatulenceTorpedoGoodie's Declaration and Inlines
////////////////////////////////////////////////////////

class FlatulenceTorpedoGoodie : public Goodie
{
public:
    FlatulenceTorpedoGoodie(StudentWorld* world, double startX, double startY);
    virtual ~FlatulenceTorpedoGoodie();
    virtual Nachenblaster* collideWNach();
    
private:
};

///////////////////////////////////////////
//Projectile's Declaration and Inlines
///////////////////////////////////////////

class Projectiles : public Actor
{
public:
    
    Projectiles(StudentWorld* world, double startX, double startY, int dir, double deltaX, int imageID, double size, int depth, double damageAmt, bool rotates);
    virtual ~Projectiles();
    virtual void doSomething();
    
    //abstract base class
    virtual bool collide() = 0;
    
    //returns the amount of damage projectile will do
    double damageAmt() const;
    
    //returns how much the projectile moves in the x direction for each tick
    double deltX() const;
    
    //returns whether the projectile rotates
    bool rotates() const;
    
private:
    double m_damageAmt;
    double m_deltX;
    bool m_rotates;
};

///////////////////////////////////////////
//Cabbage's Declaration and Inlines
///////////////////////////////////////////

class Cabbage : public Projectiles
{
public:
    
    Cabbage(StudentWorld* world, double startX, double startY);
    virtual ~Cabbage();
    virtual bool collide();

private:
};

///////////////////////////////////////////
//Turnip's Declaration and Inlines
///////////////////////////////////////////

class Turnip: public Projectiles
{
public:
    
    Turnip(StudentWorld* world, double startX, double startY);
    virtual ~Turnip();
    virtual bool collide();
    
private:
    
};

////////////////////////////////////////////////
//Flatulence Torpedo's Declaration and Inlines
////////////////////////////////////////////////

class FlatulenceTorpedo : public Projectiles
{
public:
    FlatulenceTorpedo(StudentWorld* world, double startX, double startY, int imageDir, double deltaX);
    virtual ~FlatulenceTorpedo();
private:
    
};

//////////////////////////////////////////////////
//PlayerLaunchedTorpedo's Declaration and Inlines
//////////////////////////////////////////////////

class PlayerLaunchedTorpedo: public FlatulenceTorpedo
{
public:
    PlayerLaunchedTorpedo(StudentWorld* world, double startX, double startY);
    virtual ~PlayerLaunchedTorpedo();
    virtual bool collide(); //check for collision with aliens

private:
    
    
};

//////////////////////////////////////////////////
//AlienLaunchedTorpedo's Declaration and Inlines
//////////////////////////////////////////////////

class AlienLaunchedTorpedo: public FlatulenceTorpedo
{
public:
    AlienLaunchedTorpedo(StudentWorld* world, double startX, double startY);
    virtual ~AlienLaunchedTorpedo();
    virtual bool collide(); //check for collision with nachenblaster
    
private:
    
    
};

///////////////////////////////////////////
//Explosion's Declaration and Inlines
///////////////////////////////////////////


class Explosion : public Actor
{
public:
    Explosion(StudentWorld* world, double startX, double startY);
    virtual ~Explosion();
    virtual void doSomething();
    
private:
    int count;
};

 
///////////////////////////////////////////
//Star's Declaration and Inlines
///////////////////////////////////////////

class Star : public Actor
{
public:
    
    Star(StudentWorld* world, double startX, double startY);
    virtual ~Star();
    virtual void doSomething();
    
private:
};

#endif // ACTOR_H_
