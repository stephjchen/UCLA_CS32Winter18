//
//  History.cpp
//  proj1
//
//  Created by Stephanie Chen on 1/11/18.
//  Copyright © 2018 Stephanie Chen. All rights reserved.
//

#include "History.h"
#include <iostream>

using namespace std;

History::History(int nRows, int nCols)
:m_nRows(nRows), m_nCols(nCols)
{
    for (int i=0; i < m_nRows; i++)
    {   for (int j=0; j < m_nCols; j++)
            grid[i][j]= 0;
    }
}

bool History::record(int r, int c)
{
    if (r > m_nRows || c > m_nCols || r < 1 || c < 1)
        return false;
    
    grid[r-1][c-1] ++;
    return true;
}

void History::display() const
{
   
    char out;
    clearScreen();
    for(int i=0; i < m_nRows; i++)
    {
        for(int j=0; j < m_nCols; j++)
        {
            if(grid[i][j] == 0)
                cout << '.';
            else if(grid[i][j] >= 26)
                cout << 'Z';
            else
            {
                out = 65 + grid[i][j] - 1;
                cout << out;
            }
        }
        cout << endl;
    }
    
    cout << endl;
    
    
    
   }


