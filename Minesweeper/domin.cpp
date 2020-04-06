#include <iostream>
#include <iomanip>
#include <time.h>
#include <stdlib.h>

using namespace std;

char table[100][100];
int counter = 0;
int m, n, k;
int rowSelected, columnSelected;
char mineTable[100][100];
bool alreadyHaveBomb;
bool areYouDead = false;
bool areYouWinner = false;
const int PATCH_LINE = 30;

void creatTableWithMine(int row, int column, int numberOfMine)
{
    int mine = 0, i, j;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            mineTable[i][j] = '0';
        }
    }
    srand(time(NULL));
    while (mine < numberOfMine)
    {
        alreadyHaveBomb = false;
        i = rand() % row;
        j = rand() % column;
        if (mineTable[i][j] == 'X')
        {
            alreadyHaveBomb = true;
        }
        if (alreadyHaveBomb == false)
        {
            mineTable[i][j] = 'X';
            mine++;
            if (mineTable[i+1][j] != 'X') mineTable[i+1][j]++;
            if (mineTable[i-1][j] != 'X') mineTable[i-1][j]++;
            if (mineTable[i][j+1] != 'X') mineTable[i][j+1]++;
            if (mineTable[i][j-1] != 'X') mineTable[i][j-1]++;
            if (mineTable[i+1][j+1] != 'X') mineTable[i+1][j+1]++;
            if (mineTable[i-1][j-1] != 'X') mineTable[i-1][j-1]++;
            if (mineTable[i+1][j-1] != 'X') mineTable[i+1][j-1]++;
            if (mineTable[i-1][j+1] != 'X') mineTable[i-1][j+1]++;
        }
    }
}

void printTable(int row, int column, char arr[100][100])
{
    cout << "    ";
    for (int i = 0; i < column; i++)
    {
        cout << setw(3) << i;
    }
    cout << endl << setw(7);
    for (int i = 0; i < 3 * column - 1; i++)
    {
        cout << "_";
    }
    cout << endl;
    for (int i = 0; i < row; i++)
    {
        cout << setw(3) << i << "|";
        for (int j = 0; j < row; j++)
        {
            cout << setw(3) << arr[i][j];
        }
        cout << endl;
    }
}

void clearScreen()
{
    for (int i = 0; i < PATCH_LINE; i++)
    {
        cout << endl;
    }
}

void reveal(int i, int j)
{
    if (table[i][j] == '*')
    {
        table[i][j] = mineTable[i][j];
        counter++;
        if (mineTable[i][j] == '0' )
        {
            reveal(i+1, j);
            reveal(i-1, j);
            reveal(i, j+1);
            reveal(i, j-1);
            reveal(i-1, j-1);
            reveal(i+1, j+1);
            reveal(i-1, j+1);
            reveal(i+1, j-1);
        }
    }
}

void gameplay(int i, int j, int numberOfMine)
{
    if (mineTable[i][j] != 'X')
    {
        reveal(i, j);
    }
    else
    {
        areYouDead = true;
    }
    if (m*n - counter == numberOfMine)
    {
        areYouWinner = true;
    }
}

void answer(int row, int column)
{
    cout << endl << endl;
    cout << "    ";
    for (int i = 0; i < column; i++)
    {
        cout << setw(3) << i;
    }
    cout << endl << setw(7);
    for (int i = 0; i < 3 * column - 1; i++)
    {
        cout << "_";
    }
    cout << endl;
    for (int i = 0; i < row; i++)
    {
        cout << setw(3) << i << "|";
        for (int j = 0; j < row; j++)
        {
            cout << setw(3) << mineTable[i][j];
        }
        cout << endl;
    }
}

int main()
{
    cout << "This game has x = ? rows and y = ? columns" << endl;
    cin >> m >> n;
    cout << "This game has m = ? mines" << endl;
    cin >> k;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            table[i][j] = '*';
        }
    }
    creatTableWithMine(m, n, k);
    while (areYouDead != true && areYouWinner != true)
    {
        clearScreen();
        printTable(m, n, table);
        cin >> columnSelected >> rowSelected;
        gameplay(rowSelected, columnSelected, k);
        if (areYouDead)
        {
            clearScreen();
            cout << "YOU ARE DEAD!!!";
        }
        if (areYouWinner)
        {
            cout << "YOU WIN!!!";
        }
    }
    if (areYouDead != false) answer(m, n);
}
