#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define X -1
#define O -2
#define BOARD_SIZE 3

int valid(int argc, char **argv)
{
    if (argc != 2)
        return 0;
    if (atoi(argv[1]) < 1)
        return 0;
    if (strlen(argv[1]) != BOARD_SIZE * BOARD_SIZE)
        return 0;
    int counter[10] = {0};
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        if (argv[1][i] < '1' || argv[1][i] > '9')
            return 0;
        counter[argv[1][i] - '0']++;
        if (counter[argv[1][i] - '0'] != 1)
            return 0;
    }
    return 1;
}

void print(int board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == X)
                printf("X ");
            else if (board[i][j] == O)
                printf("O ");
            else
                printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int makeAplay(int board[BOARD_SIZE][BOARD_SIZE], int move, int sign)
{
    switch (move)
    {
    case 1:
        if (board[0][0] != X && board[0][0] != O)
        {
            board[0][0] = sign;
            return 0;
        }
        else
        {
            printf("Invalid - Already Occupied\n");
            return 1;
        }
        break;
    case 2:
        if (board[0][1] != X && board[0][1] != O)
        {
            board[0][1] = sign;
            return 0;
        }
        else
        {
            printf("Invalid - Already Occupied\n");
            return 1;
        }
        break;
    case 3:
        if (board[0][2] != X && board[0][2] != O)
        {
            board[0][2] = sign;
            return 0;
        }
        else
        {
            printf("Invalid - Already Occupied\n");
            return 1;
        }
        break;
    case 4:
        if (board[1][0] != X && board[1][0] != O)
        {
            board[1][0] = sign;
            return 0;
        }
        else
        {
            printf("Invalid - Already Occupied\n");
            return 1;
        }
        break;
    case 5:
        if (board[1][1] != X && board[1][1] != O)
        {
            board[1][1] = sign;
            return 0;
        }
        else
        {
            printf("Invalid - Already Occupied\n");
            return 1;
        }
        break;
    case 6:
        if (board[1][2] != X && board[1][2] != O)
        {
            board[1][2] = sign;
            return 0;
        }
        else
        {
            printf("Invalid - Already Occupied\n");
            return 1;
        }
        break;
    case 7:
        if (board[2][0] != X && board[2][0] != O)
        {
            board[2][0] = sign;
            return 0;
        }
        else
        {
            printf("Invalid - Already Occupied\n");
            return 1;
        }
        break;
    case 8:
        if (board[2][1] != X && board[2][1] != O)
        {
            board[2][1] = sign;
            return 0;
        }
        else
        {
            printf("Invalid - Already Occupied\n");
            return 1;
        }
        break;
    case 9:
        if (board[2][2] != X && board[2][2] != O)
        {
            board[2][2] = sign;
            return 0;
        }
        else
        {
            printf("Invalid - Already Occupied\n");
            return 1;
        }
        break;
    default:
        printf("Invalid move!\n");
        break;
    }
}

int over(int board[BOARD_SIZE][BOARD_SIZE])
{
    int isBoardFull = 1;
    int countXMainDiagonal = 0;
    int countXSecondaryDiagonal = 0;
    int countOMainDiagonal = 0;
    int countOSecondaryDiagonal = 0;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int countXHorizontal = 0;
        int countXVertical = 0;
        int countOHorizontal = 0;
        int countOVertical = 0;

        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] != X && board[i][j] != O)
			{
				isBoardFull = 0;
			}

            if (X == board[i][j])
                countXHorizontal++;
            if (X == board[j][i])
                countXVertical++;
            if (O == board[i][j])
                countOHorizontal++;
            if (O == board[j][i])
                countOVertical++;
        }
        if (countOHorizontal == BOARD_SIZE || countOVertical == BOARD_SIZE)
            return O;
        else if (countXHorizontal == BOARD_SIZE || countXVertical == BOARD_SIZE)
            return X;

        if (X == board[i][i])
            countXMainDiagonal++;
        if (X == board[i][BOARD_SIZE - 1 - i])
            countXSecondaryDiagonal++;
        if (O == board[i][i])
            countOMainDiagonal++;
        if (O == board[i][BOARD_SIZE - 1 - i])
            countOSecondaryDiagonal++;
    }

    if (countOMainDiagonal == BOARD_SIZE || countOSecondaryDiagonal == BOARD_SIZE)
        return O;
    else if (countXMainDiagonal == BOARD_SIZE || countXSecondaryDiagonal == BOARD_SIZE)
        return X;
    return isBoardFull;
}

int move(char *argv, int board[BOARD_SIZE][BOARD_SIZE], int *turn)
{
    while (makeAplay(board, argv[*turn] - '0', X) == 1)
        ++*turn;
    printf("Machine Turn: \n");
    print(board);
    
    if (over(board) == O)
        return O;
    if (over(board) == X)
        return X;
    if (over(board) == 1)
        return 1;
        

    int move = 0;
    do
    {
        printf("Enter your move: ");
        fflush(stdout);
        scanf("%d", &move);
    } while (makeAplay(board, move, O) != 0);
    printf("User Turn: \n");
    print(board);

    if (over(board) == O)
        return O;
    if (over(board) == X)
        return X;
    if (over(board) == 1)
        return 1;

	

    return 0;
}

int main(int argc, char **argv)
{
    int lastState = 0;
    int board[BOARD_SIZE][BOARD_SIZE] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    if (!(valid(argc, argv)))
    {
        printf("Error\n");
        return 1;
    }
    int position = 0;
    print(board);
    while (lastState == 0)
    {
        lastState = move(argv[1], board, &position);
    }

    if (lastState == X)
        printf("I won\n");
    else if (lastState == O)
        printf("I lost\n");
    else if (lastState == 1)
        printf("DRAW\n");

    return 0;
}