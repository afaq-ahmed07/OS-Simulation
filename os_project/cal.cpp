#include <iostream>
#include <sys/shm.h>
#include <cstdlib>
using namespace std;


const int no_of_processes = 12;
struct cf
{
   bool close_flag[no_of_processes]={false};
};
int main(){
    system("echo \"\\033]0;calculator\\007\"");//naming the terminal
    cout << "Calculator" << endl;
    float num1, num2, result;
    char op;
    cout << "Enter first number: ";
    cin >> num1;
    cout << "Enter operator (+,-,*,/): ";
    cin >> op;
    cout << "Enter second number: ";
    cin >> num2;
    switch (op) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 != 0) {
                result = num1 / num2;
            } else {
                cout << "Error: division by zero" << endl;
            }
            break;
        default:
            cout << "Error: invalid operator" << endl;
            
    }
    cout << "Result: " << result << endl;
    cout << "Press any key to exit:"<<endl;
    cin.ignore();
	while(!cin.get()){}
	int shmid=shmget(1234,sizeof(cf),0666|IPC_CREAT);
			cf*obj =(cf*)shmat(shmid,NULL,0);
			obj->close_flag[2]=true;
	 system("wmctrl -c :ACTIVE:"); 
    return 0;
}
