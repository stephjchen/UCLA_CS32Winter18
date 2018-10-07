#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir); //dynamically allocated StudentWorld obj
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

#include "Actor.h"
#include <list>
#include <algorithm>
#include <iostream>
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw

using namespace std;

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    //dynamically allocate a nachenblaster
    nachp = new Nachenblaster (this);
    
    T = 6+(4 * (getLevel()));
    M = 4 + (0.5 * (getLevel()));
    S2 = 20 + (getLevel() * 5);
    S3 = 5 + (getLevel() * 10);
    S = S1+S2+S3;
    
    D = 0;
    CUR = 0;
    R = T-D;
    
    //dynamically allocate 30 stars
    for (int i=0; i < 30; i++)
    {
        Star * sp = new Star (this, (double)randInt(0, VIEW_WIDTH-1)/1.0, (double)randInt(0, VIEW_HEIGHT-1)/1.0);
        al.push_back(sp); //add each star to container
    }
    
    //finished initializing all data structure/objects for current level
    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    if (nachp->alive())
        nachp->doSomething();
    
    // Give each actor a chance to do something, incl. the NachenBlaster
    //for each of the actors in the game world
    for (it = al.begin(); it != al.end(); it++)
    {
        if ((*it)->alive())
            (*it)->doSomething();
        
        //nachenblaster has died
        if (! (nachp->alive()))
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        else if (D >= T)
            return GWSTATUS_FINISHED_LEVEL;
    }
    
    //Remove newly-dead actors after each tick
    for (it = al.begin(); it != al.end();)
    {
        if (!(*it)->alive())
        {
            if ((*it)->isAliens())
            {
                CUR --;
            }
            delete *it;
            it = al.erase(it);
        }
        else
            it++;
    }
    
    //update status bar
    updateGameStats();
    
    //introduce new stars and Aliens
    newStars();
    newAliens();
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if (nachp != nullptr)
    {
        delete nachp;
        nachp = nullptr;
    }
    
    for (it = al.begin(); it != al.end();)
    {
        if ((*it)!=nullptr)
        {
            delete *it;
            it = al.erase(it);
        }
    }
}

void StudentWorld::getObjects(Actor* obj)
{
    al.push_back(obj);
}

//checks whether collision has occured between any two objects
bool StudentWorld::collide(const Actor * actor1, const Actor * actor2) const
{
    
    if (sqrt(pow((actor1->getX()-actor2->getX()), 2) + pow ((actor1->getY()-actor2->getY()), 2)) < ( .75 * (actor1->getRadius()+actor2->getRadius())) )
        return true;
    
    return false;
}

Nachenblaster* StudentWorld::getCollidingPlayer(const Actor* a) const
{
    if ( collide(a, nachp) )
        return nachp;
    else
        return nullptr;
}

Actor* StudentWorld::getOneCollidingAlien(const Actor* a) const
{
    list<Actor*> :: const_iterator it;
    
    for (it = al.begin(); it != al.end(); it++)
    {
        if ((*it)->isAliens() && collide((*it), a))
            return (*it);
    }
    
    return nullptr;
}

//a pointer to alien will be passed in here for a
bool StudentWorld::playerInLineOfFire(const Actor* a) const
{
    //NachenBlaster is to left of Smallgon
    if (nachp->getX() < a->getX() && sqrt(pow(((a->getY())-(nachp->getY())), 2)) <= 4) //player is in line of fire
        return true;
    return false;
}

void StudentWorld::destroyedAliens()
{
    D++;
}

void StudentWorld::updateGameStats()
{
    ostringstream oss;
    oss << "Lives: " << getLives() << "  ";
    oss << "Health: "<< nachp->healthPct() << "%  ";
    oss << "Score: " << getScore() << "  ";
    oss << "Level: " << getLevel() << "  ";
    oss << "Cabbages: " << nachp->cabbagePct() << "%  ";
    oss << "Torpedoes: " << nachp->numTorpedoes() << "  ";
    
    string gamestats = oss.str();
    setGameStatText(gamestats);
}


//////////////////////////////
// private member functions
//////////////////////////////

void StudentWorld::newAliens()
{
    int chance = randInt(1, S);
    R = T - D;
    
    if ( CUR < (min(M, R)) )
    {
        if (chance >=1 && chance <= S1)
        {
            Smallgon* smap = new Smallgon(this);
            al.push_back(smap);
            CUR++;
        }
        else if (chance > S1 && chance <= S-S3 )
        {
            Smoregon* smop = new Smoregon(this);
            al.push_back(smop);
            CUR++;
        }
        else if (chance > S-S3)
        {
            Snagglegon* snap = new Snagglegon(this);
            al.push_back(snap);
            CUR++;
        }
    }
}

void StudentWorld::newStars()
{
    int num = randInt(1,15);
    if (num == 15)
    {
        Star * sp = new Star(this, VIEW_WIDTH-1, (double)randInt(0, VIEW_HEIGHT-1)/1.0);
        al.push_back(sp); //add each star to container
    }
}
