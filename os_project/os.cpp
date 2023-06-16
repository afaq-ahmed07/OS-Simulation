#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ncurses.h>
#include <pthread.h>
#include <queue>
#include <string>
#include <sys/shm.h>
#include <unistd.h>
#include <vector>

using namespace std;
const int no_of_processes = 12;//no of tasks =11
int running_count, waiting_count = 0;
//making a shared memory
struct cf {
  bool close_flag[no_of_processes] = {false};
};

int shmid = shmget(1234, sizeof(cf), 0666 | IPC_CREAT);
cf *obj = (cf *)shmat(shmid, NULL, 0);
//functions prototypes
void *notepad(void *arg);
void *run_calculator(void *arg);
void *current_time(void *arg);
void *create_file(void *arg);
void *move_file(void *arg);
void *copy_file(void *arg);
void *delete_file(void *arg);
void *check_file_info(void *arg);
void *run_numguess(void *arg);
void *run_tictac(void *arg);
void *run_hangman(void *arg);
void check_for_closed_process();
void logo();
void logo_user();

// system resources
int ram, no_of_cores, hard_drive;
float avl_ram;
int avl_hard;
queue<int> rq;
queue<int> temp;
queue<int> waiting;

// start the os
//loading bar
void loadingBar()
{
      
     assume_default_colors(COLOR_BLACK, COLOR_CYAN);// Change background to cyan

    initscr(); // Initialize ncurses screen
    cbreak(); // Disable line buffering
    noecho(); // Don't echo user input
    curs_set(0); // Hide cursor
    start_color(); // Enable color mode
    init_pair(1, COLOR_GREEN, COLOR_CYAN); // Define color pair

    int height = 8;
    int width = 40;
    int x = (COLS - width) / 2; // Center horizontally
    int y = (LINES - height) / 2; // Center vertically

    WINDOW* win = newwin(height, width, y, x); // Create window
    box(win, 0, 0); // Draw border

    mvwprintw(win, 3, 14, "Loading..."); // Print label
    wrefresh(win);

    for (int i = 1; i <= 100; i++)
    {
        int len = (i * (width - 2)) / 100; // Calculate bar length
        wattron(win, COLOR_PAIR(1)); // Set color
        for (int j = 1; j <= len; j++)
        {
            mvwprintw(win, 5, j, " "); // Draw bar
        }
        wattroff(win, COLOR_PAIR(1)); // Reset color
        wrefresh(win);
        usleep(50000); // Delay 500ms
    }

    endwin(); // End ncurses screen
}
//logo
void logo() {
  cout
      << "				\033[1m\033[31m SSSSS   \033[33m IIIII "
         " \033[32m RRRRR   \033[36m IIIII  \033[35m U    U   \033[31m SSSSS\n";
  usleep(150000); // introduce a 500 millisecond delay

  cout << "				\033[1m\033[31mS          \033[33m I   "
          " \033[32m R    R    \033[36m I    \033[35m U    U  \033[31mS     \n";
  usleep(150000);

  cout
      << "				\033[1m\033[31m S         \033[33m I   "
         " \033[32m R    R    \033[36m I    \033[35m U    U   \033[31m S    \n";
  usleep(150000);

  cout
      << "				\033[1m\033[31m   SSS     \033[33m I   "
         " \033[32m RRRRR     \033[36m I    \033[35m U    U     \033[31m SSS\n";
  usleep(150000);

  cout << "				\033[1m\033[31m      S    \033[33m I   "
          " \033[32m R   R     \033[36m I    \033[35m U    U       \033[31m  "
          "S\n";
  usleep(150000);

  cout << "				\033[1m\033[31m      S    \033[33m I   "
          " \033[32m R    R    \033[36m I    \033[35m U    U       \033[31m  "
          "S\n";
  usleep(150000);

  cout << "				\033[1m\033[31mSSSSS     \033[33m "
          "IIIII \033[32m R     R \033[36m IIIII \033[35m   UUUU   \033[31m  "
          "SSSSSS  \n";
  usleep(150000);
}
// specially design logo for user menu
void logo_user() {
  cout << "				    \033[1m\033[31m SSSSS   \033[33m "
          "IIIII  \033[32m RRRRR   \033[36m IIIII  \033[35m U    U   \033[31m "
          "SSSSS\n";
  usleep(150000); // introduce a 500 millisecond delay

  cout << "				    \033[1m\033[31mS          \033[33m "
          "I    \033[32m R    R    \033[36m I    \033[35m U    U  \033[31mS    "
          " \n";
  usleep(150000);

  cout << "				    \033[1m\033[31m S         \033[33m "
          "I    \033[32m R    R    \033[36m I    \033[35m U    U   \033[31m S  "
          "  \n";
  usleep(150000);

  cout << "				    \033[1m\033[31m   SSS     \033[33m "
          "I    \033[32m RRRRR     \033[36m I    \033[35m U    U     \033[31m "
          "SSS\n";
  usleep(150000);

  cout << "				    \033[1m\033[31m      S    \033[33m "
          "I    \033[32m R   R     \033[36m I    \033[35m U    U       "
          "\033[31m  S\n";
  usleep(150000);

  cout << "				    \033[1m\033[31m      S    \033[33m "
          "I    \033[32m R    R    \033[36m I    \033[35m U    U       "
          "\033[31m  S\n";
  usleep(150000);

  cout << "				    \033[1m\033[31mSSSSS     \033[33m "
          "IIIII \033[32m R     R \033[36m IIIII \033[35m   UUUU   \033[31m  "
          "SSSSSS  \n";
  usleep(150000);
}

// menu for user mode
int home() {
  int choice = 0;
  system("clear");
  cout << endl << endl << endl;
  logo_user();
  cout << "                               "
          "\033[1;"
          "36m▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄"
          "\033[0m\n\n";
  cout << "\033[1;36m                               █\033[0m                   "
          " Select an option:                      \033[0m \033[1;36m█\033[0m  "
       << endl;
  cout << "\033[1;36m                               █\033[0m 		"
          "	\033[1;32m1. Notepad\033[0m             \033[1;36m             "
          "█\033[0m "
       << endl;
  cout << "\033[1;36m                               █\033[0m 		"
          "	\033[1;32m2. Calculator\033[0m		\033[1;36m	    "
          "█\033[0m "
       << endl;
  cout << "\033[1;36m			       █\033[0m 		"
          "	\033[1;32m3. Time\033[0m			"
          "\033[1;36m	    █\033[0m "
       << endl;
  cout << "\033[1;36m			       █\033[0m 		"
          "	\033[1;32m4. Create a File\033[0m       \033[1;36m             "
          "█\033[0m "
       << endl;
  cout << "\033[1;36m			       █\033[0m 		"
          "	\033[1;32m5. Move a File\033[0m          \033[1;36m            "
          "█\033[0m "
       << endl;
  cout << "\033[1;36m			       █\033[0m                        "
          "\033[1;32m6. Copy a File\033[0m	         \033[1;36m           "
          "█\033[0m "
       << endl;
  cout << "\033[1;36m		               █\033[0m 		"
          "	\033[1;32m7. Delete a File\033[0m	\033[1;36m	    "
          "█\033[0m "
       << endl;
  cout << "\033[1;36m			       █\033[0m 		"
          "	\033[1;32m8. Check File Info\033[0m      \033[1;36m            "
          "█\033[0m "
       << endl;
  cout << "\033[1;36m			       █\033[0m 		"
          "	\033[1;32m9. Minigames\033[0m		\033[1;36m	    "
          "█\033[0m "
       << endl;
  cout << "\033[1;36m			       █\033[0m    	               "
          " \033[1;32m0. Exit\033[0m		\033[1;36m		    "
          "█\033[0m "
       << endl;
  cout << "\033[1;36m  			       █\033[0m 		: "
          "Press -1 to Switch to Kernal mode :   \033[1;36m    █\033[0m "
       << endl;
  check_for_closed_process();
  cout << "\033[1;36m			       "
          "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\033["
          "0m\n";
  cout << " Option: " << endl;
  cin >> choice;
 cout << "\033[1;31m::OUTPUT::\033[0m" << endl; 

  check_for_closed_process();
  return choice;
}
// minigames menu
int mini() {
  int min_choice = 0;
  cout << "1.Numguess game "<<endl;
  cout << "2.Tic tac toe game "<<endl;
  cout << "3.Hangman game "<<endl;
  cin >> min_choice;
  return min_choice;
}
//setting system resources to user defined values
void input_resources(int ram, int no_of_cores, int hard_drive) {
  // Store the input values in global variables
  ::ram = ram;
  ::no_of_cores = no_of_cores;
  ::hard_drive = hard_drive;
  // Set the available RAM and hard drive space to the input values
  avl_ram = ram;
  avl_hard = hard_drive;
}

// define the process structure
struct Process_info {
  string name;
  int id;
  int hard_usage;
  float ram_usage;
};
//making an array of objects each with thier properties
Process_info process[12];
//giving defualt values to properties
void set_process_res() {
  for (int i = 1; i < no_of_processes; i++) {
    process[i].id = i;
  }
  process[1].name = "Notepad";
  process[2].name = "Calculator";
  process[3].name = "Current_time";
  process[4].name = "Create_file";
  process[5].name = "Move_file";
  process[6].name = "Copy_file";
  process[7].name = "Delete_file";
  process[8].name = "Check_file_info";
  process[9].name = "Number_guess_game";
  process[10].name = "Tic_tac_toe_game";
  process[11].name = "Hangman_game";

  // ram and hard usage
  process[1].ram_usage = 0.1;
  process[1].hard_usage = 2;
  process[2].ram_usage = 0.3;
  process[2].hard_usage = 6;
  process[3].ram_usage = 0.4;
  process[3].hard_usage = 7;
  process[4].ram_usage = 0.1;
  process[4].hard_usage = 8;
  process[5].ram_usage = 0.2;
  process[5].hard_usage = 5;
  process[6].ram_usage = 0.5;
  process[6].hard_usage = 10;
  process[7].ram_usage = 0.2;
  process[7].hard_usage = 5;
  process[8].ram_usage = 0.2;
  process[8].hard_usage = 5;
  process[9].ram_usage = 0.2;
  process[9].hard_usage = 5;
  process[10].ram_usage = 0.3;
  process[10].hard_usage = 7;
  process[11].ram_usage = 0.4;
  process[11].hard_usage = 8;
}
//////resources handle///////
//print the name and id  of all processes
void show_all_process_res() {
  cout << "Process_id|Process_name" << endl;
  for (int i = 1; i < no_of_processes; i++) {
    cout << process[i].id << "          " << process[i].name << endl;
  }
}
//check if resources are avl for a specific process
bool chk_process_res(int p_id) {
  if (avl_ram > process[p_id].ram_usage &&
      avl_hard > process[p_id].hard_usage) {
    return true;
  }
  return false;
}
// print the avl resources of system
void show_avl_res() {
  cout << " :: The Availiable Resources are following ::" << endl;
  cout << " :: Ram Usage = " << avl_ram << endl;
  cout << " :: Hard Usage = " << avl_hard << endl;
}
//subtract the resources from avl when a process goes into running status
void handle_res(int p_id) {
  avl_ram -= process[p_id].ram_usage;
  avl_hard -= process[p_id].hard_usage;
}
//return the resources to system when a process closes
void return_res(int p_id) {
  avl_ram += process[p_id].ram_usage;
  avl_hard += process[p_id].hard_usage;
}

//mode select menu for user
int mode_select() {
  int choice;
  string pass;
  system("clear");
  cout << endl << endl << endl;
  logo();
  // cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
  cout << "\033[1;36m                            "
          "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\033["
          "0m \n\n";
  cout << "                        \033[1;36m    █\033[0m                    "
          "Select an option:                       \033[1;36m█\033[0m  "
       << endl;
  cout << "                       \033[1;36m     █\033[0m  \033[1;32m\t\t\t "
          "1.Kernal\033[0m                          \033[1;36m      █\033[0m  "
       << endl;
  cout
      << "                        \033[1;36m    █\033[0m  \033[1;32m\t\t\t "
         "2.User\033[0m			      \033[1;36m           █\033[0m  "
      << endl;
  cout
      << "                        \033[1;36m    █\033[0m  \033[1;32m\t\t\t "
         "3.Exit\033[0m				\033[1;36m         █\033[0m  "
      << endl;
  cout << "\033[1;36m                            "
          "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\033["
          "0m "
       << endl;
  cout << " Option: " << endl;
  cin >> choice;
  if (choice == 1) {
    cout << " Enter the Password of OS :" << endl;
    cin >> pass;
    while (pass != "123") { // give 3 tries pending
      cout << " Wrong :: Enter again :" << endl;
      cin >> pass;
    }
  }
  return choice;
}
//kernal
//kernal menu
int kernal_menu() {
  int choice;
  system("clear");
  cout << endl << endl << endl;
  logo();
  cout << "\033[1;36m                            "
          "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n\n";
  cout << "\033[1;36m                            █\033[0m                    "
          "Select an option:           \033[1;36m            █\033[0m"
       << endl;
  cout << "\033[1;36m                            █\033[0m 		      "
          "\033[1;32m1.Show Running Processes\033[0m       \033[1;36m          "
          "  █\033[0m"
       << endl;
  cout << "\033[1;36m                            █\033[0m 		      "
          "\033[1;32m2.Modify Processes Properties\033[0m     \033[1;36m       "
          "  █\033[0m"
       << endl;
  cout << "\033[1;36m                            █\033[0m 		      "
          "\033[1;32m3.Close Running Processes\033[0m     \033[1;36m           "
          "  █\033[0m"
       << endl;
  cout << "\033[1;36m                            █\033[0m 		      "
          "\033[1;32m4.Switch to User mode\033[0m	   \033[1;36m          "
          "    █\033[0m"
       << endl;
  cout
      << "\033[1;36m                            "
         "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\033[0m"
      << endl;
  cout << " Choice: " << endl;
  check_for_closed_process();
  cin >> choice;
cout << "\033[1;31m::OUTPUT::\033[0m" << endl; 
  check_for_closed_process();
  return choice;
}
//a function for kernal to modify the resources of specific process
void modify_res(int pid) {
  cout << "Previous resources are ::" << endl;
  cout << "Hard Usage:" << process[pid].hard_usage << endl;
  cout << "Ram Usage:" << process[pid].ram_usage << endl;
  cout << "Enter New resources ::" << endl;
  cout << "Hard Usage:";
  cin >> process[pid].hard_usage;
  cout << "Ram Usage:";
  cin >> process[pid].ram_usage;
  cout << ":: Changes Applied Successfully ::" << endl;
}
//function that remove the process from running queue when it is closed
void remove_from_running(int pid) {
  int save_pop_value;
  bool repeat_flag=true;
  while (!rq.empty()) {
    if (pid == rq.front() &&repeat_flag) {
      rq.pop();
      obj->close_flag[pid] = false; // if closed flag false
      repeat_flag=false;
    } else {
      save_pop_value = rq.front();
      temp.push(save_pop_value);
      rq.pop();
    }
  }
  while (!temp.empty()) {
    rq.push(temp.front());
    temp.pop();
  }
  running_count--;
}
//function that remove the process from waiting queue when it goes into running
void remove_from_waiting(int pid) {
  int save_pop_value;
  bool repeat_flag=true;
  while (!waiting.empty()) {
    if (pid == waiting.front() &&repeat_flag) {
      waiting.pop();
     repeat_flag=false;
    } else {
      save_pop_value = waiting.front();
      temp.push(save_pop_value);
      waiting.pop();
    }
  }
  while (!temp.empty()) {
    waiting.push(temp.front());
    temp.pop();
  }
  waiting_count--;
}
//print the process id's which are in running or waiting state
void show_running_queue() {
  int save_pop_value, count;
  cout << " Current Running Processes are : " << endl;
  while (!rq.empty()) {
    cout << rq.front() << "->";
    save_pop_value = rq.front();
    temp.push(save_pop_value);
    rq.pop();
  }
  while (!temp.empty()) {
    rq.push(temp.front());
    temp.pop();
  }
  cout << endl;
  cout << " Current Waiting Processes are : " << endl;
  while (!waiting.empty()) {
    cout << waiting.front() << "->";
    save_pop_value = waiting.front();
    temp.push(save_pop_value);
    waiting.pop();
  }
  while (!temp.empty()) {
    waiting.push(temp.front());
    temp.pop();
  }
  cout << endl;
}
//function that apply the sjf algorithim and return the pid with least resources
int sjf_waiting_queue() {
  float min_ram_usage = ram + 1;       // set to maximum possible float value
  int min_hard_usage = hard_drive + 1; // set to maximum possible integer value
  int min_id = -1;
  int index = 0;
  for (int i = 1; i <= waiting_count; i++) {
    index = waiting.front();
    if (process[index].hard_usage < min_hard_usage &&
        process[index].ram_usage < min_ram_usage &&
        avl_ram > process[index].ram_usage &&
        avl_hard >
            process[index].hard_usage) { // find process with the minimum hard
                                         // usage and minimum RAM usage
      min_hard_usage = process[index].hard_usage;
      min_ram_usage = process[index].ram_usage;
      min_id = process[index].id;
    }
    waiting.pop();
    waiting.push(index);
  }
  return min_id;
}
//function that switches the process from waiting to running
void waiting_to_running() {
  pthread_t thread_id;
  int pid = sjf_waiting_queue();
  remove_from_waiting(pid);
  switch (pid) {
  case 1:
    // run notepad
    pthread_create(&thread_id, NULL, notepad, (void *)&pid);
    // code to run notepad here...
    rq.push(pid);
    running_count++;
    break;
  case 2:
    // run calculator
    pthread_create(&thread_id, NULL, run_calculator, (void *)&pid);
    // code to run calculator here...
    rq.push(pid);
    running_count++;
    break;
  case 3:
    // run current_time
    pthread_create(&thread_id, NULL, current_time, (void *)&pid);
    // code to get current time here...
    rq.push(pid);
    running_count++;
    break;
  case 4:
    // run create_file
    pthread_create(&thread_id, NULL, create_file, (void *)&pid);
    // code to create a file here...
    rq.push(pid);
    running_count++;
    break;
  case 5:
    // run move_file
    pthread_create(&thread_id, NULL, move_file, (void *)&pid);
    // code to move a file here...
    rq.push(pid);
    running_count++;
    break;
  case 6:
    // run copy_file
    pthread_create(&thread_id, NULL, copy_file, (void *)&pid);
    // code to copy a file here...
    rq.push(pid);
    running_count++;
    break;
  case 7:
    // run delete_file
    pthread_create(&thread_id, NULL, delete_file, (void *)&pid);
    // code to delete a file here...
    rq.push(pid);
    running_count++;
    break;
  case 8:
    // run check_file_info
    pthread_create(&thread_id, NULL, check_file_info, (void *)&pid);
    // code to get file information here...
    rq.push(pid);
    running_count++;
    break;
  case 9:
    // run num_guess_game
    pthread_create(&thread_id, NULL, run_numguess, (void *)&pid);
    // code to play number guessing game here...
    rq.push(pid);
    running_count++;
    break;
  case 10:
    // run tic_tac_toe_game
    pthread_create(&thread_id, NULL, run_tictac, (void *)&pid);
    // code to play tic tac toe game here...
    rq.push(pid);
    running_count++;
    break;
  case 11:
    // run hangman_game
    pthread_create(&thread_id, NULL, run_hangman, (void *)&pid);
    // code to play hangman game here...
    rq.push(pid);
    running_count++;
    break;
  default:
    // handle invalid process ID
    cout << "Invalid Process ID." << endl;
    break;
  }
}
//to check if some process is closed(very important function)
void check_for_closed_process() {
  for (int i = 1; i < no_of_processes; i++) {
    if (obj->close_flag[i] == true) {
      return_res(i);
      remove_from_running(i);
      if (!waiting.empty()) {
        waiting_to_running();
      }
    }
  }
}
//a function avl for kernal to kill a running process
void kill_process() {
  show_running_queue();
  cout << endl;
  int pid;
  cout << " Enter the Process_ID to kill the Process : " << endl;
  cin >> pid;
  switch (pid) {
  case 1:
    system("wmctrl -c 'notepad'");
    obj->close_flag[pid] = true;
    cout << "Process killed  Successfully " << endl;
    break;
  case 2:
    system("wmctrl -c 'calculator'");
    obj->close_flag[pid] = true;
    cout << "Process killed  Successfully " << endl;
    break;
  case 3:
    system("wmctrl -c 'current_time'");
    obj->close_flag[pid] = true;
    cout << "Process killed  Successfully " << endl;
    break;
  case 4:
    system("wmctrl -c 'create_file'");
    obj->close_flag[pid] = true;
    cout << "Process killed  Successfully " << endl;
    break;
  case 5:
    system("wmctrl -c 'move_file'");
    obj->close_flag[pid] = true;
    cout << "Process killed  Successfully " << endl;
    break;
  case 6:
    system("wmctrl -c 'copy_file'");
    obj->close_flag[pid] = true;
    cout << "Process killed  Successfully " << endl;
    break;
  case 7:
    system("wmctrl -c 'delete_file'");
    obj->close_flag[pid] = true;
    cout << "Process killed  Successfully " << endl;
    break;
  case 8:
    system("wmctrl -c 'check_file_info'");
    obj->close_flag[pid] = true;
    cout << "Process killed  Successfully " << endl;
    break;
  case 9:
    system("wmctrl -c 'numguess'");
    obj->close_flag[pid] = true;
    cout << "Process killed  Successfully " << endl;
    break;
  case 10:
    system("wmctrl -c 'tictactoe'");
    obj->close_flag[pid] = true;
    cout << "Process killed  Successfully " << endl;
    break;
  case 11:
    system("wmctrl -c 'hangman'");
    obj->close_flag[pid] = true;
    cout << "Process killed  Successfully " << endl;
    break;
  default:
    cout << " Invalid choice " << endl;
    break;
  }
  check_for_closed_process();
}
//--------------------------------------------------------MAIN------------------------------------------------------------------//
int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cout << "Usage: " << argv[0] << " <Ram> <No_of_Cores> <Hard_drive>"
              << std::endl;
    return 1;
  }
  loadingBar();//calling the loading bar
  // Parse the command line arguments into integers
  int ram = std::stoi(argv[1]);
  int no_of_cores = std::stoi(argv[2]);
  int hard_drive = std::stoi(argv[3]);
  // Call the input_resources() function with the parsed values
  input_resources(ram, no_of_cores, hard_drive);
  //intializing the shared memory to default values
  for (int i = 1; i < 12; i++) {
    obj->close_flag[i] = false;
  }
  bool user_mode_flag = false;
  int temp, min_choi;
  set_process_res();//set process resources to default
  int mode = mode_select();
  if (mode == 3) {
    return 0;
  } // exit
  ////////////kernal mode////////////////

  else if (mode == 1) {
  kernal:
    check_for_closed_process();
    int pid;
    int key = -1;
    while (key != 0) {
      key = kernal_menu();
      check_for_closed_process();
      switch (key) {
      case 1:
        show_running_queue();
        sleep(3);
        break;

      case 2:
        show_all_process_res();
        cout << " Enter Process_Id to modify its Properties : " << endl;
        cin >> pid;
        modify_res(pid);
        break;
      case 3:
        kill_process();
        break;
      case 4:
        key = 0;
        user_mode_flag = true;
        break;
      default:
        cout << " Invalid Choice " << endl;
        break;
      }
    }
  }
  if (user_mode_flag == true || mode == 2) {
    /////user mode/////
    check_for_closed_process();
    int choice = home();
    check_for_closed_process();
    pthread_t thread_id;
    while (choice != 0) {
      if (choice == -1) {
        goto kernal;
      } else if (running_count >= no_of_cores) {
        waiting.push(choice);
        waiting_count++;
        cout << ":: Added to waiting queue ::" << endl;
      } else {
        switch (choice) {
        case 1:

          cout << "You chose Notepad." << endl;
          // check resources if avl then open
          if (chk_process_res(choice)) {
            handle_res(choice);
            rq.push(choice);
            running_count++;
            pthread_create(&thread_id, NULL, notepad, (void *)&choice);

          } else {
            cout << " Not enough res " << endl;
            waiting.push(choice);
            waiting_count++;
            cout << ":: Added to waiting queue ::" << endl;
          }
          break;
        case 2:
          cout << "You chose Calculator." << endl;
          if (chk_process_res(choice)) {
            handle_res(choice);
            rq.push(choice);
            running_count++;
            pthread_create(&thread_id, NULL, run_calculator, (void *)&choice);

          } else {
            cout << " Not enough res " << endl;
            waiting.push(choice);
            waiting_count++;
            cout << ":: Added to waiting queue ::" << endl;
          }
          break;
        case 3:
          // check resources if avl then open
          cout << "You chose Time." << endl;
          if (chk_process_res(choice)) {
            handle_res(choice);
            rq.push(choice);
            running_count++;
            pthread_create(&thread_id, NULL, current_time, (void *)&choice);

          } else {
            cout << " Not enough res " << endl;
            waiting.push(choice);
            waiting_count++;
            cout << ":: Added to waiting queue ::" << endl;
          }
          break;
        case 4:
          // check resources if avl then open
          cout << "You chose Create a file." << endl;
          if (chk_process_res(choice)) {
            handle_res(choice);
            rq.push(choice);
            running_count++;

            pthread_create(&thread_id, NULL, create_file, (void *)&choice);

          } else {
            cout << " Not enough res " << endl;
            waiting.push(choice);
            waiting_count++;
            cout << ":: Added to waiting queue ::" << endl;
          }
          break;
        case 5:
          // check resources if avl then open
          cout << "You chose Move a file." << endl;
          if (chk_process_res(choice)) {
            handle_res(choice);
            rq.push(choice);
            running_count++;
            pthread_create(&thread_id, NULL, move_file, (void *)&choice);
          } else {
            cout << " Not enough res " << endl;
            waiting.push(choice);
            waiting_count++;
            cout << ":: Added to waiting queue ::" << endl;
          }
          // move_file();
          break;
        case 6:
          // check resources if avl then open
          cout << "You chose Copy a file." << endl;
          if (chk_process_res(choice)) {
            handle_res(choice);
            rq.push(choice);
            running_count++;
            pthread_create(&thread_id, NULL, copy_file, (void *)&choice);
          } else {
            cout << " Not enough res " << endl;
            waiting.push(choice);
            waiting_count++;
            cout << ":: Added to waiting queue ::" << endl;
          }
          // copy_file();
          break;
        case 7:
          // check resources if avl then open
          cout << "You chose Delete a file." << endl;
          if (chk_process_res(choice)) {
            handle_res(choice);
            rq.push(choice);
            running_count++;
            pthread_create(&thread_id, NULL, delete_file, (void *)&choice);

          } else {
            cout << " Not enough res " << endl;
            waiting.push(choice);
            waiting_count++;
            cout << ":: Added to waiting queue ::" << endl;
          }
          // delete_file();
          break;
        case 8:
          // check resources if avl then open
          cout << "You chose Check file info." << endl;
          if (chk_process_res(choice)) {
            handle_res(choice);
            rq.push(choice);
            running_count++;
            pthread_create(&thread_id, NULL, check_file_info, (void *)&choice);

          } else {
            cout << " Not enough res " << endl;
            waiting.push(choice);
            waiting_count++;
            cout << ":: Added to waiting queue ::" << endl;
          }
          break;
        case 9:
          min_choi = mini();
          switch (min_choi) {
          case 1:
            // check resources if avl then open
            cout << "You chose Guessing Game." << endl;

            if (chk_process_res(choice)) {
              handle_res(choice);
              rq.push(choice);
              running_count++;
              pthread_create(&thread_id, NULL, run_numguess, (void *)&choice);
            } else {
              cout << " Not enough res " << endl;
              waiting.push(choice);
              waiting_count++;
              cout << ":: Added to waiting queue ::" << endl;
            }
            break;
          case 2:
            // check resources if avl then open
            cout << "You chose Tic Tac Toe Game." << endl;
            choice += 1;

            if (chk_process_res(choice)) {
              handle_res(choice);
              rq.push(choice);
              running_count++;
              pthread_create(&thread_id, NULL, run_tictac, (void *)&choice);
            } else {
              cout << " Not enough res " << endl;
              waiting.push(choice);
              waiting_count++;
              cout << ":: Added to waiting queue ::" << endl;
            }

            break;
          case 3:
            // check resources if avl then open
            cout << "You chose Hangman Game." << endl;
            choice += 2;
            if (chk_process_res(choice)) {
              handle_res(choice);
              rq.push(choice);
              running_count++;
              pthread_create(&thread_id, NULL, run_hangman, (void *)&choice);
            } else {
              cout << " Not enough res " << endl;
              waiting.push(choice);
              waiting_count++;
              cout << ":: Added to waiting queue ::" << endl;
            }
            break;
          default:
            cout << "Invalid choice" << endl;
            break;
          }

          break;
        default:
          cout << "Invalid choice" << endl;
          break;
        }
      }
      check_for_closed_process();
      choice = home();
      check_for_closed_process();
    }
  }
exit:
  return 0;
}
//////main end//////

// PROCESSES
void *notepad(void *arg) {
  int choice = *(int *)arg;
  system("gnome-terminal -- /bin/bash -c './np; exec bash'");
  pthread_exit(NULL);
}
void *run_calculator(void *arg) {
  int choice = *(int *)arg;
  system("gnome-terminal -- /bin/bash -c './cal; exec bash'");
  pthread_exit(NULL);
}
void *current_time(void *arg) {
  int choice = *(int *)arg;
  system("gnome-terminal -- /bin/bash -c './ct; exec bash'");
  pthread_exit(NULL);
}
void *create_file(void *arg) {
  int choice = *(int *)arg;
  system("gnome-terminal -- /bin/bash -c './cf; exec bash'");
  pthread_exit(NULL);
}
void *copy_file(void *arg) {
  int choice = *(int *)arg;
  ;
  system("gnome-terminal -- /bin/bash -c './cpyf; exec bash'");
  pthread_exit(NULL);
}
void *move_file(void *arg) {
  int choice = *(int *)arg;
  system("gnome-terminal -- /bin/bash -c './mf; exec bash'");
  pthread_exit(NULL);
}
void *delete_file(void *arg) {
  int choice = *(int *)arg;
  system("gnome-terminal -- /bin/bash -c './df; exec bash'");
  pthread_exit(NULL);
}
void *check_file_info(void *arg) {
  int choice = *(int *)arg;
  system("gnome-terminal -- /bin/bash -c './cfi; exec bash'");
  pthread_exit(NULL);
}
void *run_numguess(void *arg) {
  int choice = *(int *)arg;
  system("gnome-terminal -- /bin/bash -c './guess; exec bash'");
  pthread_exit(NULL);
}
void *run_tictac(void *arg) {
  int choice = *(int *)arg;
  system("gnome-terminal -- /bin/bash -c './tic; exec bash'");
  pthread_exit(NULL);
}
void *run_hangman(void *arg) {
  int choice = *(int *)arg;
  system("gnome-terminal -- /bin/bash -c './hang; exec bash'");
  pthread_exit(NULL);
}
