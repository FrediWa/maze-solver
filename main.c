#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MIN(a,b) ((a < b) ? a : b)
#define MAX(a,b) ((a > b) ? a : b)
#define ABS(x)   ((x > 0) ? -x : x)

#define MAX_WIDTH  30
#define MAX_HEIGHT 30
#define MAX_Q_SIZE 2000
#define PRINT 1 // Whether or not to print the maze during flooding

typedef struct {
    int front;
    int back;
    int items[MAX_Q_SIZE];
} Queue;

void enQueue(Queue* q, int value)
{
    if (q->back == MAX_Q_SIZE - 1) return;
    
    if (q->front == -1) q->front = 0;
    
    q->back++;
    q->items[q->back] = value;
    
}
void deQueue(Queue* q)
{
    if (q->front == -1) return;

    q->front++;

    if (q->front > q->back)
    {
        q->front = -1;
        q->back = -1;
    }
}
void initQueue(Queue* q)
{
    q->front = -1;
    q->back = -1;
}

typedef struct {
    char values[MAX_HEIGHT * MAX_WIDTH];
    int height;
    int width;
    int start;
} Maze;

void printMaze(Maze* maze)
{
    printf("%s\n", maze->values);
}

void getMaze(const char* path, Maze* maze)
{
    FILE *fp;
    char values[MAX_WIDTH * MAX_HEIGHT];
    char ch;
    int counter = 0;
    int row = 0;

    maze->width = 0;

    fp = fopen(path, "r");

    while (ch != EOF) {
        ch = fgetc(fp);

        if ( ch == '^')
            maze->start = counter;

        // Newline => new row
        if ( ch == '\n'){
            // Width = counter up until first newline, i.e. length of a row
            if ( maze->width == 0) maze->width = counter;
            row++;
        }

        values[counter++] = ch;
    }

    values[counter] = '\0';
    strcpy(maze->values, values);
    maze->height = row;
}

void flood(int pos, Maze* maze, Queue* q)
{
    // Distance to cell above and below any given cell is the width
    const int down  = maze->width;  
    const int up    = -maze->width; 

    const int right = 1;
    const int left  = -1;

    // Return if out of bounds
    if ( (pos < 0) || (pos > strlen(maze->values)) ) return;

    printf("POS: %d CHAR: [%c]\n", pos, maze->values[pos]);
    
    // Add initial position to queue
    enQueue(q, pos);
    int n;
    char counter = 0;
    
    // Queue is empty when both pointers are -1
    while ( q->front != -1 && q->back != -1)
    {
        printf("----------\n");
        n = q->items[q->front];
        deQueue(q);

        printf("%d\n", n);

        // Check if position is in bounds
        if ( (n < 0) || (n > strlen(maze->values)) || (maze->values[n] == '#') || (maze->values[n] == '\n'))
            continue;

        // Continue if position is already set
        if ( (maze->values[n] > ' ') && (maze->values[n] < '^') )
            continue;

        // Set value for position
        maze->values[n] = '0';

        enQueue(q, n + up);
        enQueue(q, n + right);
        enQueue(q, n + down);
        enQueue(q, n + left);

        #if PRINT
        printMaze(maze);
        usleep(10*1000);
        #endif
    }

}
int main()
{
    // Read maze files
    Maze* maze = (Maze*) malloc(sizeof(Maze));
    getMaze("./maze.txt", maze);

    // Init queue for flood
    Queue* q = (Queue*) malloc(sizeof(Queue));
    initQueue(q);


    flood(maze->start, maze, q);
    printMaze(maze);

    return(0);
}