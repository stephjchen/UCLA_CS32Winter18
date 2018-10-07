
#include "globals.h"

#ifndef History_h
#define History_h

class Flatulan;

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;

private:
    int m_nRows;
    int m_nCols;
    int grid[MAXROWS][MAXCOLS];
};


#endif
