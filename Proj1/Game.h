
//

#ifndef Game_h
#define Game_h

class City;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nFlatulans);
    ~Game();
    
    // Mutators
    void play();
    int decodeDirection(char dir);
    
private:
    City* m_city;
};


#endif
