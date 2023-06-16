#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sys/shm.h>
using namespace std;
const int no_of_processes = 12;
struct cf
{
   bool close_flag[no_of_processes]={false};
};

int main()
{
	  system("echo \"\\033]0;numguess\\007\"");//naming the terminal
    srand(time(NULL)); // initialize random seed based on current time

    const int MAX_NUMBER = 100;
    int secretNumber = rand() % MAX_NUMBER + 1; // generate random number between 1 and MAX_NUMBER

    int guess;
    int numGuesses = 0;
    bool guessedCorrectly = false;

    cout << "Welcome to the number guessing game!" << endl;
    cout << "I'm thinking of a number between 1 and " << MAX_NUMBER << endl;

    while (!guessedCorrectly)
    {
        cout << "Enter your guess: ";
        cin >> guess;

        if (guess == secretNumber)
        {
            guessedCorrectly = true;
            numGuesses++;
            cout << "Congratulations, you guessed the number in " << numGuesses << " guesses!" << endl;
        }
        else if (guess < secretNumber)
        {
            cout << "Too low, try again." << endl;
            numGuesses++;
        }
        else
        {
            cout << "Too high, try again." << endl;
            numGuesses++;
        }
    }
	 cout << "Press any key to exit:"<<endl;
       cin.ignore();
	while(!cin.get()){}
	int shmid=shmget(1234,sizeof(cf),0666|IPC_CREAT);
			cf*obj =(cf*)shmat(shmid,NULL,0);
			obj->close_flag[9]=true;
	 system("wmctrl -c :ACTIVE:"); 
    return 0;
}
