/* 
 * File:   squares.cpp
 * Author: lion
 * 
 * Created on 30 mai 2015, 18:08
 */

#include "squares.h"

string IntToString(int a) {
    ostringstream temp;
    temp << a;
    return temp.str();
}

string result(int * squares) {

    string output = "[";
    for (int* ptrSquares = squares; *ptrSquares >= 0; ptrSquares += 3) {
        const int row = *(ptrSquares + 1);
        const int col = *ptrSquares;
        const int square_size = *(ptrSquares + 2);

        if (output != "[") {
            output += ",";
        }
        output += "{ \"X\" : " + IntToString(col - 1) + ", \"Y\" : " + IntToString(row - 1) + ", \"size\" : " + IntToString(square_size) + " }";



    }

    return output + "]";


}

void clear_square(int * puzzle, const int width, const int * square) {

    const int row = *(square + 1);
    const int col = *square;
    const int square_size = *(square + 2);
    const int * ptrLineMax = puzzle + col + (row + square_size) * width;
    for (int* ptrLine = puzzle + col + (row) * width; ptrLine < ptrLineMax; ptrLine += width) {
        const int * ptrColMax = ptrLine + square_size;
        for (int* ptrCol = ptrLine; ptrCol < ptrColMax; ++ptrCol) {
            *ptrCol = 1;
        }
    }

}

int clear_squares(int * puzzle, const int width, int * area, int * squares_removed) {
    int nb_squares_removed = 0;
    int first_col = area[0];
    int first_row = area[1];
    int last_col = area[2];
    int last_row = area[3];

    const int * ptrLineMax = puzzle + last_row * width;
    int row = first_row;
    for (int* ptrLine = puzzle + (first_row) * width; ptrLine <= ptrLineMax; ptrLine += width) {
        const int * ptrColMax = ptrLine + last_col;
        int col = first_col;
        for (int* ptrCol = ptrLine + first_col; ptrCol <= ptrColMax; ++ptrCol) {

            if (*ptrCol > 1) {
                const int id = *ptrCol;

                int size = 1;
                int xoffset = 0;
                int yoffset = 0;
                int* gridPos = ptrCol - width;

                while (*gridPos == id) {
                    gridPos -= width;
                    yoffset -= 1;
                }
                gridPos = ptrCol - 1;
                while (*gridPos == id) {
                    gridPos -= 1;
                    xoffset -= 1;
                }

                gridPos = ptrCol + 1 + width;
                while (*gridPos == id) {
                    gridPos += width + 1;
                    ++size;
                }

                size += (min(xoffset, yoffset)*-1);
                if (col + xoffset < area[0]) {
                    area[0] = col + xoffset;
                }
                if (row + yoffset < area[1]) {
                    area[1] = row + yoffset;
                }
                if (col + xoffset + size - 1 > area[2]) {
                    area[2] = col + xoffset + size - 1;
                }
                if (row + yoffset + size - 1 > area[3]) {
                    area[3] = row + yoffset + size - 1;
                }
                *squares_removed = col + xoffset;
                *(squares_removed + 1) = row + yoffset;
                *(squares_removed + 2) = size;
                clear_square(puzzle, width, squares_removed);
                ++nb_squares_removed;
                squares_removed += 3;
            }
            ++col;
        }
        ++row;
    }
    *squares_removed = -1;
    return nb_squares_removed;
}

void use_squares(int * puzzle, const int width, int * squares) {
    for (int* ptrSquares = squares; *ptrSquares >= 0; ptrSquares += 3) {
        const int row = *(ptrSquares + 1);
        const int col = *ptrSquares;
        const int square_size = *(ptrSquares + 2);
        const int id = (row) * width + col;
        const int * ptrLineMax = puzzle + col + (row + square_size) * width;
        for (int* ptrLine = puzzle + col + (row) * width; ptrLine < ptrLineMax; ptrLine += width) {
            const int * ptrColMax = ptrLine + square_size;
            for (int* ptrCol = ptrLine; ptrCol < ptrColMax; ++ptrCol) {
                *ptrCol = id;
            }
        }
    }
}

int solver(int * puzzle, const int width, int orientation, const int *const area) {
    int id;
    int nb_squares_used = 0;
    //code here was duplicated in order to gain performance
    if (orientation == 0) {
        int * ptrLineMin = puzzle + area[0] + area[1] * width;
        const int * ptrLineMax = puzzle + area[0] + area[3] * width;
        int row = area[1];

        for (int* ptrLine = ptrLineMin; ptrLine <= ptrLineMax; ptrLine += width, ++row) {
            int col = area[0];
            const int computed_line = row * width;
            const int * ptrColMax = ptrLine + area[2] - area[0];
            for (int* ptrCol = ptrLine; ptrCol <= ptrColMax; ++ptrCol, ++col) {

                if (*ptrCol == 1) {
                    id = computed_line + col;
                    ++nb_squares_used;
                    *ptrCol = id;
                    int size = 1;
                    while (true) {
                        bool stop = false;
                        int * ptrCol2 = ptrCol + size * width;
                        if (*(ptrCol2 + size) != 1)break;
                        for (int i = 0; i < size; ++i) {
                            if (*(ptrCol2 + i) != 1 || *(ptrCol + i * width + size) != 1) {
                                stop = true;
                                break;
                            }
                        }
                        if (stop) {
                            break;
                        }

                        for (int i = 0; i < size; ++i) {
                            *(ptrCol2 + i) = id;
                            *(ptrCol + i * width + size) = id;
                        }
                        *(ptrCol2 + size) = id;
                        ++size;
                    }
                }

            }

        }

    } else if (orientation == 1) {
        int * ptrLineMin = puzzle + area[2] + area[3] * width;
        const int * ptrLineMax = puzzle + area[2] + area[1] * width;
        int row = area[3];

        for (int* ptrLine = ptrLineMin; ptrLine >= ptrLineMax; ptrLine -= width, --row) {
            int col = area[2];
            const int computed_line = row * width;
            const int * ptrColMax = ptrLine + area[0] - area[2];
            for (int* ptrCol = ptrLine; ptrCol >= ptrColMax; --ptrCol, --col) {

                if (*ptrCol == 1) {
                    id = computed_line + col;
                    ++nb_squares_used;
                    *ptrCol = id;
                    int size = 1;
                    while (true) {
                        bool stop = false;
                        int * ptrCol2 = ptrCol - size * width;
                        if (*(ptrCol2 - size) != 1)break;
                        for (int i = 0; i < size; ++i) {
                            if (*(ptrCol2 - i) != 1 || *(ptrCol - i * width - size) != 1) {
                                stop = true;
                                break;
                            }
                        }
                        if (stop) {
                            break;
                        }

                        for (int i = 0; i < size; ++i) {
                            *(ptrCol2 - i) = id;
                            *(ptrCol - i * width - size) = id;
                        }
                        *(ptrCol2 - size) = id;
                        ++size;
                    }
                }

            }

        }
    } else if (orientation == 2) {
        int * ptrLineMin = puzzle + area[0] + area[3] * width;
        const int * ptrLineMax = puzzle + area[0] + area[1] * width;
        int row = area[3];

        for (int* ptrLine = ptrLineMin; ptrLine >= ptrLineMax; ptrLine -= width, --row) {
            int col = area[0];
            const int computed_line = row * width;
            const int * ptrColMax = ptrLine + area[2] - area[0];
            for (int* ptrCol = ptrLine; ptrCol <= ptrColMax; ++ptrCol, ++col) {

                if (*ptrCol == 1) {
                    id = computed_line + col;
                    ++nb_squares_used;
                    *ptrCol = id;
                    int size = 1;
                    while (true) {
                        bool stop = false;
                        int * ptrCol2 = ptrCol - size * width;
                        if (*(ptrCol2 + size) != 1)break;
                        for (int i = 0; i < size; ++i) {
                            if (*(ptrCol2 + i) != 1 || *(ptrCol - i * width + size) != 1) {
                                stop = true;
                                break;
                            }
                        }
                        if (stop) {
                            break;
                        }

                        for (int i = 0; i < size; ++i) {
                            *(ptrCol2 + i) = id;
                            *(ptrCol - i * width + size) = id;
                        }
                        *(ptrCol2 + size) = id;
                        ++size;
                    }
                }

            }

        }

    } else {
        int * ptrLineMin = puzzle + area[2] + area[1] * width;
        const int * ptrLineMax = puzzle + area[2] + area[3] * width;
        int row = area[1];

        for (int* ptrLine = ptrLineMin; ptrLine <= ptrLineMax; ptrLine += width, ++row) {
            int col = area[2];
            const int computed_line = row * width;
            const int * ptrColMax = ptrLine + area[0] - area[2];
            for (int* ptrCol = ptrLine; ptrCol >= ptrColMax; --ptrCol, --col) {

                if (*ptrCol == 1) {
                    id = computed_line + col;
                    ++nb_squares_used;
                    *ptrCol = id;
                    int size = 1;
                    while (true) {
                        bool stop = false;
                        int * ptrCol2 = ptrCol + size * width;
                        if (*(ptrCol2 - size) != 1)break;
                        for (int i = 0; i < size; ++i) {
                            if (*(ptrCol2 - i) != 1 || *(ptrCol + i * width - size) != 1) {
                                stop = true;
                                break;
                            }
                        }
                        if (stop) {
                            break;
                        }

                        for (int i = 0; i < size; ++i) {
                            *(ptrCol2 - i) = id;
                            *(ptrCol + i * width - size) = id;
                        }
                        *(ptrCol2 - size) = id;
                        ++size;
                    }
                }

            }

        }

    }


    return nb_squares_used;
}

