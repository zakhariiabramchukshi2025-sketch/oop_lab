#include "../includes/source.hpp"
#include <arpa/inet.h>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;


vector<string> providers = {
    "Kyivstar", "Viasat", "Volia", "Lanet", "Triolan", "Ukrtelecom",
    "Tenet", "Vega", "Frigate", "McLaut", "Datagroup", "Sweet.tv", "Megogo"
};

// --- MetaData Implementation ---

// ~~~~~~~~~~ validation

MetaData::MetaData(string firmware_version, int error_count, bool is_online,
                   optional<string> network_alias, optional<string> ipv4_address,
                   optional<string> ipv6_address, optional<string> getaway_ip)
    : firmware_version_(move(firmware_version)), error_count_(error_count), is_online_(is_online),
      network_alias_(move(network_alias)), ipv4_address_(move(ipv4_address)),
      ipv6_address_(move(ipv6_address)), getaway_ip_(move(getaway_ip)) {}
        
bool MetaData::is_valid_error_count(int number_of_errors) noexcept {
    return number_of_errors >= 0 && number_of_errors <= 10000;
}

bool MetaData::is_valid_ipv4(const string& ipv4) noexcept {
    if (ipv4.empty()) return false;
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ipv4.c_str(),         &(sa.sin_addr)) == 1;
}

bool MetaData::is_valid_ipv6(const string& ipv6) noexcept {
    if (ipv6.empty()) return false;
    struct in6_addr sa6;
    return inet_pton(AF_INET6, ipv6.c_str(), &sa6) == 1;
}



// ~~~~~~~~~~ getters

[[nodiscard]] const string& MetaData::get_firmware_version() const noexcept {return firmware_version_;}

[[nodiscard]] int MetaData::get_error_count() const noexcept {return error_count_;}

[[nodiscard]] bool MetaData::get_is_online() const noexcept {return is_online_;}

[[nodiscard]] const optional<string>& MetaData::get_network_alias() const noexcept {return network_alias_;}

[[nodiscard]] const optional<string>& MetaData::get_ipv4_address() const noexcept {return ipv4_address_;} // щоби ці сетери працювали коректно треба буде писати при запуску .value_or("anything else") бо може нічого не повернути блін(((

[[nodiscard]] const optional<string>& MetaData::get_ipv6_address() const noexcept {return ipv6_address_;}

[[nodiscard]] const optional<string>& MetaData::get_getawat_ip_address() const noexcept {return getaway_ip_;}





// ~~~~~~~~~~ setters and set-like-methods

MetaData& MetaData::set_firmware_version(string new_firmware_version) noexcept {
    auto temp = move(new_firmware_version);
    firmware_version_.swap(temp);
    return *this;
}

MetaData& MetaData::reset_error_count() noexcept {
    error_count_  = 0;
    return *this;
}

MetaData& MetaData::set_error_count(int new_error_count) {
    if (!is_valid_error_count(new_error_count)) {
        throw std::invalid_argument("ERROR: Error count is out of range!");
    }
    error_count_ = new_error_count;
    return *this;
}


MetaData& MetaData::set_online_status(bool new_online_status) noexcept {
    is_online_ = new_online_status;
    return *this;
}

MetaData& MetaData::set_network_alias(optional<string> new_network_alias) noexcept {
    auto temp = move(new_network_alias);
    network_alias_.swap(temp);
    return *this;
}

MetaData& MetaData::set_ipv4_address(optional<string> new_ipv4)  { // add a validator
    if (new_ipv4.has_value() && !is_valid_ipv4(*new_ipv4)) {
        throw std::invalid_argument("ERROR: Ipv4 is not valid!");
    }
    auto temp = move(new_ipv4);
    ipv4_address_.swap(temp);
    return *this;
}


MetaData& MetaData::set_ipv6_address(optional<string> new_ipv6)  { // add a validator
    if (new_ipv6.has_value() && !is_valid_ipv6(*new_ipv6)) {
        throw std::invalid_argument("ERROR: Ipv6 is not valid!");
    }
    auto temp = move(new_ipv6);
    ipv6_address_.swap(temp);
    return *this;
}


MetaData& MetaData::set_getaway_ipv_address(optional<string> new_getaway_ip)  { // ipv4 typa shit
    if (new_getaway_ip.has_value() && !is_valid_ipv4(*new_getaway_ip)) {
        throw std::invalid_argument("ERROR: Ipv4 is not valid!");
    }
    auto temp = move(new_getaway_ip);
    getaway_ip_.swap(temp);
    return *this;
}



// ~~~~~~ methods

void MetaData::meta_info() const noexcept{
    cout << "Meta Info:\n";
    cout << "firmware version: " << firmware_version_ << "\n"
     << "Error count: " << error_count_ << "\n"
     << "Online: " << (is_online_ ? "yes" : "no") << "\n"
     << "Network alias: " << network_alias_.value_or("NO INFO") << "\n"
     << "IPv4: " << ipv4_address_.value_or("NO IPv4") << "\n"
     << "IPv6: " << ipv6_address_.value_or("NO IPv6") << "\n";
}

void MetaData::metadata_configuration() {
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
















// --- NameplateData Implementation ---
  
NameplateData::NameplateData(string man, int year, string sn, optional<string> coo, optional<string> mod)
    : manufacturer_(move(man)), manufacture_year_(year), serial_number_(move(sn)),
      country_of_origin_(move(coo)), model_(move(mod)) {}




// ~~~~~~ validation

[[nodiscard]] bool NameplateData::is_valid_manufacture_year(int manufacture_year) noexcept { // static can only be specified in hpp file
    return manufacture_year >= 1900 && manufacture_year <= 2100; // program will be outdated till 2100
}





// ~~~~~~~~~~ getters

[[nodiscard]] const string& NameplateData::get_manufacturer() const noexcept {return manufacturer_;}

[[nodiscard]] int NameplateData::get_manufacture_year() const noexcept {return manufacture_year_;}

[[nodiscard]] const string& NameplateData::get_serial_number() const noexcept {return serial_number_;}

[[nodiscard]] const optional<string>& NameplateData::get_country_of_origin() const noexcept {return country_of_origin_;}

[[nodiscard]] const optional<string>& NameplateData::get_model() const noexcept {return model_;}



// ~~~~~~~~~~ setters

NameplateData& NameplateData::set_manufacturer(string new_manufacturer) noexcept {
    auto temp = move(new_manufacturer);
    manufacturer_.swap(temp);
    return *this;
}

NameplateData& NameplateData::set_manufacture_year(int new_man_year) {
    if (!is_valid_manufacture_year(new_man_year)) {
        throw std::invalid_argument("ERROR: Year is out of range!");
    }
    manufacture_year_ = new_man_year;
    return *this;
}


NameplateData& NameplateData::set_serial_number(string new_serial_number) noexcept {
    auto temp = move(new_serial_number);
    serial_number_.swap(temp);
    return *this;
}

NameplateData& NameplateData::set_country_of_origin(optional<string> new_country_origin) { // check for valid country
    auto temp = move(new_country_origin);
    country_of_origin_.swap(temp);
    return *this;
}

NameplateData& NameplateData::set_model(optional<string> new_model) noexcept {
    auto temp = move(new_model);
    model_.swap(temp);
    return *this;
}




// ~~~~~~ methods

void NameplateData::nameplate_info() const noexcept {
    cout << "Nameplate Info:\n";
    cout << "Manufacturer: " << manufacturer_ << "; \n"
     << "Model: " << model_.value_or("Unknown;") << "\n"
     << "Year: " << manufacture_year_ << "; \n"
     << "Origin: " << country_of_origin_.value_or("N/A;") << "\n";
}



void NameplateData::nameplate_configuration() {
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







// --- Device Implementation ---
Device::Device(string nickname, MetaData meta, NameplateData nameplate)
: nickname_(move(nickname)), meta_(move(meta)), nameplate_(move(nameplate)) {}

Device::Device(string nickname)
: Device(std::move(nickname), MetaData(), NameplateData()) {} // USER SKIPPING CONFIGURATION

void Device::identify() const {
    cout << "~~~ Device: " << nickname_ << " ~~~\n";
    nameplate_.nameplate_info();
    meta_.meta_info();
}



void Device::config_meta() { meta().metadata_configuration(); }
void Device::config_nameplate() { nameplate().nameplate_configuration(); }







// --- Devices Implementation ---


// --- SmartTV

void SmartTV::perform_action()   {
    cout << "[TV] Displaying content on channel " << current_channel_ << "\n";
}

void SmartTV::send_signal() const   {
    if (is_remote_enabled_) cout << "[TV Remote] Signal received! \n";
    else  cout << "[TV Remote] No Signal!\n";
}


void SmartTV::change_channel(const int new_channel) {
    if (new_channel >= 1000 || new_channel <= 0)  {
        cout << "[TV] ERROR: your channel is invalide\n";
        return;
    } else current_channel_ = new_channel;
}


void SmartTV::change_provider(const string new_provider) {
    auto it = std::find(providers.begin(), providers.end(), new_provider);
    if (it != providers.end()) {
        provider_ = new_provider;
    }
}


void SmartTV::identify() const   {
    cout << "~~~~~~~~~ Device: " << nickname_ << " | Type: TV ~~~~~~~~~" ;
    cout << "current_chanel_: " << current_channel_ << ";\n";
    cout << "Provider: " << provider_ << ";\n";
    
    // and the rest does here:
    nameplate_.nameplate_info();
    meta_.meta_info();
}




// --- SmartLock

void SmartLock::perform_action()   {
    locked_ = !locked_;
    cout << "[Lock] State changed to: " << (locked_ ? "LOCKED;" : "OPEN;") << "\n";
}

void SmartLock::send_signal() const   {
    if (is_remote_enabled_) cout << "[Lock Remote] Signal received! \n";
    else cout << "[Lock Remote] No Signal!\n";
}

void SmartLock::identify() const   {
    cout << "~~~~~~~~~ Device: " << nickname_ << " | Type: Smart Lock ~~~~~~~~~" ;
    cout << "Current state: " << (locked_ ? "LOCKED" : "OPEN") << ";\n";
    
    // and the rest does here:
    nameplate_.nameplate_info();
    meta_.meta_info();
}





// --- SmartLights

void SmartLights::send_signal() const   {
    if (is_remote_enabled_) cout << "[Lights] Signal received! \n";
    else cout << "[Lights] No Signal!\n";
}

void SmartLights::perform_action()   {
    brightness_ += 10;
    brightness_ = brightness_ % 100;
    cout << "[Lights] Now: " << brightness_ << "% brightness!\n";
}


void SmartLights::set_brightness(const int new_brightness) noexcept {
    if (new_brightness >= 100) brightness_ = 100;
    else if (new_brightness <= 0) brightness_        = 0;
    else brightness_ = new_brightness;
}


void SmartLights::identify() const   {
    cout << "~~~~~~~~~ Device: " << nickname_ << " | Type: Smart Lights ~~~~~~~~~" ;
    cout << "Brightness: " << brightness_  << "%";
    
    // and the rest does here:
    nameplate_.nameplate_info();
    meta_.meta_info();
}


