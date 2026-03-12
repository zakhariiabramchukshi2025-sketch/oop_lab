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



#include "./engine/classes/includes/source.hpp"
#include "./engine/system_requirements/includes/namespaces.hpp"



using std::cout;
using std::cin;
using std::ws;
using std::string;
using std::vector;
using std::optional;
using std::move;
using std::nullopt;
using std::unique_ptr;


using std::left;
using std::setw;
using std::numeric_limits;
using std::streamsize;
using std::exception;




class Program {
private:
    vector<unique_ptr<Device>> devices;
    
    vector<string> classes_list = { // list for object types
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
    vector<string> functional_tv_menu = {
        "send signal",
        "perform action",
        "change chanell",
        "change provider",
        "META AND NAMEPLATE DATA ACCTIONS", // BETA!!! TODO: create actions
        "identify",
        "Exit"
    };
    
    vector<string> functional_lock_menu = {
        "send signal",
        "perform action",
        "META AND NAMEPLATE DATA ACCTIONS", // BETA!!! TODO: create actions
        "identify",
        "Exit"
    };
    
    vector<string> functional_lights_menu = {
        "send signal",
        "perform action",
        "set brightness",
        "META AND NAMEPLATE DATA ACCTIONS", // BETA!!! TODO: create actions
        "identify",
        "Exit"
    };
    
    
    
    

public:
    explicit Program() = default;
    
    
    
    void start_menu_layout() {
        int i = 0;
        size_t length = start_menu_.size();
        for (auto punkt : start_menu_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            if (i == length) cout << "e. " << punkt << ";\n";
            else cout << ui::ITALIC << ui::BOLD << ui::GREEN << ++i <<  ". " << ui::RESET << punkt << ";\n";
        }
        cout << "\n";
    }
    
    void config_menu_layout() {
        int i = 0;
        size_t length = сonfig_menu_.size();
        for (auto punkt : сonfig_menu_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            if (i == length) cout << "e. " << punkt << ";\n";
            else cout << ui::ITALIC << ui::GREEN << ++i <<  ". " << ui::RESET << punkt << ";\n";
        }
        cout << "\n";
    }
    
};


void run_info(Program& app) {
    cout << "Device info section: ";
//                                               ..... HOP ON HERE .....
}

void run_config(Program& app) {
    app.config_menu_layout();
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
    
    
        while (userChoice != 'e' && userChoice != 'E') {
            app.start_menu_layout();
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
