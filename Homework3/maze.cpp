
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr == er && sc == ec)
        return true;
    
    maze[sr][sc] = '#';
    
    //if EAST is open and undiscovered
    if (maze[sr][sc+1] == '.')
        if (pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
            return true;
    
    //if SOUTH is open and undiscovered
    if (maze[sr+1][sc] == '.')
        if (pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
            return true;
    
    //if WEST is open and undiscovered
    if (maze[sr][sc-1] == '.')
        if (pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
            return true;
    
    
    //if NORTH is open and undiscovered
    if (maze[sr-1][sc] == '.')
        if (pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
            return true;
    
    return false;
}

