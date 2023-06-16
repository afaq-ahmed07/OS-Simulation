#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include<algorithm>
#include <sys/shm.h>
using namespace std;
const int no_of_processes = 12;
struct cf
{
   bool close_flag[no_of_processes]={false};
};


const vector<string> WORDS = {"APPLE", "BANANA", "ORANGE", "PEAR", "KIWI", "MANGO", "PINEAPPLE", "WATERMELON", "GRAPEFRUIT", "PEACH"};

int main() {
      system("echo \"\\033]0;hangman\\007\"");//naming the terminal
    // Seed the random number generator
    srand(time(NULL));

    // Choose a random word from the list
    string word = WORDS[rand() % WORDS.size()];

    // Set up the game board
    string board(word.size(), '_');

    // Set up variables for tracking the player's guesses and number of wrong guesses
    vector<char> guesses;
    int wrongGuesses = 0;

    // Loop until the player wins or loses
    while (wrongGuesses < 6 && board != word) {
        // Print the game board and the player's guesses
        cout << "Word: " << board << endl;
        cout << "Guesses: ";
        for (char c : guesses) {
            cout << c << " ";
        }
        cout << endl;

        // Get the player's guess
        cout << "Enter a letter: ";
        char guess;
        cin >> guess;

        // Check if the player has already guessed the letter
        if (find(guesses.begin(), guesses.end(), guess) != guesses.end()) {
            cout << "You already guessed that letter. Try again." << endl;
            continue;
        }

        // Add the guess to the list of guesses
        guesses.push_back(guess);

        // Check if the guess is in the word
        if (word.find(guess) != string::npos) {
            // Update the game board
            for (int i = 0; i < word.size(); i++) {
                if (word[i] == guess) {
                    board[i] = guess;
                }
            }
        }
        else {
            // Wrong guess
            cout << "Wrong guess!" << endl;
            wrongGuesses++;
        }
    }

    // Print the final game board and the outcome of the game
    cout << "Word: " << board << endl;
    if (wrongGuesses == 6) {
        cout << "You lost. The word was " << word << "." << endl;
    }
    else {
        cout << "Congratulations, you won!" << endl;
    }
 cout << "Press any key to exit:"<<endl;
       cin.ignore();
	while(!cin.get()){}
	int shmid=shmget(1234,sizeof(cf),0666|IPC_CREAT);
			cf*obj =(cf*)shmat(shmid,NULL,0);
			obj->close_flag[11]=true;
	 system("wmctrl -c :ACTIVE:"); 
    return 0;
}
