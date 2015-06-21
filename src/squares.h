/* 
 * File:   squares.h
 * Author: Lion Cedric
 *
 * Created on 30 mai 2015, 18:08
 */

#ifndef SQUARES_H
#define	SQUARES_H

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <list>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream>
using namespace std;

/** @brief convert interger to string .
 * @param a the integer to convert.
 * @return the string value
 */
string IntToString(int a);

/** @brief convert a table as json.
 * The table format is [x,y,size,x,y,size,x,y,size,x,y,size,....,-1]
 * To come back to original value we also need to substract 1 in x and y (because a row and a column of 0 where added)
 * @param squares the table to convert.
 * @return the table converted
 */
string result(int * squares);

/** @brief remove in the grid the square given in parameter
 * It remove the squqre by resseting all the squares values by 1
 * @param puzzle the grid .
 * @param width  the grid's witdh.
 * @param square  the square to delete [x,y,size].
 */
void clear_square(int * puzzle, const int width, const int * square);

/** @brief remove in the grid the squares given in parameter
 * It remove the squares by resseting all the squares values by 1
 * @param puzzle the grid .
 * @param width  the grid's witdh.
 * @param area delete all the squares in the given area [x0,y0,x1,y1]
 * @param squares_removed the squares deleted [x,y,size,x,y,size,x,y,size,x,y,size,....,-1].
 * @return the number of squares deleted
 */
int clear_squares(int * puzzle, const int width, int * area, int * squares_removed);

/** @brief add in the grid the squares given in parameter
 * It adds all the squares by setting for each squares an id >1
 * this id is generated using the top lef corner of the square (x0*width+y0)
 * First ID is >1 beacuse first row and first col is only 0 (0 is invalid case)
 * @param puzzle the grid .
 * @param width  the grid's witdh.
 * @param area delete all the squares in the given area [x0,y0,x1,y1]
 * @param squares the squares to add in the gid [x,y,size,x,y,size,x,y,size,x,y,size,....,-1].
 * @return the number of squares deleted
 */
void use_squares(int * puzzle, const int width, int * squares);

/** @brief resolve the puzzle inside the given area
 * It adds all the squares by setting each squares values with an id >1
 * this id is generated using the top lef corner of the square (x0*width+y0)
 * First ID is 2 beacuse firs row and first col is only 0 (0 is invalid case)
 * @param puzzle the grid .
 * @param width  the grid's witdh.
 * @param area  resolve only in this area [x0,y0,x1,y1], full area is [1,1,height-2,width-2]
 * @param orientation is the corner of the area where to start resolving .
 * @return the number of squares deleted
 */
int solver(int * puzzle, const int width, int orientation, const int *const area);
#endif	/* SQUARES_H */

