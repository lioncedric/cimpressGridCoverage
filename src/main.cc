/* 
 * File:   main.cpp
 * Author: LION
 *
 * Created on 12 mai 2015, 21:54
 */

#include <mpi.h>
#include "squares.h"

void algoTabuMPI(int * puzzle_step, const int height, const int width, const int nbNeigbours, const int tps) {
    const int numProc = MPI::COMM_WORLD.Get_rank();
    const int nbProc = MPI::COMM_WORLD.Get_size();
    //initialize the seed for all the process
    int seed;

    if (numProc == 0) {
        srand(time(NULL));
        seed = rand();

        for (int it = 1; it < nbProc; it++) {
            seed = rand();
            MPI::COMM_WORLD.Send(&seed, 1, MPI::INT, it, 10);
        }

    } else {
        MPI::COMM_WORLD.Recv(&seed, 1, MPI::INT, 0, 10);
        srand(seed);
    }

    //timer
    double start = MPI::Wtime();
    double end;

    //the number of increments
    int count = 0;

    //first solution is calculated on the full area starting form the top left corner
    int area[4] = {1, 1, width - 2, height - 2};
    int f_step = solver(puzzle_step, width, 1, area);
    if (numProc == 0) {
        cout << "init : " << f_step << endl;
    }
    //fmin is now the first solution
    int fmin = f_step;
    int fminpuzzle[height * width];
    memcpy(fminpuzzle, puzzle_step, height * width * sizeof *puzzle_step);

    //iterations variables

    int f;
    int test;
    int params[4];
    int fparams[4];
    int squares_removed[height * width * 3]; //for better performance array size is initialized to maximum (instead of a list)

    //taboo list
    list< int > listeS1;

    //start
    do {
        //neighbour solution min 
        f = 9999999;

        for (int nb = 0; nb < nbNeigbours / nbProc; ++nb) {
            //Calculation of the next neighbour
            params[0] = 1 + rand() % (width - 2); //x0
            params[1] = 1 + rand() % (height - 2); //y0 
            params[2] = 1 + rand() % 20; //size
            params[3] = rand() % 4; //solver orientation

            //define the area to recompute
            area[0] = params[0];
            area[1] = params[1];
            area[2] = min(params[0] + params[2], width - 2);
            area[3] = min(params[1] + params[2], height - 2);

            //clear area with 1 and recalulate the area accorting to the squares removed
            int nb_squares_removed = clear_squares(puzzle_step, width, area, squares_removed);
            //compute area previously removed
            int nb_squares_used = solver(puzzle_step, width, params[3], area);
            //get result
            test = f_step - nb_squares_removed + nb_squares_used;

            //save if best neighbour
            if (find(listeS1.begin(), listeS1.end(), test) == listeS1.end()) {
                if (test < f) {
                    fparams[0] = params[0];
                    fparams[1] = params[1];

                    fparams[2] = params[2];
                    fparams[3] = params[3];
                    f = test;
                }
            }
            //back to original to process new neighbour
            use_squares(puzzle_step, width, squares_removed);
        }
        //get best neighbour from all proc
        if (numProc == 0) {
            for (int it = 1; it < nbProc; it++) {
                MPI::COMM_WORLD.Recv(&test, 1, MPI::INT, it, 1);
                MPI::COMM_WORLD.Recv(params, 4, MPI::INT, it, 4);

                if (test < f) {
                    fparams[0] = params[0];
                    fparams[1] = params[1];

                    fparams[2] = params[2];
                    fparams[3] = params[3];
                    f = test;
                }
            }

        } else {
            MPI::COMM_WORLD.Send(&f, 1, MPI::INT, 0, 1);
            MPI::COMM_WORLD.Send(fparams, 4, MPI::INT, 0, 4);
        }
        MPI::COMM_WORLD.Bcast(fparams, 4, MPI::INT, 0);
        MPI::COMM_WORLD.Bcast(&f, 1, MPI::INT, 0);

        //if a neighbour was saved use it for the next iterations
        if (f < 9999999) {
            area[0] = fparams[0];
            area[1] = fparams[1];
            area[2] = min(fparams[0] + fparams[2], width - 2);
            area[3] = min(fparams[1] + fparams[2], height - 2);
            clear_squares(puzzle_step, width, area, squares_removed);
            solver(puzzle_step, width, fparams[3], area);
            f_step = f;
        }
        //update tabu list
        if (listeS1.size() > 3) {
            listeS1.pop_back();
        }
        listeS1.push_front(f);

        if (numProc == 0 && f < fmin) {
            fmin = f_step;
            memcpy(fminpuzzle, puzzle_step, height * width * sizeof *puzzle_step);
            cout << fmin << endl;
        }
        count++;

        //there is no way to know is the best anwser is found so we stop after a certain duration
        end = MPI::Wtime() - start;
        MPI::COMM_WORLD.Bcast(&end, 1, MPI::DOUBLE, 0);


    } while (end < tps);
    if (numProc == 0) {
        memcpy(puzzle_step, fminpuzzle, height * width * sizeof *fminpuzzle);
        cout << "Computation Performances : " << count * nbNeigbours << endl;
    }

}

int main(int argc, char ** argv) {
    //init parameters
    MPI::Init(argc, argv);
    string puzzle_path = "puzzle.txt";
    int duration = 8;
    int nbNeigbours = 200;
    if (argc > 3) {
        puzzle_path = argv[1];
        duration = atoi(argv[2]);
        nbNeigbours = atoi(argv[3]);
    } else {
        if (MPI::COMM_WORLD.Get_rank() == 0)
            cout << "Usage : " << argv[0] << " " << "/puzzle/path.txt int_duration int_nbNeigbours" << endl;
        MPI::Finalize();
        return -1;
    }

    //read the puzzle from file and ad a border full of 0
    std::ifstream file(puzzle_path.c_str());
    int width, height;
    file >> width >> height;
    width+=2;
    height+=2;
    int puzzle[width * height];
    for (int y = 0; y <height ; ++y) {
        for (int x = 0; x <width ; ++x) {
	
    		if(y==0 || y==height-1){
			puzzle[y*width+x]=0;
		}else{
			if(x==0 || x==width-1){
				puzzle[y*width+x]=0;
			}else{
        			file >> puzzle[y*width+x];
			}
   		 }
	}
    }
    //process
    algoTabuMPI(puzzle, height, width, nbNeigbours, duration);
    if (MPI::COMM_WORLD.Get_rank() == 0){
	    //retreive the squares result by clearing the puzzle
	    int area[4] = {1, 1, width - 2, height - 2};
	    int squares_removed[height * width * 3];
	    clear_squares(puzzle, width, area, squares_removed);

	    //print json result
	    cout << "Result:"<<endl;
	    cout << result(squares_removed)<<endl;

    }
    MPI::Finalize();
    return 0;


}
