#include <iostream>
#include <ctime>
#include <sys/shm.h>
using namespace std;
const int no_of_processes = 12;
struct cf
{
   bool close_flag[no_of_processes]={false};
};
int main() {
	  system("echo \"\\033]0;current_time\\007\"");//naming the terminal
    // get the current time
    time_t now = time(nullptr);

    // convert the time to a string format
    char* time_str = ctime(&now);

    // print the time
    cout << "The current time is: " << time_str << endl;

    cout << "Press any key to exit:"<<endl;
	while(!cin.get()){}
	int shmid=shmget(1234,sizeof(cf),0666|IPC_CREAT);
			cf*obj =(cf*)shmat(shmid,NULL,0);
			obj->close_flag[3]=true;
	 system("wmctrl -c :ACTIVE:"); 

    return 0;
}

