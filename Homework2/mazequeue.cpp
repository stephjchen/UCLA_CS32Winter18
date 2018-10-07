//
#include <queue>
#include <string>

using namespace std;


class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
    
    queue<Coord> coordQueue;
    
    Coord ending(er,ec); //declare a coordinate for the ending point
    Coord starting(sr, sc); //declare a coordinate for the starting point

    coordQueue.push(starting);
    maze[sr][sc] = '#'; //mark the starting point

    while(coordQueue.empty() == false)
    {
        Coord curr = coordQueue.front(); //check current position
        coordQueue.pop(); //pop current item off the queue
        
        if (curr.r() == ending.r() && curr.c() == ending.c()) //reached endpoint!
            return true;
        
        if (curr.r()+1 > nRows-1|| curr.r()-1 < 0 || curr.c()+1 > nCols-1 || curr.c()-1 <0) //should not get here
            return false;
        
        //if EAST is open and undiscovered
        if (maze[curr.r()][curr.c()+1] != '#' && maze[curr.r()][curr.c()+1] != 'X')
        {
            maze[curr.r()][curr.c()+1] = '#'; //mark as discovered
            Coord temp(curr.r(), curr.c()+1);
            coordQueue.push(temp); //push coordinate onto queue
        }
        
        //if SOUTH is open and undiscovered
        if (maze[curr.r()+1][curr.c()] != '#' && maze[curr.r()+1][curr.c()] != 'X')
        {
            maze[curr.r()+1][curr.c()] = '#'; //mark as discovered
            Coord temp(curr.r()+1, curr.c());
            coordQueue.push(temp); //push coordinate onto queue
        }
        
        //if WEST is open and undiscovered
        if (maze[curr.r()][curr.c() -1] != '#' && maze[curr.r()][curr.c()-1] != 'X')
        {
            maze[curr.r()][curr.c() -1] = '#'; //mark as discovered
            Coord temp(curr.r(), curr.c()-1);
            coordQueue.push(temp); //push coordinate onto queue
        }
        
        //if NORTH is open and undiscovered
        if (maze[curr.r()-1][curr.c()] != '#' && maze[curr.r()-1][curr.c()] != 'X')
        {
            maze[curr.r()-1][curr.c()] = '#'; //mark as discovered
            Coord temp(curr.r()-1, curr.c());
            coordQueue.push(temp); //push coordinate onto queue
        }
        
    }
    
    return false;

    
    
}

