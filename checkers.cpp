#include <iostream>
#include <ctime>
using namespace std;

int opponentPieces = 12;
int playerPieces = 12;
char board[8][8]{
    //0    1    2    3    4    5    6    7
   { '@', 'X', '@', 'X', '@', 'X', '@', 'X' }, //0
   { 'X', '@', 'X', '@', 'X', '@', 'X', '@' }, //1
   { '@', 'X', '@', 'X', '@', 'X', '@', 'X' }, //2
   { 'X', ' ', 'X', ' ', 'X', ' ', 'X', ' ' }, //3
   { ' ', 'X', ' ', 'X', ' ', 'X', ' ', 'X' }, //4
   { 'X', 'O', 'X', 'O', 'X', 'O', 'X', 'O' }, //5
   { 'O', 'X', 'O', 'X', 'O', 'X', 'O', 'X' }, //6
   { 'X', 'O', 'X', 'O', 'X', 'O', 'X', 'O' }  //7
};

void displayBoard() {
    cout << "   0     1     2     3     4     5     6     7" << endl;
    cout << " _______________________________________________" << endl;
    for (int i = 0; i < 8; ++i) {
        cout << "|     |     |     |     |     |     |     |     |" << endl;
        for (int j = 0; j < 8; ++j) {
            if ((i + j) % 2 == 0) cout << "|  " << board[i][j] << "  ";
            else cout << "|     ";
        }
        cout << "|    " << i << endl;
        cout << "|_____|_____|_____|_____|_____|_____|_____|_____|" << endl;
    }
}

bool isValidMove(int x, int y, int x1, int y1) {
    if (x1 < 0 || x1 >= 8 || y1 < 0 || y1 >= 8 || board[x1][y1] != ' ') {
        return false;
    }

    if (board[x][y] == 'O' || board[x][y] == '@') {
        return (abs(x1 - x) == 1 && abs(y1 - y) == 1);
    }
    if (board[x][y] == 'Q' || board[x][y] == 'K') {
        int dx, dy;
        if (x1 > x) dx = 1;
        else dx = -1;
        if (y1 > y) dy = 1;
        else dy = -1;
        for (int i = x + dx, j = y + dy; i != x1 && j != y1; i += dx, j += dy) {
            if (board[i][j] != ' ') {
                return false;
            }
        }
        return abs(x1 - x) == abs(y1 - y);
    }
    return false;
}

bool isValidCapture(int x, int y, int x1, int y1, char enemy, char enemyKing, int& mx, int& my) {
    if (x1 < 0 || x1 >= 8 || y1 < 0 || y1 >= 8 || board[x1][y1] != ' ') {
        return false;
    }

    if (board[x][y] == 'O' || board[x][y] == '@') {
        mx = (x + x1) / 2;
        my = (y + y1) / 2;
        return (abs(x1 - x) == 2 && abs(y1 - y) == 2 &&
            (board[mx][my] == enemy || board[mx][my] == enemyKing));
    }
    if (board[x][y] == 'Q' || board[x][y] == 'K') {
        int dx, dy;
        if (x1 > x) dx = 1;
        else dx = -1;
        if (y1 > y) dy = 1;
        else dy = -1;
        mx = -1;
        my = -1;
        bool foundEnemy = false;
        for (int i = x + dx, j = y + dy; i != x1 && j != y1; i += dx, j += dy) {
            if (board[i][j] == enemy || board[i][j] == enemyKing) {
                if (foundEnemy) {
                    return false;
                }
                foundEnemy = true;
                mx = i;
                my = j;
            }
            else if (board[i][j] != ' ') {
                return false;
            }
        }
        return foundEnemy && abs(x1 - x) == abs(y1 - y);
    }
    return false;
}

void playerMove() {
    int x, y, x1, y1;
    while (true) {
        cout << "Enter coordinates of piece 'O' or 'Q'" << endl;
        cin >> x >> y;
        if (x >= 0 && x < 8 && y >= 0 && y < 8 && (board[x][y] == 'O' || board[x][y] == 'Q')) {
            break;
        }
        cout << "Invalid piece selection, try again." << endl;
    }

    while (true) {
        cout << "Enter coordinates of the destination" << endl;
        cin >> x1 >> y1;
        int mx, my;
        if (isValidMove(x, y, x1, y1)) {
            board[x1][y1] = board[x][y];
            board[x][y] = ' ';
            if (x1 == 0 && board[x1][y1] == 'O') {
                board[x1][y1] = 'Q'; // Promote to king
            }
            break;
        }
        else if (isValidCapture(x, y, x1, y1, '@', 'K', mx, my)) {
            board[x1][y1] = board[x][y];
            board[x][y] = ' ';
            if (board[x1][y1] == 'O' && x1 == 0) {
                board[x1][y1] = 'Q'; // Promote to king
            }
            board[mx][my] = ' ';
            opponentPieces--;
            break;
        }
        cout << "Invalid move, try again." << endl;
    }

    if (opponentPieces == 0) {
        cout << "Player 'O' wins!" << endl;
    }
}

void opponentMove() {
    int x, y, x1, y1;
    while (true) {
        cout << "Enter coordinates of piece '@' or 'K'" << endl;
        cin >> x >> y;
        if (x >= 0 && x < 8 && y >= 0 && y < 8 && (board[x][y] == '@' || board[x][y] == 'K')) {
            break;
        }
        cout << "Invalid piece selection, try again." << endl;
    }

    while (true) {
        cout << "Enter coordinates of the destination" << endl;
        cin >> x1 >> y1;
        int mx, my;
        if (isValidMove(x, y, x1, y1)) {
            board[x1][y1] = board[x][y];
            board[x][y] = ' ';
            if (x1 == 7 && board[x1][y1] == '@') {
                board[x1][y1] = 'K'; // Promote to king
            }
            break;
        }
        else if (isValidCapture(x, y, x1, y1, 'O', 'Q', mx, my)) {
            board[x1][y1] = board[x][y];
            board[x][y] = ' ';
            if (board[x1][y1] == '@' && x1 == 7) {
                board[x1][y1] = 'K'; // Promote to king
            }
            board[mx][my] = ' ';
            playerPieces--;
            break;
        }
        cout << "Invalid move, try again." << endl;
    }

    if (playerPieces == 0) {
        cout << "Player '@' wins!" << endl;
    }
}

int main() {
    setlocale(LC_ALL, "RUS");
    srand(time(NULL));

    bool playerTurn = rand() % 2 == 0;

    while (true) {
        displayBoard();
        if (playerTurn) {
            playerMove();
        }
        else {
            opponentMove();
        }
        playerTurn = !playerTurn;
        if (opponentPieces == 0 || playerPieces == 0) {
            break;
        }
    }

    return 0;
}

