#import  "../includes/input_config.hpp"
#import "../includes/namespaces.hpp"
#include <iostream>

#include <termios.h>
#include <unistd.h>

#include <sys/utsname.h> // get devices info
#include <sys/sysctl.h>

#include <vector>
#include <string>



using std::string;
using std::vector;
using std::cout;
using std::flush;

string trim(const string s) {
    size_t first = s.find_first_not_of(" \t\n\r");
    
    if (first == string::npos) return "";
    
    size_t last = s.find_last_not_of(" \t\n\r");

    return s.substr(first, (last - first + 1));
}




int getRawKey() {
    struct termios oldt, newt;
    unsigned char ch; // unsigned makes char diapason 2 times wider and deletes negative numbers (no need)
    tcgetattr(STDIN_FILENO, &oldt); // writing up all old settings of user's terminal
    newt = oldt; // copying
    newt.c_lflag &= ~(ICANON | ECHO); // getting rid of echo because we need to control our input
    
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // setting new options
    
    ssize_t n = read(STDIN_FILENO, &ch, 1); // getting ascii code of user's input into char
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // returning old options
    
    return (n > 0) ? (int)ch : -1; // EOF + piping problems solved
//#endif
}





//string arrow_manipulate(vector<string>& menu, int arrow) {
////    string cleanedInput = trim(currentInput);
//    vector<string> options;
////    string prefix_to_keep = cleanedInput;
//
////    auto it = cmd_dict::tree.find(cleanedInput);
////
////    if (it != cmd_dict::tree.end()) {
////        options = it->second;
////        prefix_to_keep = cleanedInput + (cleanedInput.empty() ? "" : " ");
////    }
////
////
////
////    else {
////        size_t last_space = cleanedInput.find_last_of(' ');
////        string context = "";
////        string partial_word = cleanedInput;
////
////        if (last_space != string::npos) {
////            context = cleanedInput.substr(0, last_space);
////            partial_word = cleanedInput.substr(last_space + 1);
////        }
////
////        auto ctx_it = cmd_dict::tree.find(context);
////        if (ctx_it != cmd_dict::tree.end()) {
////            for (const string& opt : ctx_it->second) {
////                if (opt.find(partial_word) == 0) {
////                    options.push_back(opt);
////                }
////            }
////        }
////        prefix_to_keep = (last_space != string::npos) ? context + " " : "";
////    }
//
//    if (menu.empty()) {
//        cout << "\n[No suggestions]" << flush;
//        std::cout << "\a" << flush;
//        return "[No suggestions]";
//    }
//
//
//
//    int selected = 0; // selected by user index
//
//    while (true) {
//        cout << "\n";
//
//        for (int i = 0; i < menu.size(); i++) {
//            if (i == selected) {
//                cout << " " << ui::RED << ui::DIM << ui::UNDERLINE << menu[i] << ui::RESET << " ";
//            } else {
//                cout << " " << ui::CYAN << menu[i] << ui::RESET << " ";
//            }
//        }
//
//        cout << "\r\033[A" << flush; // getting back on the line of input
//
////        int k = getRawKey(); // getting input key
////        if (k == 9) {
////            selected = (selected + 1) % options.size();
////        }
//
//
//
////        else if (k == 27 || k == 32539) {
////            int next1 = getRawKey();
////            int next2 = getRawKey();
////            if (next1 == 91 || next1 == 32603 ) {
////                if (next2 == 65 || next2 == 32577) selected = (selected + 2) % options.size();
////                if (next2 == 66 || next2 == 32578) selected = (selected - 2) % options.size();
////                if (next2 == 67 || next2 == 32579) selected = (selected + 1) % options.size();
////                if (next2 == 68 || next2 == 32580) selected = (selected - 1) % options.size();
////            }
////        }
//
//
//        if (arrow == 65 || arrow == 32577) selected = (selected - 1) % options.size();
//        if (arrow == 66 || arrow == 32578) selected = (selected + 1) % options.size();
//        if (arrow == 67 || arrow == 32579) selected = (selected + 1) % options.size();
//        if (arrow == 68 || arrow == 32580) selected = (selected - 1) % options.size();
//
//        else if (k == 10 || k == 13) { // ENTER
//            cout << "\n\033[K" << flush; // clearing the line under main line
//            cout << "\033[A" << flush;    // getting back on the main line
//            return menu[selected];
////            currentInput = trim(currentInput) +  " " + options[selected];
//            break;
//        }
//
//
//        else continue; // anything else  ///// changed
//
//    }
//}







string read_line(vector<string> menu) {
    string userInputBuffer = "";
//    string path = system_func::get_path_manual();

//    cout << prompt << flush;
    int optionIndex = 0;
    
    while (true) {
        int k = getRawKey();
        
        
        if (k == 27 || k == 32539) {
            int next1 = getRawKey();
            int next2 = getRawKey();
            
            if (next1 == 91 || next1 == 32603 ) {
                if (next2 == 65 || next2 == 32577) {
//                    cout << "[UP ARROW]\n";
                    cout << "\033[K" << flush; // clearing the line
                    optionIndex = (optionIndex - 1) % menu.size();
                    userInputBuffer = menu[optionIndex];
                }
                
                if (next2 == 66 || next2 == 32578) {
//                    cout << "[DOWN ARROW]\n";
                    cout << "\033[K" << flush; // clearing the line
                    optionIndex = (optionIndex + 1) % menu.size();
                    userInputBuffer = menu[optionIndex];
                }
                
                if (next2 == 67 || next2 == 32579) {
//                    cout << "[RIGHT ARROW]\n";
                    cout << "\033[K" << flush; // clearing the line
                    optionIndex = (optionIndex + 1) % menu.size();
                    userInputBuffer = menu[optionIndex];
                }
                
                if (next2 == 68 || next2 == 32580) {
//                    cout << "[LEFT ARROW]\n";
                    cout << "\033[K" << flush; // clearing the line
                    optionIndex = (optionIndex - 1) % menu.size();
                    userInputBuffer = menu[optionIndex];
                }
                
                cout << menu[optionIndex] << "?" << flush << "\n" << flush;
//                userInputBuffer = arrow_manipulate(menu, next2);
            }
        }
        else if (k == 9 || k == 32521) { // TAB
            cout << "[TAB]\n";
            optionIndex = (optionIndex - 1) % menu.size();
            cout << menu[optionIndex] << "?" << flush << "\n" << flush;
//            userInputBuffer = arrow_manipulate(menu, 66);
//            cout << "\n" << prompt << userInputBuffer << flush;
        }
    
        else if (k == 10 || k == 13 || k == 32522) {
//            cout << "\n";
//            enterPressed(userInputBuffer);
            cout << "\033[K" << flush; // clearing the line
            return userInputBuffer;
            
        }
        else if (k == 127 || k == 8 || k == 32639) {
            if (!userInputBuffer.empty()) {
                userInputBuffer.pop_back();
                cout << "\b \b" << flush;
            }
        }
        else if (k >= 32 && k <= 126){
            userInputBuffer += (char)k;
            cout << (char)k << flush;
//            cout << k;
        }
    }
}









//int checkForSpecials(string userInput) {
//    if (userInput == "q!") return 1;
//    if (userInput == "a!") return 2;
//    return 0;
//}



//void youEnteredFunc(string userInput) {
//    if (userInput == "") return;
//    cout << "\nYou've entered: " << userInput << "\n";
//}


