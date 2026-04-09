#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 50

int matrix[MAX][MAX];
int n;
int magic_constant;

int row_status[MAX];
int col_status[MAX];
int diag_status[2];
int unique_status;

int score = 0;
int max_score;

pthread_mutex_t lock;

/* Update score safely */
void update_score(int valid)
{
    pthread_mutex_lock(&lock);

    if(valid)
        score++;
    else
    {
        score--;
        if(score < 0)
            score = 0;
    }
    
    pthread_mutex_unlock(&lock);
}

/* Row validation thread */
void *check_rows(void *arg)
{
    for(int i = 0; i < n; i++)
    {
        int sum = 0;

        for(int j = 0; j < n; j++)
            sum += matrix[i][j];
        
        sleep(1);   // artificial delay

        if(sum == magic_constant)
        {
            row_status[i] = 1;
            update_score(1);
        }
        else
        {
            row_status[i] = 0;
            update_score(0);
        }
    }

    printf("Thread ID-%lu: Row checks completed.\n", pthread_self());
    pthread_exit(NULL);
}

/* Column validation thread */

void *check_columns(void *arg)
{
    for(int i = 0; i < n; i++)
    {
        int sum = 0;

        for(int j = 0; j < n; j++)
            sum += matrix[j][i];

        sleep(1);

        if(sum == magic_constant)
        {
            col_status[i] = 1;
            update_score(1);
        }
        else{
            col_status[i] = 0;
            update_score(0);
        }
    }

    primtf("Thread ID-%lu: Column checks completed.\n", pthread_self());
    pthread_exit(NULL);
}