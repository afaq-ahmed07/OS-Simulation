#include <iostream>
#include <fstream>
#include <string>
#include<sys/shm.h>
using namespace std;
const int no_of_processes = 12;
struct cf
{
   bool close_flag[no_of_processes]={false};
};
int main() {
      system("echo \"\\033]0;notepad\\007\"");//naming the terminal
    string filename, line, input;
    ofstream myfile;
    ifstream readFile;
    int choice;

    do {
        cout << "1. Create new file" << endl;
        cout << "2. Open existing file" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

            if (choice==1){
                cout << "Enter file name: ";
                cin >> filename;
                myfile.open(filename.c_str(), ios::out);
                if (myfile.is_open()) {
                    cout << "File created successfully." << endl;
                    cout << "Start typing your text." << endl;
                    while (getline(cin, line) && line != "exit") {
                        myfile << line << endl;
                    }
                    myfile.close();
                }
                else {
                    cout << "Error in creating file." << endl;
                }
                }
            else if (choice==2){
                cout << "Enter file name: ";
                cin >> filename;
                readFile.open(filename.c_str());
                if (readFile.is_open()) {
                    cout << "File opened successfully." << endl;
                    while (getline(readFile, input)) {
                        cout << input << endl;
                    }
                    readFile.close();
                }
                else {
                    cout << "Error in opening file." << endl;
                }
                }
          	else{
			int shmid=shmget(1234,sizeof(cf),0666|IPC_CREAT);
			cf*obj =(cf*)shmat(shmid,NULL,0);
			obj->close_flag[1]=true;
                cout << "Exiting program." << endl;
                //close_flag[1]=true;
                system("wmctrl -c :ACTIVE:"); 

                }
          
    } while (choice != 3);
    return 0;
}

