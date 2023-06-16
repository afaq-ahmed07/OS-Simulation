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
     system("echo \"\\033]0;delete_file\\007\"");//naming the terminal
   string curr_dir, filename;

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
   cin >> filename;
	//Deletion
         system(("rm " + curr_dir + "/" + filename).c_str());
         cout << "File deleted successfully." << endl;
	cout << "Press any key to exit "<<endl;
	  cin.ignore();
	while(!cin.get()){}
	int shmid=shmget(1234,sizeof(cf),0666|IPC_CREAT);
			cf*obj =(cf*)shmat(shmid,NULL,0);
			obj->close_flag[7]=true;
	 system("wmctrl -c :ACTIVE:");
   return 0;
}

