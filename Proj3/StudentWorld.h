#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
#include <list>

const int S1 = 60;

class Actor;
class Nachenblaster;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    //allow student world to get objects newly added to the game
    void getObjects(Actor* obj);
    
    //returns a pointer to nachenblaster if "a" collided with nachenblaster
    Nachenblaster* getCollidingPlayer(const Actor* a) const;
    
    //returns a pointer to an alien that has collided with "a"
    Actor* getOneCollidingAlien(const Actor* a) const;    
    
    //determine y distance is within range for aliens to make an attack
    bool playerInLineOfFire(const Actor* a) const; 
    
    //check if actor1 and actor2 has collided using euclidian distance
    bool collide(const Actor * actor1, const Actor * actor2) const;
    
    //update the number of aliens destoryed (D)
    void destroyedAliens();
    
    //update game statistic
    void updateGameStats();
    
    
private:
    
    //determines whether new aliens should be added to the game
    void newAliens();
    //determines whether new stars should be added to the game
    void newStars();
    
    //list that holds pointers to actors
    std::list<Actor*> al;
    
    //iterator to the list
    std::list<Actor*> :: iterator it;
    
    //pointer to nachenblaster
    Nachenblaster* nachp;
    
    int T; //number of ships that must be destroyed at current level
    int M; //max number of alien ships that should be on screen

    int D; //number of ships destroyed at current level
    int R; //number of remaining ships that must be destroyed
    int CUR; //current number of alien ships on screen
    int S2; //use to determine what kind of alien should be added to game (see newAliens())
    int S3; //use to determine what kind of alien should be added to game (see newAliens)
    int S; //use to determine what kind of alien should be added to game (see newAliens)
    
};

#endif // STUDENTWORLD_H_
