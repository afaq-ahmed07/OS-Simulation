#include <iostream>
#include <fstream>
#include <sys/shm.h>
using namespace std;
const int no_of_processes = 12;
struct cf
{
   bool close_flag[no_of_processes]={false};
};

int main(){
  system("echo \"\\033]0;create_file\\007\"");//naming the terminal
ofstream out;
string filename;
cout << "Enter the name of file you want to create:"<<endl;
cin>>filename;
out.open(filename);

 cout << "Press any key to exit:"<<endl;
       cin.ignore();
	while(!cin.get()){}
	int shmid=shmget(1234,sizeof(cf),0666|IPC_CREAT);
			cf*obj =(cf*)shmat(shmid,NULL,0);
			obj->close_flag[4]=true;
	 system("wmctrl -c :ACTIVE:"); 

    return 0;;
}
