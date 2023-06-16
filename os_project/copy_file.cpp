#include <iostream>
#include <cstdlib> // for the system() function
#include <unistd.h> // for getcwd() function
#include <sys/shm.h>
using namespace std;
const int no_of_processes = 12;
struct cf
{
   bool close_flag[no_of_processes]={false};
};

int main() {
	  system("echo \"\\033]0;copy_file\\007\"");//naming the terminal
   string curr_dir, file1, file2;

   // Get the current directory
   char cwd[256];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
      curr_dir = cwd;
   }
   else {
      cout << "Error getting current directory." << endl;
      return 1;
   }

   // Prompt the user for input
   cout << "Enter the source file name: ";
   cin >> file1;
   cout << "Enter the destination file name: ";
   cin >> file2;

         system(("cp " + curr_dir + "/" + file1 + " " + curr_dir + "/" + file2).c_str());
         cout << "File copied successfully." << endl;
	cout << "Press any key to exit:"<<endl;
       cin.ignore();
	while(!cin.get()){}
	int shmid=shmget(1234,sizeof(cf),0666|IPC_CREAT);
			cf*obj =(cf*)shmat(shmid,NULL,0);
			obj->close_flag[6]=true;
	 system("wmctrl -c :ACTIVE:"); 
   return 0;
}

