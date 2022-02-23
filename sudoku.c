#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int board[9][9];
int valid[11];
int subgridCt = 2;


/* Structure for passing data to threads */
typedef struct {
    int row; 
    int column;
} parameters;


/* Now create the thread passing it data as a parameter */
pthread_t column_thread, row_thread, first_thread, second_thread, third_thread, fourth_thread, fifth_thread, sixth_thread, seventh_thread, eigth_thread, ninth_thread;

/* Declare the thread that checks columns and rows */
void *column_worker(void *param);
void *row_worker(void *param);
void *square_worker(void *param);


int main(int argc, char *argv[])
{
    // Read file from agrv[]
	FILE *sudoku; 
	sudoku = fopen(argv[1], "r");

    // Assign numbers to the sudoku board
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			fscanf(sudoku, "%d,", &board[i][j]);
		}
	}

    /* Assign values to structure variables */
    parameters *checkRows = (parameters *) malloc(sizeof(parameters));
    checkRows->row = 0; 
    checkRows->column = 0;

    parameters *checkColumns = (parameters *) malloc(sizeof(parameters));
    checkColumns->row = 0; 
    checkColumns->column = 0;

    parameters *firstSquare = (parameters *) malloc(sizeof(parameters));
    firstSquare->row = 0;
    firstSquare->column = 0;

    parameters *secondSquare = (parameters *) malloc(sizeof(parameters));
    secondSquare->row = 0;
    secondSquare->column = 3;

    parameters *thirdSquare = (parameters *) malloc(sizeof(parameters));
    thirdSquare->row = 0;
    thirdSquare->column = 6;

    parameters *fourthSquare = (parameters *) malloc(sizeof(parameters));
    fourthSquare->row = 3;
    fourthSquare->column = 0;

    parameters *fifthSquare = (parameters *) malloc(sizeof(parameters));
    fifthSquare->row = 3;
    fifthSquare->column = 3;

    parameters *sixthSquare = (parameters *) malloc(sizeof(parameters));
    sixthSquare->row = 3;
    sixthSquare->column = 6;

    parameters *seventhSquare = (parameters *) malloc(sizeof(parameters));
    seventhSquare->row = 6;
    seventhSquare->column = 0;

    parameters *eigthSquare = (parameters *) malloc(sizeof(parameters));
    eigthSquare->row = 6;
    eigthSquare->column = 3;

    parameters *ninthSquare = (parameters *) malloc(sizeof(parameters));
    ninthSquare->row = 6;
    ninthSquare->column = 6;


    /* Create pthread functions */
    pthread_create(&column_thread, NULL, column_worker, (void *) checkColumns);
    pthread_create(&row_thread, NULL, row_worker, (void *) checkRows);
    pthread_create(&first_thread, NULL, square_worker, (void *) firstSquare);
    pthread_create(&second_thread, NULL, square_worker, (void *) secondSquare);
    pthread_create(&third_thread, NULL, square_worker, (void *) thirdSquare);
    pthread_create(&fourth_thread, NULL, square_worker, (void *) fourthSquare);
    pthread_create(&fifth_thread, NULL, square_worker, (void *) fifthSquare);
    pthread_create(&sixth_thread, NULL, square_worker, (void *) sixthSquare);
    pthread_create(&seventh_thread, NULL, square_worker, (void *) seventhSquare);
    pthread_create(&eigth_thread, NULL, square_worker, (void *) eigthSquare);
    pthread_create(&ninth_thread, NULL, square_worker, (void *) ninthSquare);

    /* Call pthread_join() for each children thread so that the parent will wait */
    pthread_join(column_thread, NULL);
    pthread_join(row_thread, NULL);
    pthread_join(first_thread, NULL);
    pthread_join(second_thread, NULL);
    pthread_join(third_thread, NULL);
    pthread_join(fourth_thread, NULL);
    pthread_join(fifth_thread, NULL);
    pthread_join(sixth_thread, NULL);
    pthread_join(seventh_thread, NULL);
    pthread_join(eigth_thread, NULL);
    pthread_join(ninth_thread, NULL);


    /* Verify board, if k=11 the board is solved */ 
    for (int k = 0; k < 11; ++k) {
        if (!valid[k]) {
            printf("\n The Sudoku puzzle is NOT solved! \n");
            exit(0);
        }
    }
    printf("\n The Sudoku puzzle is solved! \n");

    return 0;
}

/* Check all columns */
void *column_worker(void *params)
{
    parameters * column_worker = (parameters *) params;
    int beginColumn = column_worker->column;
    int beginRow = column_worker->row;

    /* Sort sudoku column. Puts the column in a sorted order and checks for duplicates */
    for (int i = beginColumn; i < 9; i++) {

	    int col[9]= {0}; // Array to hold sorted column
        
        for (int j = beginRow; j < 9; j++) {
            
            int val = board[i][j];
            
            /* If the coresponding array element for the value is 0, it has not been seen and 
		    the value will be put into the sorted column array. */
	        if (col[val-1] == 0 && val > 0){
               col[val-1] = val;
            } 
            else {
		    /* If the value is not 0, the value is a duplicate and the sudoku puzzle
		    is not solved or valid. Value of column_worker is 0. */
		    valid[0] = 0;
                pthread_exit(0);
            }
        }
    }   
    valid[0] = 1;
    pthread_exit(0);	       
}

/* Check all rows */
void *row_worker(void *params) {

    parameters * row_worker = (parameters *) params;
    int beginColumn = row_worker->column;
    int beginRow = row_worker->row;
    
    /* Sort sudoku row. Puts the row into a sorted order and check for duplicates */
    for (int i = beginColumn; i < 9; i++){
        
        int row[9]= {0}; // Array to hold sorted row
        
        for (int j = beginRow; j < 9; j++){
            
            int val = board[i][j];
            
            /* If the corresponding array element for the value is 0, it has not been seen and 
		    the value will be put into the sorted column array */
            if (row[val-1] == 0 && val > 0){
                row[val-1] = val;
            }
            else{
            /* If the value is not 0, the value is a duplicate and the sudoku puzzle
		    is not solved or valid. Tha value of column_worker is 0. */
            
            valid[1] = 0;
		    pthread_exit(0);
            }
        }
    }        
    valid[1] = 1;
    pthread_exit(0);
}

/* Check all 3x3 subgrids*/
void *square_worker(void *params){
	
    int i, j;

    parameters * square_worker = (parameters *) params;
    int beginRow = square_worker->row;
    int beginColumn = square_worker->column;
    
    /* Sort sudoku subgrids. Puts the 3x3 subgrid, as determined by the beginRow and beginCol parameters
      into a sorted order and checks for duplicates. If there are duplicates, this section is not valid and returns 0 for the square_worker;*/
      
    int square[9] = {0}; // 1d array to hold sorted 3x3 square

    for (i = beginRow; i < beginRow + 3; ++i) {
        for (j = beginColumn; j < beginColumn + 3; ++j) {
            
            int val = board[i][j];
            
            /* If the corresponding array element for the value is 0, it has not been seen and 
		    we will input this value to the sorted column array. */
	   
            if (square[val-1] == 0 && val > 0) {
                square[val-1] = val;
            }
            
            /* If the value is not 0, the value is a duplicate and the sudoku puzzle
		    is not solved or valid. The value of square_worker is 0. */
            
            else{
                valid[subgridCt] = 0;
		subgridCt++;
		pthread_exit(0);
            }
        }
    }	
    valid[subgridCt] = 1;
    subgridCt++;
    pthread_exit(0);
}
