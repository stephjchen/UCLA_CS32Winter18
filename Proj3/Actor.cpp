#include "Actor.h"
#include "StudentWorld.h"

#include "GameConstants.h"
#include <math.h>
#include <list>
#include <algorithm>

#include <iostream>
using namespace std;


//////////////////////////////
// Actor's Implementation
//////////////////////////////

Actor:: Actor(StudentWorld* world, double startX, double startY, int imageID, int dir, double size, int depth): GraphObject(imageID, startX, startY, dir, size, depth), wp(world), m_alive(true) {}

Actor::~Actor() {}

bool Actor::isInBound(double x, double y) const
{
    if (x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT)
        return false;
    
    return true;
}

bool Actor::isAliens() const
{
    return false;
}

void Actor::sufferDamage(double amt, int cause)
{
    return;
}

bool Actor::alive() const
{
    return m_alive;
}

void Actor::setDead()
{
    m_alive = false;
}

StudentWorld* Actor::getWorld()
{
    return wp;
}

///////////////////////////////////////
// Ship's Implementation
///////////////////////////////////////

Ship::Ship(StudentWorld* world, double startX, double startY, int imageID, double hitpoints, int dir, double size, int depth): Actor(world, startX, startY, imageID, dir, size, depth), m_hit(hitpoints){}

Ship::~Ship(){}

double Ship::getHitPoints() const
{
    return m_hit;
}

//increases hitpoints by amount passed in from parameter
void Ship::setHitPoints (double hitpoints)
{
    m_hit += hitpoints;
}

///////////////////////////////////////
// Nachenblaster's Implementation
///////////////////////////////////////

//Ship(StudentWorld* world, double startX, double startY, int imageID, double hitpoints, int dir, double size, int depth)

Nachenblaster::Nachenblaster(StudentWorld* world): Ship(world, 0, 128, IID_NACHENBLASTER, 50, 0, 1.0, 0), m_cabEnergy(30), m_torpedoes(0)
{}

Nachenblaster::~Nachenblaster(){}

int Nachenblaster::cabbagePct() const
{
    return (m_cabEnergy/30)*100;
}

int Nachenblaster::healthPct() const
{
    return (getHitPoints()/50)*100;
}

void Nachenblaster::increaseHitPoints(double num)
{
    if (getHitPoints()+num <= 50)
        setHitPoints(num);
}

void Nachenblaster::increaseTorpedoes(int amt)
{
    m_torpedoes += amt;
}

int Nachenblaster::numTorpedoes() const
{
    return m_torpedoes;
}

void Nachenblaster::sufferDamage(double amt, int cause)
{
    if (cause == HIT_BY_PROJECTILE)
    {
        getWorld()->playSound(SOUND_BLAST);
    }
    
    setHitPoints(amt); //decreases Nachenblaster's hitpoints
    
    if (getHitPoints() <= 0) //resulting damage caused death of Nachenblaster
        setDead();
}

void Nachenblaster::doSomething()
{
    //check if Nachenblaster has died
    if (!alive())
        return;
    
    //continue to increase cabbage energy until its full amount
    if (m_cabEnergy < 30)
        m_cabEnergy++;
    
    int ch = 0;
    if (getWorld() -> getKey(ch)) //make a move if player pressed a directional, space, or tab key
    {
        switch (ch)
        {
            //only move in direction indicated if resulting move will not cause nachenblaster to move offscreen
            case KEY_PRESS_LEFT:
                if (isInBound(getX()-6, getY()))
                    moveTo(getX()-6, getY());
                break;
            case KEY_PRESS_RIGHT:
                if (isInBound(getX()+6, getY()))
                    moveTo(getX()+6, getY());
                break;
            case KEY_PRESS_UP:
                if (isInBound(getX(), getY()+6))
                    moveTo(getX(), getY()+6);
                break;
            case KEY_PRESS_DOWN:
                if (isInBound(getX(), getY()-6))
                    moveTo(getX(), getY()-6);
                break;
                
            //fire cabbage and decreases cabbage energy accordingly
            case KEY_PRESS_SPACE:
                if (m_cabEnergy >= 5)
                {
                    m_cabEnergy -= 5;
                    Cabbage* cp = new Cabbage(getWorld(), getX()+12, getY());
                    getWorld()->playSound(SOUND_PLAYER_SHOOT);
                    getWorld()->getObjects(cp); //pass the cabbage for studentWorld to manage
                }
                
                break;
            
            //fire torpedo if there nachenblaster has acquired torpedo throughout the game, decrease torpedo count accordingly
            case KEY_PRESS_TAB:
                if (m_torpedoes > 0)
                {
                    PlayerLaunchedTorpedo* ptp = new PlayerLaunchedTorpedo(getWorld(), getX()+12, getY());
                    getWorld()->getObjects(ptp);
                    m_torpedoes--;
                }
                
                break;
                
        }
        
    }
}

/////////////////////////////
// Aliens' Implementation
/////////////////////////////

Aliens::Aliens(StudentWorld* world, int imageID, double hitpoints, double damageAmt, double speed, unsigned int scoreValue, double startX = VIEW_WIDTH-1, double startY = (double)randInt(0,VIEW_HEIGHT-1)/1.0): Ship(world, startX, startY, imageID, hitpoints, 0, 1.5, 1), m_damageAmt(damageAmt), m_travelspeed(speed), m_flightlength(0), m_scoreValue(scoreValue){}

Aliens::~Aliens(){}

bool Aliens::isAliens() const
{
    return true;
}

double Aliens::getSpeed() const
{
    return m_travelspeed;
}

double Aliens::damageAmt() const
{
    return m_damageAmt;
}

int Aliens::scoreValue() const
{
    return m_scoreValue;
}

char Aliens::getTravDir() const
{
    return m_travdir;
}

// If this alien collided with the player, damage the player and return
// true; otherwise, return false.
bool Aliens::damageCollidingPlayer()
{
    Nachenblaster* player = getWorld()->getCollidingPlayer(this);
    if ( player != nullptr) //a collision has occured between an alien and Nachenblaster
    {
        player->sufferDamage(damageAmt(), HIT_BY_SHIP); //make player suffer alien damage
        return true;
    }
    //a collision with nachenblaster did not occur
    return false;
}

void Aliens::alienDied()
{
    //play sound SOUND_DEATH due to collision with Nachenblaster
    getWorld()->playSound(SOUND_DEATH);
    
    //indicate to Student World an alien has been destroyed
    getWorld()->destroyedAliens();
    
    //alien is now dead
    setDead();
    
    //introduce a new explosion
    Explosion* ep = new Explosion(getWorld(), getX(), getY());
    getWorld()->getObjects(ep);
    
    //increases the score of by the alien's scoreValue
    getWorld()->increaseScore(scoreValue());
    
    //might drop a goodie as result of the collision
    possiblyDropGoodie();
}

void Aliens::sufferDamage(double amt, int cause)
{
    //suffer damage from colliding with Nachenblaster
    if (cause == HIT_BY_SHIP)
        alienDied();
    
    else if (cause == HIT_BY_PROJECTILE) //suffer damage from projectile
    {
        setHitPoints(amt); //decreases hitpoints by the damage caused by the projectile
        
        if(getHitPoints() <= 0)
            alienDied();
        else
        //if alien is injured but did not die, play sound SOUND_BLAST
        getWorld()->playSound(SOUND_BLAST);
    }
}

void Aliens::moveCurrent()
{
    //alien is moving up and left
    if (m_travdir == 'U')
        moveTo(getX() - getSpeed(), getY() + getSpeed());
    else if (m_travdir == 'D')
        moveTo(getX() - getSpeed(), getY() - getSpeed());
    else if (m_travdir == 'L')
        moveTo(getX() - getSpeed(), getY());
    
    m_flightlength --;
}


char Aliens::randomDir()
{
    char arr[3] = {'D', 'U' ,'L'};
    int num = randInt(0,2); //randomly choose a number from 0~2, inclusive
    return arr[num]; //return the char at the randomly chosen position
}

void Aliens::setFlightLength(double fl)
{
    m_flightlength = fl;
}

void Aliens::setSpeed(double speed)
{
    m_travelspeed = speed;
}

void Aliens::setNewFlight()
{
    if (m_flightlength <= 0 || getY() >= VIEW_HEIGHT - 1 || getY() <= 0 )
    {
        //alien has reached top of screen
        if (getY() >= VIEW_HEIGHT-1)
        {
            m_travdir = 'D'; //reset travel direction to down and left
        }
        //alien has reached bottom of screen
        else if (getY() <= 0)
        {
            m_travdir = 'U'; //reset travel direction to up and left
        }
        //alien's flightlength reached zero
        else if (m_flightlength <= 0)
        {
            m_travdir = randomDir(); //reset travel direction to a random direction
        }
        
        //set the new flight plan length to a random length
        setFlightLength((double)randInt(1,32)/1.0);
    }
}

bool Aliens::attack()
{
    int chance = randInt(1, 20 + 5 * (getWorld()->getLevel()) );
    if  (chance <= (getWorld()->getLevel()))
    {
        getWorld()->playSound(SOUND_ALIEN_SHOOT);
        Turnip* tp = new Turnip(getWorld(), getX()-14, getY());
        getWorld()->getObjects(tp);
        return true;
    }
    return false;
}

void Aliens::setTravDir(char dir)
{
    m_travdir = dir;
}

// If this alien drops goodies, drop one with the appropriate probability.
void Aliens::possiblyDropGoodie()
{
    return; //aliens are set to not drop goodie by default
}

void Aliens::collideWNach()
{
    if (damageCollidingPlayer()) //if collided with Nachenblaster, damage Nachenblaster
        this->sufferDamage(-1, HIT_BY_SHIP); //Aliens also suffer damage from Nachenblaster
}

void Aliens::doSomething()
{
    if (!alive()) //check if alien is still alive
        return;
    
    if (getX() < 0) //if alien flew off right side of screen, set itself dead
    {
        setDead();
        return;
    }
    
    //check if it has collided with nachenblaster, checking for collision with nachenblaster projectile will be implemented in projectile's dosomething
    collideWNach();
    //if alien needs a new flight plan, set a new flight plan
    setNewFlight();
    
    //if nachenblaster is in line of fire, the alien might cause an attack
    if (getWorld()->playerInLineOfFire(this))
        if (attack())
            return;
    
    //move in its current travel direction
    moveCurrent();
    
    //check again if alien has collided with nachenblaster
    collideWNach();
}

/////////////////////////////
// Smallgon's Implementation
/////////////////////////////

Smallgon::Smallgon(StudentWorld* world): Aliens(world, IID_SMALLGON, 5*(1+((world->getLevel())-1)*.1), -5, 2.0, 250)
{}

Smallgon::~Smallgon() {}

/////////////////////////////
// Smoregon's Implementation
/////////////////////////////

Smoregon::Smoregon(StudentWorld* world): Aliens(world, IID_SMOREGON,5*(1+((world->getLevel())-1)*.1), -5, 2.0, 250)
{}

Smoregon::~Smoregon() {}

bool Smoregon::attack()
{
    int chance = randInt(1, 20 + 5 * (getWorld()->getLevel()) );
    
    if (Aliens::attack())
        return true;

    //if a turnip was not fire, possibly ram towards Nachenblaster
    //20/getWorld()->getLevel())+5 < 1
    else if ( chance <= (getWorld()->getLevel()) )
    {
        setTravDir('L');
        setFlightLength(VIEW_WIDTH);
        setSpeed(5);
        return true;
    }
    return false; //returns false to doSomething if an attack was not carried out
}

void Smoregon::possiblyDropGoodie()
{

        int chance = randInt(1,3);
        if (chance == 1) //1/3 chance Smoregon will drop a goodie
        {
            int chance2 = randInt(1,2);
            
            if (chance2 == 1) //1/2 chance Smoregon will drop a repair goodie
            {
                RepairGoodie* rgp = new RepairGoodie(getWorld(), getX(), getY());
                getWorld()->getObjects(rgp);
            }
            else
            {
                FlatulenceTorpedoGoodie* fgp = new FlatulenceTorpedoGoodie(getWorld(), getX(), getY());
                getWorld()->getObjects(fgp);
            }
            
        }

}

////////////////////////////////
// Snagglegon's Implementation
////////////////////////////////

//Aliens(StudentWorld* world, int imageID, double hitpoints, double damageAmt, double speed, unsigned int scoreValue)
Snagglegon::Snagglegon(StudentWorld* world): Aliens(world, IID_SNAGGLEGON, 10*(1+((world->getLevel())-1)*.1), -15, 1.75, 1000)
{
    setTravDir('D');
}

Snagglegon::~Snagglegon() {}

void Snagglegon::moveCurrent()
{
    if ( getTravDir()== 'U')
        moveTo(getX() - getSpeed(), getY() + getSpeed());
    else if ( getTravDir() == 'D')
        moveTo(getX() - getSpeed(), getY() - getSpeed());
}

bool Snagglegon::attack()
{
    int chance = randInt(1, 15 + 10 * (getWorld()->getLevel()) );
    //determine if snagglegon will make an attack
    if (chance <= (getWorld()->getLevel()))
    {
        //launch a torpedo
        AlienLaunchedTorpedo* atp = new AlienLaunchedTorpedo(getWorld(),getX()-14, getY());
        getWorld()->getObjects(atp);
        getWorld()->playSound(SOUND_TORPEDO);
        return true;
    }
    return false;
}

void Snagglegon::setNewFlight()
{
    if (getY() >= VIEW_HEIGHT-1)
        setTravDir('D'); //move down and left
    
    else if (getY() <= 0)
        setTravDir('U'); //move up and left
}

void Snagglegon::possiblyDropGoodie()
{
        int chance = randInt(1,6);
        if (chance == 1) //1/6 chance Smoregon will drop an ExtraLifeGoodie
        {
            ExtraLifeGoodie* egp = new ExtraLifeGoodie(getWorld(), getX(), getY());
            getWorld()->getObjects(egp);
        }
}

///////////////////////////////////////////
//Goodie's Implementation
///////////////////////////////////////////

//Actor(StudentWorld* world, double startX, double startY, int imageID, int dir, double size, int depth)

Goodie::Goodie(StudentWorld* world, double startX, double startY, int imageID):Actor(world, startX, startY, imageID, 0, 0.5, 1)
{}

Goodie::~Goodie(){}

Nachenblaster* Goodie::collideWNach()
{
    if (getWorld()->getCollidingPlayer(this) != nullptr)
    {
        getWorld()->playSound(SOUND_GOODIE);
        getWorld()->increaseScore(100);    //increases 100 points for all kinds of goodies
        return getWorld()->getCollidingPlayer(this);
    }
    return nullptr;
}

void Goodie::doSomething()
{
    if (!alive())
        return;
    
    if (! isInBound(getX(), getY()) )
    {
        setDead();
        return;
    }
    
    //check if the goodie collided with Nachenblaster
    if (collideWNach() != nullptr)
    {
        setDead();
        return;
    }
    
    //move down and left by 0.75 units in each direction
    moveTo(getX()-0.75, getY()-0.75);
    
    if (collideWNach() != nullptr)
    {
        setDead();
        return;
    }
}

///////////////////////////////////////////
//ExtraLifeGoodie's Implementation
///////////////////////////////////////////

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* world, double startX, double startY): Goodie(world, startX, startY, IID_LIFE_GOODIE){}

ExtraLifeGoodie::~ExtraLifeGoodie(){}

Nachenblaster* ExtraLifeGoodie::collideWNach()
{
    if (Goodie::collideWNach() != nullptr) //calls Goodie's version of collideWNach
    {
        getWorld()->incLives(); //increase Nachenblaster's life by 1
        return Goodie::collideWNach();
    }
    return nullptr;
}

///////////////////////////////////////////
//RepairGoodie's Implementation
///////////////////////////////////////////

RepairGoodie::RepairGoodie(StudentWorld* world, double startX, double startY):Goodie(world, startX, startY, IID_REPAIR_GOODIE){}

RepairGoodie::~RepairGoodie(){}

Nachenblaster* RepairGoodie::collideWNach()
{
    if (Goodie::collideWNach() != nullptr)
    {
        (Goodie::collideWNach())->increaseHitPoints(10); //increases Nachenblaster's hitpoints by 10
        return Goodie::collideWNach();
    }
    return nullptr;
}

///////////////////////////////////////////
//FlatulenceTorpedoGoodie's Implementation
///////////////////////////////////////////

FlatulenceTorpedoGoodie::FlatulenceTorpedoGoodie(StudentWorld* world, double startX, double startY):Goodie(world, startX, startY, IID_TORPEDO_GOODIE){}

FlatulenceTorpedoGoodie::~FlatulenceTorpedoGoodie() {}

Nachenblaster* FlatulenceTorpedoGoodie::collideWNach()
{
    if (Goodie::collideWNach() != nullptr)
    {
        (Goodie::collideWNach())->increaseTorpedoes(5); //increases Nachenblaster's torpedo count by 5
        return Goodie::collideWNach();
    }
    return nullptr;
}


///////////////////////////////////////////
//Projectile's Implementation
///////////////////////////////////////////

Projectiles::Projectiles(StudentWorld* world, double startX, double startY, int dir, double deltaX, int imageID, double size, int depth, double damageAmt, bool rotates): Actor(world, startX, startY, imageID, dir, size, depth), m_damageAmt(damageAmt), m_deltX(deltaX), m_rotates(rotates){}

Projectiles::~Projectiles() {}

void Projectiles::doSomething()
{
    if (!alive())
        return;
    
    if (!isInBound(getX(), getY()))
    {
        setDead();
        return;
    }
    
    if (collide())
        return;
    
    moveTo(getX()+ deltX(), getY());
    
    if (rotates())
    {
        setDirection(getDirection()+20);
    }
    
    if (collide())
        return;
}

//Accessors
double Projectiles::damageAmt() const
{
    return m_damageAmt;
}
double Projectiles::deltX() const
{
    return m_deltX;
}
bool Projectiles::rotates() const
{
    return m_rotates;
}

/////////////////////////////
// Cabbage's Implementation
/////////////////////////////

Cabbage::Cabbage(StudentWorld* world, double startX, double startY): Projectiles(world, startX, startY, 0, 8, IID_CABBAGE, 0.5, 1, -2, true){}
Cabbage::~Cabbage(){}

bool Cabbage::collide()
{
    if ((getWorld()->getOneCollidingAlien(this)) != nullptr) //has collided with an alien
    {
        (getWorld()->getOneCollidingAlien(this))->sufferDamage(damageAmt(), HIT_BY_PROJECTILE);
        setDead();
        return true;
    }
    return false;
}

/////////////////////////////
// Turnip's Implementation
/////////////////////////////

Turnip::Turnip(StudentWorld* world, double startX, double startY): Projectiles(world, startX, startY, 0, -6, IID_TURNIP, 0.5, 1, -2, true){}

Turnip::~Turnip() {}

bool Turnip::collide()
{
    if (getWorld()->getCollidingPlayer(this) != nullptr) //has collided with nachenblaster
    {
        (getWorld()->getCollidingPlayer(this))->sufferDamage(damageAmt(),HIT_BY_PROJECTILE);
        setDead();
        return true;
    }
    return false;
}

/////////////////////////////////////////
// FlatulenceTorepedo's Implementation
/////////////////////////////////////////

FlatulenceTorpedo::FlatulenceTorpedo(StudentWorld* world, double startX, double startY, int imageDir, double deltaX):Projectiles(world, startX, startY, imageDir, deltaX, IID_TORPEDO, 0.5, 1, -8, false){}

FlatulenceTorpedo::~FlatulenceTorpedo(){}

////////////////////////////////////////////
// PlayerLaunchedTorepedo's Implementation
////////////////////////////////////////////

PlayerLaunchedTorpedo::PlayerLaunchedTorpedo(StudentWorld* world, double startX, double startY):FlatulenceTorpedo(world, startX, startY, 0, 8){}

PlayerLaunchedTorpedo::~PlayerLaunchedTorpedo(){}

bool PlayerLaunchedTorpedo::collide()
{
    if (getWorld()->getOneCollidingAlien(this) != nullptr)
    {
        (getWorld()->getOneCollidingAlien(this))->sufferDamage(damageAmt(), HIT_BY_PROJECTILE);
        setDead();
        return true;
    }
    return false;
}


////////////////////////////////////////////
// AlienLaunchedTorepedo's Implementation
////////////////////////////////////////////

////FlatulenceTorpedo(StudentWorld* world, double startX, double startY, int imageDir, double deltaX)

AlienLaunchedTorpedo::AlienLaunchedTorpedo(StudentWorld* world, double startX, double startY):FlatulenceTorpedo(world, startX, startY, 180, -8){}

AlienLaunchedTorpedo::~AlienLaunchedTorpedo() {}

bool AlienLaunchedTorpedo::collide()
{
    if (getWorld()->getCollidingPlayer(this) != nullptr)
    {
        (getWorld()->getCollidingPlayer(this))->sufferDamage(damageAmt(), HIT_BY_PROJECTILE);
        setDead();
        return true;
    }
    return false;
}

///////////////////////////////
// Explosion's Implementation
///////////////////////////////

Explosion::Explosion(StudentWorld* world, double startX, double startY): Actor(world, startX, startY, IID_EXPLOSION, 0, 1, 0), count(0)
{}

Explosion::~Explosion(){}

void Explosion::doSomething()
{
    if (count == 0)
    {
        count++;
        return;
    }
    
    setSize(getSize()*1.5);
    count++;
    
    if (count > 4)
        setDead();
}

//////////////////////////
// Star's Implementation
//////////////////////////
                                                                 
Star::Star(StudentWorld* world, double startX, double startY) : Actor(world, startX, startY, IID_STAR, 0, (double)randInt(5,50)/100, 3) {}

Star::~Star() {}

void Star::doSomething()
{
    if (getX() >= 0)
        moveTo(getX()-1, getY());
    else
        setDead();
}

