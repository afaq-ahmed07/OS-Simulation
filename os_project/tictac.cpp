#include <iostream>
#include <vector>
#include <sys/shm.h>
using namespace std;
const int no_of_processes = 12;
struct cf
{
   bool close_flag[no_of_processes]={false};
};

const int BOARD_SIZE = 3;

enum class CellState {
    Empty,
    X,
    O
};

class TicTacToe {
private:
    vector<vector<CellState>> board;

public:
    TicTacToe() {
        board.resize(BOARD_SIZE, vector<CellState>(BOARD_SIZE, CellState::Empty));
    }

    void printBoard() {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                switch (board[i][j]) {
                    case CellState::X:
                        cout << "X ";
                        break;
                    case CellState::O:
                        cout << "O ";
                        break;
                    case CellState::Empty:
                        cout << ". ";
                        break;
                }
            }
            cout << endl;
        }
        cout << endl;
    }

    bool makeMove(int row, int col, CellState player) {
        if (board[row][col] == CellState::Empty) {
            board[row][col] = player;
            return true;
        }
        return false;
    }

    bool isBoardFull() {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == CellState::Empty) {
                    return false;
                }
            }
        }
        return true;
    }

    bool hasWon(CellState player) {
        // Check rows
        for (int i = 0; i < BOARD_SIZE; i++) {
            bool hasWon = true;
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] != player) {
                    hasWon = false;
                    break;
                }
            }
            if (hasWon) {
                return true;
            }
        }

        // Check columns
        for (int j = 0; j < BOARD_SIZE; j++) {
            bool hasWon = true;
            for (int i = 0; i < BOARD_SIZE; i++) {
                if (board[i][j] != player) {
                    hasWon = false;
                    break;
                }
            }
            if (hasWon) {
                return true;
            }
        }

        // Check diagonals
        bool hasWon = true;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i][i] != player) {
                hasWon = false;
                break;
            }
        }
        if (hasWon) {
            return true;
        }

        hasWon = true;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i][BOARD_SIZE - 1 - i] != player) {
                hasWon = false;
                break;
            }
        }
        if (hasWon) {
            return true;
        }

        return false;
    }
};
int main() {
  system("echo \"\\033]0;tictactoe\\007\"");//naming the terminal
TicTacToe game;
CellState currentPlayer = CellState::X;
cout << "Welcome to Tic Tac Toe!" << endl;
cout << "Player 1: X" << endl << "Player 2: O" << endl;

while (!game.isBoardFull()) {
    game.printBoard();

    int row, col;
    cout << "Player " << ((currentPlayer == CellState::X) ? "1" : "2") << ", enter your move (row col): ";
    cin >> row >> col;

    if (game.makeMove(row, col, currentPlayer)) {
        if (game.hasWon(currentPlayer)) {
            cout << "Player " << ((currentPlayer == CellState::X) ? "1" : "2") << " wins!" << endl;
            return 0;
        }
        currentPlayer = (currentPlayer == CellState::X) ? CellState::O : CellState::X;
    }
    else {
        cout << "Invalid move. Try again." << endl;
    }
}

cout << "It's a tie!" << endl;
 cout << "Press any key to exit:"<<endl;
       cin.ignore();
	while(!cin.get()){}
	int shmid=shmget(1234,sizeof(cf),0666|IPC_CREAT);
			cf*obj =(cf*)shmat(shmid,NULL,0);
			obj->close_flag[10]=true;
	 system("wmctrl -c :ACTIVE:"); 
return 0;
}
