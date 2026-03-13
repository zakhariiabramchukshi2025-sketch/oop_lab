#include <iostream>
#include <optional>
#include <algorithm>
#include <ios>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <arpa/inet.h>
#include <iomanip>
#include <limits>
#include <memory>


#include <termios.h>
#include <unistd.h>

#include <sys/utsname.h> // get devices info
#include <sys/sysctl.h>



#include "./engine/classes/includes/source.hpp"
#include "./engine/system_requirements/includes/namespaces.hpp"
#include "./engine/system_requirements/includes/input_config.hpp"



using std::cout;
using std::cin;
using std::ws;
using std::string;
using std::vector;
using std::optional;
using std::move;
using std::nullopt;
using std::unique_ptr;
using std::flush;

using std::left;
using std::setw;
using std::numeric_limits;
using std::streamsize;
using std::exception;




class Program {
private:
    vector<unique_ptr<Device>> devices_;
    
    vector<string> classes_list_ = { // list for object types
        "Device",
        "SmartTV",
        "SmartLock",
        "SmartLights"
    };
    
    
    vector<string> start_menu_ = {
        "info",
        "Functional",
        "Configuration",
        "Create",
        "Delete",
        "Exit"
    };
        
    vector<string> сonfig_menu_ = { // menu: mostly 'bout nameplate/meta - data && IRemote interface
        "Toogle remote connection block",
        "Config Nameplate Data",
        "Config Meta Data",
        "Config Name", // need to overwrite if device vector will be string names
        "Exit"
    };
    
    
    
    
    
 // --- functional for specific objects
    vector<string> functional_tv_menu_ = {
        "send signal",
        "perform action",
        "change chanell",
        "change provider",
        "META AND NAMEPLATE DATA ACCTIONS", // BETA!!! TODO: create actions
        "identify",
        "Exit"
    };
    
    vector<string> functional_lock_menu_ = {
        "send signal",
        "perform action",
        "META AND NAMEPLATE DATA ACCTIONS", // BETA!!! TODO: create actions
        "identify",
        "Exit"
    };
    
    vector<string> functional_lights_menu_ = {
        "send signal",
        "perform action",
        "set brightness",
        "META AND NAMEPLATE DATA ACCTIONS", // BETA!!! TODO: create actions
        "identify",
        "Exit"
    };
    
    
    
    

public:
    explicit Program() = default;
//    explicit Program() = default;  // TODO: admin constructor
    
    
    
    
    [[nodiscard]] string read_input(string menuType) const {
        if (menuType == "config") { return read_line(сonfig_menu_); }
        else if (menuType == "tv") { return read_line(functional_tv_menu_); }
        else if (menuType == "lock") { return read_line(functional_lock_menu_); }
        else if (menuType == "lights") { return read_line(functional_lights_menu_); }
        else {return "Text";} // TODO: get rid of this
    }
    
    
    
    void start_menu_layout() {
        int i = 0;
        size_t length = start_menu_.size();
        for (auto punkt : start_menu_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            if (i == length - 1) cout << ui::DIM << ui::ITALIC << ui::BOLD << ui::RED << "e. " << punkt << ui::RESET << ";\n";
            else cout << ui::ITALIC << ui::BOLD << ui::GREEN << ++i <<  ". " << ui::RESET << punkt << ";\n";
        }
        cout << "\n";
    }
    
    void menu_layout(string menuType) const {
        int i = 0;
        vector<string> menuToPrint;
        
        if (menuType == "config") { menuToPrint = сonfig_menu_; }
        else if (menuType == "tv") { menuToPrint = functional_tv_menu_; }
        else if (menuType == "lock") { menuToPrint = functional_lock_menu_; }
        else if (menuType == "lights") { menuToPrint = functional_lights_menu_; }
        else {return;}
        
        
        size_t length = menuToPrint.size();
        for (auto punkt : menuToPrint) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            if (i == length - 1) cout << ui::DIM << ui::ITALIC << ui::BOLD << ui::RED << "e. " << punkt << ui::RESET << ";\n";
            else cout << ui::ITALIC << ui::GREEN << ++i <<  ". " << ui::RESET << punkt << ";\n";
        }
        cout << "\n";
    }
    
};


void run_info(Program& app) {
    cout << "\nDevice info section: " << flush;
//                                               ..... HOP ON HERE .....
}

void run_config(Program& app) {
    app.menu_layout("config");
    string userChoice;
    while (userChoice[0] != 'e' && userChoice != "exitProtocol") {
        userChoice = app.read_input("config");
//        run_info(app);
        cout << userChoice << flush;
//        if (userChoice == "")
    }
//                                               ..... HOP ON HERE .....
}



//
//void run_functional(Program& app) {
//
//}
//
//void run_info(Program& app) {
//
//}





void run_main(Program& app) {
    char userChoice = 's';
    
//    ui::intro(); // beta
//    system_func::systemAlert("", 1); // beta
    
        
        while (userChoice != 'e' && userChoice != 'E') {
            app.start_menu_layout();
//            userChoice = getRawKey(); // TODO: menu keys navigation
            cin >> userChoice;
            
            

            switch (userChoice) {
                case '1':

                    cout << "\n\n" << ui::BG_BLACK << ui::GREEN << ui::BOLD << ui::ITALIC << "[System] Displaying device menu..." << ui::RESET << "\n\n"; // and
                    // app.show_info();
                    break;
                case '2':
                    cout << "\n\n" << ui::BG_BLACK << ui::GREEN << ui::BOLD << ui::ITALIC << "[System] Displaying functional menu..." << ui::RESET << "\n\n"; // and
                    break;
                case '3':
                    run_config(app);
                    // Nested loop or logic for config could go here
                    break;
                case '4':
                    cout << "\n\n" << ui::BG_BLACK << ui::GREEN << ui::BOLD << ui::ITALIC << "[System] Displaying creating options..." << ui::RESET << "\n\n"; // and
                    break;
                case '5':
                    cout << "\n\n" << ui::BG_BLACK << ui::GREEN << ui::BOLD << ui::ITALIC << "[System] Displaying deleting options..." << ui::RESET << "\n\n"; // and
                    break;
                case '6':
                case 'e':
                case 'E':
                    system_func::visualAlert();
                    cout << "Exiting swan song... Goodbye.\n";
                    userChoice = 'e';
                    break;
                default:
                    cout << "Invalid selection. Try again.\n";
                    break;
            }
        }
    
    
}



int main() {
    
        
    Program app;
    run_main(app);
    
    return 0;
}



//while (true) {
//    int k = getRawKey();
//
//    if (k == 27 || k == 32539) {
//        int next1 = getRawKey();
//        int next2 = getRawKey();
//        if (next1 == 91 || next1 == 32603 ) {
//            if (next2 == 65 || next2 == 32577) cout << "[UP ARROW]\n";
//            if (next2 == 66 || next2 == 32578) cout << "[DOWN ARROW]\n";
//            if (next2 == 67 || next2 == 32579) cout << "[RIGHT ARROW]\n";
//            if (next2 == 68 || next2 == 32580) cout << "[LEFT ARROW]\n";
//        }
//    }
//    else if (k == 9 || k == 32521) { // TAB
//        tabAutocomplite(userInputBuffer);
//        cout << "\n" << prompt << userInputBuffer << flush;
//    }
//
//    else if (k == 10 || k == 13 || k == 32522) {
//        cout << "\n";
//        enterPressed(userInputBuffer);
//        return userInputBuffer;
//    }
//    else if (k == 127 || k == 8 || k == 32639) {
//        if (!userInputBuffer.empty()) {
//            userInputBuffer.pop_back();
//            cout << "\b \b" << flush;
//        }
//    }
//    else if (k >= 32 && k <= 126){
//        userInputBuffer += (char)k;
//        cout << (char)k << flush;
////            cout << k;
//    }
//}
