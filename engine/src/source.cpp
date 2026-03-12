#include "source.hpp"
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
    return inet_pton(AF_INET, ipv4.c_str(), &(sa.sin_addr)) == 1;
}

bool MetaData::is_valid_ipv6(const string& ipv6) noexcept {
    if (ipv6.empty()) return false;
    struct in6_addr sa6;
    return inet_pton(AF_INET6, ipv6.c_str(), &sa6) == 1;
}

MetaData& MetaData::set_firmware_version(string new_v) noexcept { firmware_version_ = move(new_v); return *this; }
MetaData& MetaData::reset_error_count() noexcept { error_count_ = 0; return *this; }
MetaData& MetaData::set_error_count(int count) {
    if (!is_valid_error_count(count)) throw invalid_argument("ERROR: Error count range!");
    error_count_ = count; return *this;
}
MetaData& MetaData::set_online_status(bool status) noexcept { is_online_ = status; return *this; }
MetaData& MetaData::set_network_alias(optional<string> alias) noexcept { network_alias_ = move(alias); return *this; }
MetaData& MetaData::set_ipv4_address(optional<string> ip) {
    if (ip && !is_valid_ipv4(*ip)) throw invalid_argument("Invalid IPv4");
    ipv4_address_ = move(ip); return *this;
}
MetaData& MetaData::set_ipv6_address(optional<string> ip) {
    if (ip && !is_valid_ipv6(*ip)) throw invalid_argument("Invalid IPv6");
    ipv6_address_ = move(ip); return *this;
}
MetaData& MetaData::set_getaway_ipv_address(optional<string> ip) {
    if (ip && !is_valid_ipv4(*ip)) throw invalid_argument("Invalid Gateway IP");
    getaway_ip_ = move(ip); return *this;
}

void MetaData::meta_info() const noexcept {
    cout << "Meta Info:\n" << "firmware: " << firmware_version_ << "\nErrors: " << error_count_
         << "\nOnline: " << (is_online_ ? "yes" : "no") << "\nIPv4: " << ipv4_address_.value_or("N/A") << "\n";
}

void MetaData::metadata_configuration() {
    string buf_s; int buf_i; bool buf_b; char choice;
    cout << "\n--- MetaData Config ---\nFirmware: ";
    getline(cin >> ws, buf_s); set_firmware_version(buf_s);
    
    while(true) {
        cout << "Error Count: ";
        if (!(cin >> buf_i)) { cin.clear(); cin.ignore(1000, '\n'); continue; }
        try { set_error_count(buf_i); break; } catch(...) { cout << "Invalid.\n"; }
    }
    cout << "Online (1/0): "; cin >> buf_b; set_online_status(buf_b);
    // ... (Optional fields follow the same logic as previous config)
}

// --- NameplateData Implementation ---
NameplateData::NameplateData(string man, int year, string sn, optional<string> coo, optional<string> mod)
    : manufacturer_(move(man)), manufacture_year_(year), serial_number_(move(sn)),
      country_of_origin_(move(coo)), model_(move(mod)) {}

bool NameplateData::is_valid_manufacture_year(int year) noexcept { return year >= 1900 && year <= 2100; }

NameplateData& NameplateData::set_manufacture_year(int y) {
    if(!is_valid_manufacture_year(y)) throw invalid_argument("Year Range");
    manufacture_year_ = y; return *this;
}
// ... (Other simple setters)

void NameplateData::nameplate_info() const noexcept {
    cout << "Manufacturer: " << manufacturer_ << "\nYear: " << manufacture_year_ << "\n";
}

void NameplateData::nameplate_configuration() {
    // Logic for Nameplate input...
}

// --- Device Implementation ---
Device::Device(string nick, MetaData m, NameplateData n)
    : nickname_(move(nick)), meta_(move(m)), nameplate_(move(n)) {}

Device::Device(string nick) : Device(move(nick), MetaData(), NameplateData()) {}

void Device::identify() const {
    cout << "~~~ Device: " << nickname_ << " ~~~\n";
    nameplate_.nameplate_info();
    meta_.meta_info();
}
void Device::config_meta() { meta_.metadata_configuration(); }
void Device::config_nameplate() { nameplate_.nameplate_configuration(); }

// --- Specific Device Methods ---
void SmartTV::perform_action() { cout << "[TV] Channel: " << current_channel_ << "\n"; }
void SmartTV::send_signal() const { cout << (is_remote_enabled_ ? "[TV] Signal OK\n" : "[TV] Locked\n"); }
void SmartTV::identify() const { cout << "Type: SmartTV | Nick: " << nickname_ << "\n"; }

void SmartLock::perform_action() { locked_ = !locked_; cout << "Lock is " << (locked_ ? "CLOSED" : "OPEN") << "\n"; }
void SmartLock::send_signal() const { cout << "[Lock] Signal received\n"; }
void SmartLock::identify() const { cout << "Type: SmartLock | Nick: " << nickname_ << "\n"; }

void SmartLights::perform_action() { brightness_ = (brightness_ + 10) % 110; cout << "Brightness: " << brightness_ << "%\n"; }
void SmartLights::send_signal() const { cout << "[Lights] Signal received\n"; }
void SmartLights::set_brightness(int b) noexcept { brightness_ = (b > 100) ? 100 : (b < 0 ? 0 : b); }
void SmartLights::identify() const { cout << "Type: SmartLights | Nick: " << nickname_ << "\n"; }
