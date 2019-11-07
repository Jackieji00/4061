#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MATSIZE 5000

int mat1[MATSIZE][MATSIZE];
int mat2[MATSIZE][MATSIZE];

typedef struct argstruct {
    int x_start;
    int x_end;
    int y_start;
    int y_end;
    int **res_mat;
} args_t;

//TODO: Complete this function (to be accessed by a thread) which will
//      perform matrix addition on a portion of a matrix, dictated by
//      the input parameter 'args'
void * partial_matrix_add(void * args)
{
    args_t *c = (args_t*)args;
    for(int i = c->x_start;i<c->x_end;i++){
        for(int j=c->y_start;j<c->y_end;j++){
            c->res_mat[i][j] = mat1[i][j]+mat2[i][j];
        }
    }
    return NULL;
}




int main() {
    pthread_t m_threads[4];
    args_t m_args[4];
    pthread_t s_thread;
    args_t s_args;
    int i, j, k, c;
    clock_t t_multi_start, t_multi_end, t_single_start, t_single_end;
    int ** res_mat_multi = malloc(MATSIZE * sizeof(int*));
    int ** res_mat_single = malloc(MATSIZE * sizeof(int*));
    for(i = 0; i < MATSIZE; i++) {
        res_mat_multi[i] = malloc(MATSIZE * sizeof(int));
        res_mat_single[i] = malloc(MATSIZE * sizeof(int));
    }

    //Populate base matrices with random integers
    //Initialize result matrices with -1
    for(j = 0; j < MATSIZE; j++) {
        for(k = 0; k < MATSIZE; k++) {
            mat1[j][k] = rand() % 1000 + 1;
            mat2[j][k] = rand() % 1000 + 1;
            res_mat_multi[j][k] = -1;
            res_mat_single[j][k] = -1;
        }
    }

    //Measure time for multiple thread addition
    t_multi_start = clock();
    //TODO: Create threads to populate res_mat_multi with the result
    //      of performing matrix addition mat1 + mat2
    for(i = 0; i < 4; i++) {
        m_args[i].x_start = (i%2)*(MATSIZE/2);
        m_args[i].x_end = (i%2)*(MATSIZE/2)+(MATSIZE/2);
        m_args[i].y_start = (i/2)*(MATSIZE/2);
        m_args[i].y_end = (i/2)*(MATSIZE/2)+(MATSIZE/2);
        m_args[i].res_mat = res_mat_multi;
        pthread_create(&m_threads[i], NULL, partial_matrix_add, &m_args[i]);
    }
    for (i = 0; i < 4; i++) {
        pthread_join(m_threads[i], NULL);
    }
    //pthread_exit(NULL);
    //This should go immediately after end of code for multiple thread
    t_multi_end = clock();

    //Measure time for single thread addition
    t_single_start = clock();

    //TODO: Create one thread to populate res_mat_multi with the result
    //      of performing matrix addition mat1 + mat2
    s_args.x_start = 0;
    s_args.x_end = MATSIZE;
    s_args.y_start = 0;
    s_args.y_end = MATSIZE;
    s_args.res_mat = res_mat_single;
    pthread_create(&s_thread, NULL, partial_matrix_add, &s_args);
    pthread_join(s_thread, NULL);
    

    //This should go immediately after end of code for single thread
    t_single_end = clock();


    //Test to ensure that both methods produce the same result
    c = 0;
    for(j = 0; j < MATSIZE; j++) {
        for(k = 0; k < MATSIZE; k++) {
            if(res_mat_multi[j][k] == res_mat_single[j][k] && res_mat_multi[j][k] != -1) {
                c++;
            }
        }
    }
    printf("Verification: %d out of %d entries matched.\n", c, MATSIZE * MATSIZE);

    //Display time for each version
    printf("Time for multiple threads: %f seconds\n", ((double) t_multi_end - t_multi_start)/CLOCKS_PER_SEC);
    printf("Time for single thread: %f seconds\n", ((double) t_single_end - t_single_start)/CLOCKS_PER_SEC);
    return 0;
}

