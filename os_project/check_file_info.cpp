#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include<sys/shm.h>
using namespace std;
const int no_of_processes = 12;
struct cf
{
   bool close_flag[no_of_processes]={false};
};


int main() {
      system("echo \"\\033]0;check_file_info\\007\"");//naming the terminal
    struct stat file_stat;
    char file_name[100];

    // Prompt the user to enter the file name
    cout << "Enter the file name: ";
    cin >> file_name;

    // Get the file information
    if (stat(file_name, &file_stat) == -1) {
        cout << "Error: File not found." <<endl;
        return 1;
    }

    // Print the file information
    cout << "File name: " << file_name << endl;
    cout << "Size: " << file_stat.st_size << " bytes" << endl;
    cout << "Last modified: " << ctime(&file_stat.st_mtime) <<endl;
 cout << "Press any key to exit:"<<endl;
       cin.ignore();
	while(!cin.get()){}
	int shmid=shmget(1234,sizeof(cf),0666|IPC_CREAT);
			cf*obj =(cf*)shmat(shmid,NULL,0);
			obj->close_flag[8]=true;
	 system("wmctrl -c :ACTIVE:"); 
    return 0;
}

