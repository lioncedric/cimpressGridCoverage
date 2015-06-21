INTRODUCTION
------------
Cimpress TechChallenge 2015<br/>
The module will resolve a grid (puzzle), trying to find the optimized solution


REQUIREMENTS
------------
It requires the following modules:

 * python 2.7 or higher
 * python easy-install module requests
 * g++
 * mpich2 (mpic++ and mpirun), 
	+ can be installed on ubuntu using : sudo apt-get install libcr-dev mpich2 mpich2-doc

OPTIMIZED FOR
--------------

  *LINUX (UBUNTU) <br/>
  *Intel core i5 ( 2 + 2 cores)<br/>

HOW TO BUILD
------------
Just type make (a makefile is included)

HOW TO RUN THE EXAMPLES
----------------------
When the build successfully completed simply type for a 10 sec run: ./bin/resolve examples/example1.txt 10 200<br/>
To use multi core capabilities type for example: mpirun -np 4 ./bin/resolve examples/example1.txt 10 200<br/>
(200 is the number of neighbour between each iteration, increase this value to get better results if you have more time )<br/>

HOW IS IT WORKING
-----------------
In order to supress addition checks (puzzle limits) a margin full of 0 was also added.<br/>
For example :<br/>
5 6<br/>
0 0 0 0 0<br/>
0 1 1 1 0<br/>
0 1 0 1 0 <br/>
0 1 1 1 0<br/>
0 1 1 1 0<br/>
0 0 0 0 0<br/>
the original puzzle was in fact of size 3 4 (when the result will be generated it will subtract 1 for the X and Y)<br/>
this puzzle will be resolved using a Tabu search (also parallelized with MPI )<br/>
The Tabu search will iterate starting from an initial solution. (it gives different ids for each squares starting from 2)<br/>
0 0 0 0 0<br/>
0 2 3 4 0<br/>
0 5 0 6 0 <br/>
0 7 7 8 0<br/>
0 7 7 9 0<br/>
0 0 0 0 0<br/>
To be able to use this well known algorithm we need a way to generate neigbours from a current solution.<br/>
To create the neigbour we have to :
+ take a random sub-grid from the current grid. <br/>
0 0  0 0  0<br/>
0 2  3 4  0<br/>
0 5  0 6  0 <br/>
0 7 |7 8| 0<br/>
0 7 |7 9| 0<br/>
0 0  0 0  0<br/>
+ Remove the squares inside this sub-grid and recalculate the sub-grid beacause some squares can intersect the sub-grid.<br/>
0  0 0 0  0<br/>
0  2 3 4  0<br/>
0  5 0 6  0 <br/>
0 |1 1 1| 0<br/>
0 |1 1 1| 0<br/>
0  0 0 0  0<br/>
+ Reprocess the sub-grid starting from a random corner. the number of square can differs in the puzzle according to the corner chozen (there is four corners)<br/>
0  0 0 0  0<br/>
0  2 3 4  0<br/>
0  5 0 6  0 <br/>
0 |9 7 7| 0<br/>
0 |8 7 7| 0<br/>
0  0 0 0  0<br/>
+ Recalculate The number of squares in the all puzzle<br/>
	here the result is the same but for complex puzzle it could have been different	<br/>
+ For each iteration we will generate by default 200 neigbours (it is recomended to increase this value if you have pleinty of time or if you have a powerfull system) <br/>
and then start it will the process again with the best neigbour.<br/>

The Tabu serach will then stop after a certain time based on the timeout defined in the command line. <br/>

