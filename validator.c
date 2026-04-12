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

/* Diognal validation thread */
void *check_diagonals(void *arg)
{
    int sum1 = 0;
    int sum2 = 0;

    for(int i = 0; i < n; i++)
    {
        sum1 += matrix[i][i];
        sum2 += matrix[i][n-i-1];
    }

    sleep(1);

    if(sum1 == magic_constant)
    {
        diag_status[0] = 1;
        update_score(1);
    }
    else
    {
        diag_status[0] = 0;
        update_score(0);
    }

    if(sum2 == magic_constant)
    {
        diag_status[1] = 1;
        update_score(1);
    }
    else
    {
        diag_status[1] = 0;
        update_score(0);
    }

    printf("Thread ID-%lu: Diagonal checks completed.\n", pthread_self());
    pthread_exit(NULL);
}

/* Uniqueness validation thread */
void *check_unique(void *args)
{
    int seen[MAX*MAX+1] = {0};
    int valid = 1;

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            int val = matrix[i][j];

            if(val < 1 || val > n*n || seen[val])
            {
                valid = 0;
                break;
            }

            seen[val] = 1;
        }
    }

    sleep(1);

    unique_status = valid;

    if(valid)
        update_score(1);
    else
        update_score(0);

    printf("Thread ID-%lu: Uniqueness check completed.\n", pthread_self());
    pthread_exit(NULL);
}

/* Print report */
void print_report()
{
    printf("\n--- Magic Square Report ---\n");

    /* Rows */
    int all_valid = 1;
    for(int i = 0; i < n; i++)
    {
        if(row_status[i] == 0)
        {
            printf("Rows: Row %d Invalid\n", i+1);
            all_valid = 0;
        }
    }
    if(all_valid)
        printf("Rows: All valid\n");

    /* Columns */
    all_valid = 1;
    for(int i = 0; i < n; i++)
    {
        if(col_status[i] == 0)
        {
            printf("Cols: Col %d Invalid\n", i+1);
            all_valid = 0;
        }
    }
    if(all_valid)
        printf("Cols: All valid\n");

    /* Diagonals */
    if(diag_status[0] && diag_status[1])
        printf("Diags")
}