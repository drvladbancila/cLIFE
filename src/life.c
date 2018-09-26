#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// dimensions of the grid
#define N 30
#define M 100

#define CLEAR_STRING "clear"

// versioning
#define MAJOR 1
#define MINOR 0

// status of a cell
#define ALIVE 1
#define DEAD 0

// colors for output
#define GREEN "\x1B[32m"
#define NORMAL "\x1B[0m"
#define RED "\x1B[31m"

// a grid is composed of N x M cells of this type
typedef struct {
    int state;      // state is either DEAD or ALIVE
    int genAlive;   // the number of generations that cell has stayed alive
} cell_t;

// this function counts the alive neighbours of every cell in the grid
// and returns this an output
int scanner(cell_t world[][M], int x, int y)
{
    int adjElements = 0;

    for (int dx = (x > 0 ? -1 : 0); dx <= (x < (N - 1) ? 1 : 0); dx++) {
        for (int dy = (y > 0 ? -1 : 0); dy <= (y < (M - 1) ? 1: 0); dy++) {
            if (dx != 0 || dy != 0) {   // this checks that the scanner doesn't count the cell itself as being its neighbour
                if (world[x + dx][y + dy].state == ALIVE) {
                    adjElements++;
                }
            }
        }
    }

    return adjElements;
}

// this function applies the rules of the game:
// if a cell (dead or alive) has two or three cells alive as its neighbours, it becomes alive
// if a living cell has two neighbours, it stays alive
void applyRules(cell_t oldGen[][M], cell_t newGen[][M])
{
    int adjLives;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            // counts the neighbours
            adjLives = scanner(oldGen, i, j);

            // three living neighbours
            if (adjLives == 3) {
                newGen[i][j].state = ALIVE;
                newGen[i][j].genAlive = ++oldGen[i][j].genAlive;
            }

            // two living neighbours
            if (adjLives == 2 && oldGen[i][j].state == ALIVE) {
                newGen[i][j].state = ALIVE;
                newGen[i][j].genAlive = ++oldGen[i][j].genAlive;
            }
        }
    }
}

// this function resets every cell
void clearWorld(cell_t world[][M])
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            world[i][j].state = DEAD;   // every cell is dead now
            world[i][j].genAlive = 0;   // every cell has never lived now
        }
    }
}

// this function copies the temporary grid (newGen) to the other grid (oldGen)
// in this way the other grid can be re-evaluated by the function "applyRules"
// it also counts the actual population and returns it
unsigned long int copyGen(cell_t oldGen[][M], cell_t newGen[][M])
{
    unsigned long int population = 0;

    clearWorld(oldGen);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (newGen[i][j].state == ALIVE) {
                oldGen[i][j].state = ALIVE; // it copies the living cells
                oldGen[i][j].genAlive = newGen[i][j].genAlive; // and saves the generations that cell has lived
                population++;
            }
        }
    }
    clearWorld(newGen);

    return population;
}

// this function prints the grid, the generation and the population
void outputWorld(cell_t world[][M], unsigned long int generation, unsigned long int population)
{
    printf("Generation:\t%lu\tPopulation:\t%lu\n",
    generation, population);

    // stampa a schermo la mappa
    for (int i = 0; i < N; i++) {
        printf("|");
        for (int j = 0; j < M; j++) {
            if (world[i][j].state == ALIVE) {
                if (world[i][j].genAlive > 4) { // if a cell has survived more than 4 generations...
                    printf("%s*%s", RED, NORMAL); // it gets promoted as a stable cell (becomes red)
                } else {
                    printf("%s*%s", GREEN, NORMAL); // otherwise it's just a boring green cell
                }
            } else {
                printf(" ");    // every dead cell is just a space
            }
        }
        printf("|\n");
    }
}

// usage function
void usage(void) 
{
    printf("cLIFE v%d.%d\n\n"
        "A simulator of John Conway's popular game written in C.\n"
        "Developed by %sdrvladbancila%s: "
        "https://github.com/drvladbancila\n\n",
        MAJOR, MINOR, GREEN, NORMAL);

    printf("%sUsage:%s\n\tcLIFE  [map file]\n"
        "\tOR cLIFE <flag>\n\n"
        "%sFLAGS:%s\n"
        "\t-h, --help\tShows this help message.\n"
        "\t-r, --random\tGenerates a random map and starts the game.\n",
        GREEN, NORMAL, RED, NORMAL);
}

// this function generates a random map for the lazy user who is not
// capable of creating one
unsigned long int randomMap(cell_t world[][M]) 
{

    unsigned long int population = 0;
    srand(time(NULL));
    
    // randomises the number of living cells to create
    int numOfCreatures = rand() % (N * M) + 1;
    int i, j;

    while (numOfCreatures > 0) {
        // randomises the coordinates of a cell 
        i = rand() % N + 1;
        j = rand() % M + 1;
        world[i][j].state = ALIVE;
        world[i][j].genAlive = 1;
        population++;
        numOfCreatures--;
    }
    long int genToWatch;

    return population;
}

int main(int argc, char *argv[])
{
    FILE *lifeFile;

    cell_t thisGen[N][M];
    cell_t nextGen[N][M];

    // this string is used to read the file line by line
    char str[M + 2];

    char strResize[20];
    int dimStr;

    int i = 0, j = 0;
    unsigned long int generation = 1;
    unsigned long int population = 0;

    // if there are no enough arguments
    if (argc < 2) {
        usage();
        return(EXIT_SUCCESS);
    }

    // reset everything
    clearWorld(thisGen);
    clearWorld(nextGen);

    if (!strcmp(argv[1], "-r") || 
        !strcmp(argv[1], "--random")) {
            // random mode
            population = randomMap(thisGen);

    } else if (!strcmp(argv[1], "-h") || 
        !strcmp(argv[1], "--help")) {
            // help
            usage();
            return EXIT_SUCCESS;

    } else {
        // here the program expects a file
        lifeFile = fopen(argv[1], "r");

        if (lifeFile == NULL) {
            fprintf(stderr, "[%s*%s] File not found (%s)\n",
            RED, NORMAL, argv[1]);
            return EXIT_FAILURE;
        }

        // reads the file until it ends or there is no more space in the grid
        while (!feof(lifeFile) && i < N) {
            fgets(str, M + 2, lifeFile);

            dimStr = strlen(str);
            for (int j = 0; j < dimStr; j++) {
                if (str[j] == '1') {
                    thisGen[i][j].state = ALIVE;
                    thisGen[i][j].genAlive = 1;
                    population++;
                }
            }

            i++;
        }

        fclose(lifeFile);
    }

    // resizes the screen
    sprintf(strResize, "resize -s %d %d", N + 2, M + 2);
    system(strResize);
    
    // infinite for loop
    for (;;) {   
        system(CLEAR_STRING); // clears the screen
        outputWorld(thisGen, generation, population);   // prints the grid (after input, the 1st time)
        applyRules(thisGen, nextGen);   // decides who lives and who dies (so poetic) in nextGen
        population = copyGen(thisGen, nextGen); // nextGen becomes thisGen
        generation++;
        usleep(100000); // little delay
    }

    return EXIT_SUCCESS;
}