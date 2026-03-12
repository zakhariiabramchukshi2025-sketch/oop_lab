#include <iostream>
#include <optional>
#include <algorithm>
#include <ios>
#include <string>
#include <vector>
#include <chrono>
#include <arpa/inet.h>
#include <iomanip>
#include <limits>
#include <memory>

using std::cout;
using std::cin;
using std::ws;
//using std::endl; // no need (not anymore)
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



class MetaData {
 private:
    string firmware_version_{"v1.0.0"};
    
    int error_count_{0};
    bool is_online_{false}; // bool goes after int because it is the smallest
    
    optional<string> network_alias_{nullopt};
    optional<string> ipv4_address_{nullopt};
    optional<string> ipv6_address_{nullopt};
    optional<string> getaway_ip_{nullopt};
    
    // add validatin static methods
    
    [[nodiscard]] static bool is_valid_error_count(int number_of_errors) noexcept {
        return number_of_errors >= 0 && number_of_errors <= 10000;
    }

    
    [[nodiscard]] static bool is_valid_ipv4(const string& ipv4) noexcept {
        if (ipv4.empty()) return false;
        struct sockaddr_in sa;
        return inet_pton(AF_INET, ipv4.c_str(), &(sa.sin_addr)) == 1;
    }
    
    [[nodiscard]] static bool is_valid_ipv6(const string& ipv6) noexcept {
       if (ipv6.empty()) return false;
        struct in6_addr sa6;
        return inet_pton(AF_INET6, ipv6.c_str(), &sa6) == 1;
    }
    
    
    
    
    
public:
    explicit MetaData(string firmware_version, int error_count, bool is_online, optional<string> network_alias = nullopt, optional<string> ipv4_address = nullopt, optional<string> ipv6_address = nullopt, optional<string> getaway_ip = nullopt)
    : firmware_version_(move(firmware_version)),
    error_count_(error_count),
    is_online_(is_online),
    network_alias_(move(network_alias)),
    ipv4_address_(move(ipv4_address)),
    ipv6_address_(move(ipv6_address)),
    getaway_ip_(move(getaway_ip)) {}
    
    MetaData() = default;
    
    

    
    
    // ~~~~~~~~~~ getters
    
    [[nodiscard]] const string& get_firmware_version() const noexcept {return firmware_version_;}
    
    [[nodiscard]] int get_error_count() const noexcept {return error_count_;}
    
    [[nodiscard]] bool get_is_online() const noexcept {return is_online_;}
    
    [[nodiscard]] const optional<string>& get_network_alias() const noexcept {return network_alias_;}
    
    [[nodiscard]] const optional<string>& get_ipv4_address() const noexcept {return ipv4_address_;} // щоби ці сетери працювали коректно треба буде писати при запуску .value_or("anything else") бо може нічого не повернути блін(((
    
    [[nodiscard]] const optional<string>& get_ipv6_address() const noexcept {return ipv6_address_;}
    
    [[nodiscard]] const optional<string>& get_getawat_ip_address() const noexcept {return getaway_ip_;}
    
    
    
    
    
    
    
    // ~~~~~~~~~~ setters and set-like-methods
    
        
    MetaData& set_firmware_version(string new_firmware_version) noexcept {
        auto temp = move(new_firmware_version);
        firmware_version_.swap(temp);
        return *this;
    }
    
    MetaData& reset_error_count() noexcept {
        error_count_  = 0;
        return *this;
    }
    
    MetaData& set_error_count(int new_error_count) {
        if (!is_valid_error_count(new_error_count)) {
            throw std::invalid_argument("ERROR: Error count is out of range!");
        }
        error_count_ = new_error_count;
        return *this;
    }
    

    MetaData& set_online_status(bool new_online_status) noexcept {
        is_online_ = new_online_status;
        return *this;
    }
    
    MetaData& set_network_alias(optional<string> new_network_alias) noexcept {
        auto temp = move(new_network_alias);
        network_alias_.swap(temp);
        return *this;
    }
    
    MetaData& set_ipv4_address(optional<string> new_ipv4)  { // add a validator
        if (new_ipv4.has_value() && !is_valid_ipv4(*new_ipv4)) {
            throw std::invalid_argument("ERROR: Ipv4 is not valid!");
        }
        auto temp = move(new_ipv4);
        ipv4_address_.swap(temp);
        return *this;
    }
    
 
    MetaData& set_ipv6_address(optional<string> new_ipv6)  { // add a validator
        if (new_ipv6.has_value() && !is_valid_ipv6(*new_ipv6)) {
            throw std::invalid_argument("ERROR: Ipv6 is not valid!");
        }
        auto temp = move(new_ipv6);
        ipv6_address_.swap(temp);
        return *this;
    }
    
    
    MetaData& set_getaway_ipv_address(optional<string> new_getaway_ip)  { // ipv4 typa shit
        if (new_getaway_ip.has_value() && !is_valid_ipv4(*new_getaway_ip)) {
            throw std::invalid_argument("ERROR: Ipv4 is not valid!");
        }
        auto temp = move(new_getaway_ip);
        getaway_ip_.swap(temp);
        return *this;
    }
    
    
    
    
    // ~~~~~~ methods
    
    void meta_info() const noexcept{
        cout << "Meta Info:\n";
        cout << "firmware version: " << firmware_version_ << "\n"
         << "Error count: " << error_count_ << "\n"
         << "Online: " << (is_online_ ? "yes" : "no") << "\n"
         << "Network alias: " << network_alias_.value_or("NO INFO") << "\n"
         << "IPv4: " << ipv4_address_.value_or("NO IPv4") << "\n"
         << "IPv6: " << ipv6_address_.value_or("NO IPv6") << "\n";
    }
    
    
    void metadata_configuration() {
        string buffer_s;
        int buffer_i;
        bool buffer_b;
        char choice;

        cout << "\n--- MetaData Configuration ---\n";

        cout << "Enter Firmware Version (e.g., v1.2.3): ";
        getline(cin >> ws, buffer_s);
        set_firmware_version(buffer_s);

        while (true) {
            cout << "Enter current Error Count: ";
            if (!(cin >> buffer_i)) {
                cout << "Invalid input. Please enter a number\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            try {
                set_error_count(buffer_i);
                break;
            } catch (const std::invalid_argument& e) {
                cout << e.what() << " Try again\n";
            }
        }

        
        cout << "Is the device online? (1 for yes, 0 for no): ";
        cin >> buffer_b;
        set_online_status(buffer_b);

        
        cout << "Set a network alias? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "Enter Alias: ";
            getline(cin >> ws, buffer_s);
            set_network_alias(make_optional(buffer_s));
        } else {
            set_network_alias(nullopt);
        }

        
        cout << "Configure IPv4? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            while (true) {
                cout << "Enter IPv4: ";
                getline(cin >> ws, buffer_s);
                try {
                    set_ipv4_address(make_optional(buffer_s));
                    break;
                } catch (const std::invalid_argument& e) {
                    cout << e.what() << " Please re-enter\n";
                }
            }
        } else {
            set_ipv4_address(nullopt);
        }

        cout << "Configure IPv6? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            while (true) {
                cout << "Enter IPv6: ";
                getline(cin >> ws, buffer_s);
                try {
                    set_ipv6_address(make_optional(buffer_s));
                    break;
                } catch (const std::invalid_argument& e) {
                    cout << e.what() << " Please re-enter\n";
                }
            }
        } else {
            set_ipv6_address(nullopt);
        }

        cout << "Configure Gateway IP? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            while (true) {
                cout << "Enter Gateway IP: ";
                getline(cin >> ws, buffer_s);
                try {
                    set_getaway_ipv_address(make_optional(buffer_s));
                    break;
                } catch (const std::invalid_argument& e) {
                    cout << e.what() << " Please re-enter\n";
                }
            }
        } else {
            set_getaway_ipv_address(nullopt);
        }

        cout << "MetaData successfully updated!\n";
    }
    
    
    
};
    
    


class NameplateData {
private:
    string manufacturer_{"Unknown"};
    int manufacture_year_{2000};
    string serial_number_{"000001"}; // nigga, at least try to fill that up
    optional<string> country_of_origin_{"USA"};
    optional<string> model_{"dv.1.0"};
    
    
    [[nodiscard]] static bool is_valid_manufacture_year(int manufacture_year) noexcept {
        return manufacture_year >= 1900 && manufacture_year <= 2100; // program will be outdated till 2100
    }
    
    
    
public:

    // ~~~~~~ constructors
    
    explicit NameplateData(string manufacturer, int manufacture_year, string serial_number, optional<string> country_of_origin = nullopt, optional<string> model = nullopt)
    : manufacturer_(move(manufacturer)),
    manufacture_year_(manufacture_year),
    serial_number_(move(serial_number)),
    country_of_origin_(move(country_of_origin)),
    model_(move(model)) {}

    NameplateData() = default;
    

    // ~~~~~~~~~~ gettersa
    
    [[nodiscard]] const string& get_manufacturer() const noexcept {return manufacturer_;}
    
    [[nodiscard]] int get_manufacture_year() const noexcept {return manufacture_year_;}
    
    [[nodiscard]] const string& get_serial_number() const noexcept {return serial_number_;}
    
    [[nodiscard]] const optional<string>& get_country_of_origin() const noexcept {return country_of_origin_;}
    
    [[nodiscard]] const optional<string>& get_model() const noexcept {return model_;}
    
    
    
    // ~~~~~~~~~~ setters
    
    NameplateData& set_manufacturer(string new_manufacturer) noexcept {
        auto temp = move(new_manufacturer);
        manufacturer_.swap(temp);
        return *this;
    }
    
    NameplateData& set_manufacture_year(int new_man_year) {
        if (!is_valid_manufacture_year(new_man_year)) {
            throw std::invalid_argument("ERROR: Year is out of range!");
        }
        manufacture_year_ = new_man_year;
        return *this;
    }
    
    
    NameplateData& set_serial_number(string new_serial_number) noexcept {
        auto temp = move(new_serial_number);
        serial_number_.swap(temp);
        return *this;
    }
    
    NameplateData& set_country_of_origin(optional<string> new_country_origin) { // check for valid country
        auto temp = move(new_country_origin);
        country_of_origin_.swap(temp);
        return *this;
    }

    NameplateData& set_model(optional<string> new_model) noexcept {
        auto temp = move(new_model);
        model_.swap(temp);
        return *this;
    }
    
    
    
    
    // ~~~~~~ methods
    
    void nameplate_info() const noexcept {
        cout << "Nameplate Info:\n";
        cout << "Manufacturer: " << manufacturer_ << "; \n"
         << "Model: " << model_.value_or("Unknown;") << "\n"
         << "Year: " << manufacture_year_ << "; \n"
         << "Origin: " << country_of_origin_.value_or("N/A;") << "\n";
    }
    
    

    void nameplate_configuration() {
        string buffer_s;
        int buffer_i;
        char choice;

        cout << "--- Nameplate Configuration ---\n";

        cout << "Enter Manufacturer: ";
        getline(cin >> ws, buffer_s);
        set_manufacturer(buffer_s);

        while (true) {
            cout << "Enter Manufacture Year: ";
            if (!(cin >> buffer_i)) {
                cout << "ERROR: Invalid input. Please enter a number\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            try {
                set_manufacture_year(buffer_i);
                break; // Success
            } catch (const std::invalid_argument& e) {
                cout << e.what() << "ERROR: Try again\n";
            }
        }

        cout << "Enter Serial Number: ";
        getline(cin >> ws, buffer_s);
        set_serial_number(buffer_s);

        cout << "Does the device have a specified Country of Origin? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "Enter Country: ";
            getline(cin >> ws, buffer_s);
            set_country_of_origin(make_optional(buffer_s));
        } else {
            set_country_of_origin(nullopt);
        }

        cout << "Does the device have a specific Model? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "Enter Model: ";
            getline(cin >> ws, buffer_s);
            set_model(make_optional(buffer_s));
        } else {
            set_model(nullopt);
        }

        cout << "Configuration complete!\n";
    }
    
    
};








// --- 1. Абстракція: Інтерфейс для дистанційного керування ---
class IRemote {
protected:
    bool is_remote_enabled_{true};
public:
    virtual ~IRemote() = default;
    virtual void toggle_remote(bool status) noexcept { is_remote_enabled_ = status; }
    virtual bool is_remote_active() const { return is_remote_enabled_; }
    virtual void send_signal() const = 0;
};


// --- 2. Базовий клас Device (Абстракція + Інкапсуляція) ---
class Device {
protected:
    string nickname_;
    MetaData meta_;
    NameplateData nameplate_;

public:
    Device(string nickname, MetaData meta, NameplateData nameplate)
        : nickname_(move(nickname)), meta_(move(meta)), nameplate_(move(nameplate)) {}
        
        
    explicit Device(string nickname)
     : Device(std::move(nickname), MetaData(), NameplateData()) {} // USER SKIPPING CONFIGURATION
    

    virtual ~Device() = default;

        
    
    
    
    
    [[nodiscard]] MetaData& meta() noexcept { return meta_; }
    [[nodiscard]] NameplateData& nameplate() noexcept { return nameplate_; }
    
    
    
    
    void config_meta() {
        meta().metadata_configuration();
    }
    
    void config_nameplate() {
        nameplate().nameplate_configuration();
    }

    virtual void identify() const {
        cout << "~~~ Device: " << nickname_ << " ~~~\n";
        nameplate_.nameplate_info();
        meta_.meta_info();
    }
    
    virtual void perform_action() = 0;
    
    
    [[nodiscard]] const string& get_nickname() const noexcept { return nickname_; }
};



// ?-Children my children, where chould i left you...

// TV використовує Множинне наслідування




vector<string> providers = {
    "Kyivstar", "Viasat", "Volia", "Lanet",
    "Triolan",
    "Ukrtelecom",
    "Tenet",
    "Vega",
    "Frigate",
    "McLaut",
    "Datagroup",
    "Sweet.tv",
    "Megogo"
};



class SmartTV : public Device, public IRemote {
private:
    int current_channel_{1};
    string provider_{"Kyivstar"};
    
public:
    using Device::Device; // constructor use
    
    void perform_action() override {
        cout << "[TV] Displaying content on channel " << current_channel_ << "\n";
    }

    void send_signal() const override {
        if (is_remote_enabled_) cout << "[TV Remote] Signal received! \n";
        else  cout << "[TV Remote] No Signal!\n";
    }
    
    
    void change_channel(const int new_channel) {
        if (new_channel >= 1000 || new_channel <= 0)  {
            cout << "[TV] ERROR: your channel is invalide\n";
            return;
        } else current_channel_ = new_channel;
    }
    

    void change_provider(const string new_provider) {
        auto it = std::find(providers.begin(), providers.end(), new_provider);
        if (it != providers.end()) {
            provider_ = new_provider;
        }
    }

    
    void identify() const override {
        cout << "~~~~~~~~~ Device: " << nickname_ << " | Type: TV ~~~~~~~~~" ;
        cout << "current_chanel_: " << current_channel_ << ";\n";
        cout << "Provider: " << provider_ << ";\n";
        
        // and the rest does here:
        nameplate_.nameplate_info();
        meta_.meta_info();
    }
    
    
};

class SmartLock : public Device, public IRemote {
private:
    bool locked_{true};
public:
    using Device::Device;

    void perform_action() override {
        locked_ = !locked_;
        cout << "[Lock] State changed to: " << (locked_ ? "LOCKED;" : "OPEN;") << "\n";
    }

    void send_signal() const override {
        if (is_remote_enabled_) cout << "[Lock Remote] Signal received! \n";
        else cout << "[Lock Remote] No Signal!\n";
    }
    
    void identify() const override {
        cout << "~~~~~~~~~ Device: " << nickname_ << " | Type: Smart Lock ~~~~~~~~~" ;
        cout << "Current state: " << (locked_ ? "LOCKED" : "OPEN") << ";\n";
        
        // and the rest does here:
        nameplate_.nameplate_info();
        meta_.meta_info();
    }
    
};

    

class SmartLights : public Device, public IRemote {
    int brightness_{100};
public:
    using Device::Device;

    void send_signal() const override {
        if (is_remote_enabled_) cout << "[Lights] Signal received! \n";
        else cout << "[Lights] No Signal!\n";
    }
    
    void perform_action() override {
        brightness_ += 10;
        brightness_ = brightness_ % 100;
        cout << "[Lights] Now: " << brightness_ << "% brightness!\n";
    }
    
    
    void set_brightness(const int new_brightness) noexcept {
        if (new_brightness >= 100) brightness_ = 100;
        else if (new_brightness <= 0) brightness_        = 0;
        else brightness_ = new_brightness;
    }
    
    
    void identify() const override {
        cout << "~~~~~~~~~ Device: " << nickname_ << " | Type: Smart Lights ~~~~~~~~~" ;
        cout << "Brightness: " << brightness_  << "%";
        
        // and the rest does here:
        nameplate_.nameplate_info();
        meta_.meta_info();
    }
};


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
    

public:
    explicit Program() = default;
    
    
    
    void start_menu_layout() {
        int i = 0;
        size_t length = start_menu_.size();
        for (auto punkt : start_menu_) {
            if (i == length) cout << "e. " << punkt << ";\n";
            else cout << ++i << ". " << punkt << ";\n";
        }
        cout << "\n";
    }
    
    void config_menu_layout() {
        int i = 0;
        size_t length = сonfig_menu_.size();
        for (auto punkt : сonfig_menu_) {
            if (i == length) cout << "e. " << punkt << ";\n";
            else cout << ++i << ". " << punkt << ";\n";
        }
        cout << "\n";
    }
    
};


void run_info(Program& app) {
    
}

void run_functional(Program& app) {
    
}

void run_info(Program& app) {
    
}



void run_main(Program& app) {
    char userChoice = 's';
        
        while (userChoice != 'e' && userChoice != 'E') {
            app.start_menu_layout();
            cin >> userChoice;

            switch (userChoice) {
                case '1':
                    cout << "[System] Displaying device info...\n";
                    // app.show_info();
                    break;
                case '2':
                    cout << "[System] Entering functional mode...\n";
                    break;
                case '3':
                    app.config_menu_layout();
                    // Nested loop or logic for config could go here
                    break;
                case '4':
                    cout << "[System] Creating new device...\n";
                    break;
                case '5':
                    cout << "[System] Deleting device...\n";
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
