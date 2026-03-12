#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <memory>

// Using declarations for cleaner headers (limited to common types)
using std::string;
using std::vector;
using std::optional;
using std::unique_ptr;
using std::nullopt;

// External list of providers (defined in cpp)
extern vector<string> providers;

// --- MetaData Class ---
class MetaData {
private:
    string firmware_version_{"v1.0.0"};
    int error_count_{0};
    bool is_online_{false};
    optional<string> network_alias_{nullopt};
    optional<string> ipv4_address_{nullopt};
    optional<string> ipv6_address_{nullopt};
    optional<string> getaway_ip_{nullopt};

    [[nodiscard]] static bool is_valid_error_count(int number_of_errors) noexcept;
    [[nodiscard]] static bool is_valid_ipv4(const string& ipv4) noexcept;
    [[nodiscard]] static bool is_valid_ipv6(const string& ipv6) noexcept;

public:
    MetaData() = default;
    explicit MetaData(string firmware_version, int error_count, bool is_online,
                     optional<string> network_alias = nullopt,
                     optional<string> ipv4_address = nullopt,
                     optional<string> ipv6_address = nullopt,
                     optional<string> getaway_ip = nullopt);

    // Getters
    [[nodiscard]] const string& get_firmware_version() const noexcept {return firmware_version_;}
    
    [[nodiscard]] int get_error_count() const noexcept {return error_count_;}
    
    [[nodiscard]] bool get_is_online() const noexcept {return is_online_;}
    
    [[nodiscard]] const optional<string>& get_network_alias() const noexcept {return network_alias_;}
    
    [[nodiscard]] const optional<string>& get_ipv4_address() const noexcept {return ipv4_address_;} // щоби ці сетери працювали коректно треба буде писати при запуску .value_or("anything else") бо може нічого не повернути блін(((
    
    [[nodiscard]] const optional<string>& get_ipv6_address() const noexcept {return ipv6_address_;}
    
    [[nodiscard]] const optional<string>& get_getawat_ip_address() const noexcept {return getaway_ip_;}
    
    
    
    // Setters
    MetaData& set_firmware_version(string new_firmware_version) noexcept;
    MetaData& reset_error_count() noexcept;
    MetaData& set_error_count(int new_error_count);
    MetaData& set_online_status(bool new_online_status) noexcept;
    MetaData& set_network_alias(optional<string> new_network_alias) noexcept;
    MetaData& set_ipv4_address(optional<string> new_ipv4);
    MetaData& set_ipv6_address(optional<string> new_ipv6);
    MetaData& set_getaway_ipv_address(optional<string> new_getaway_ip);

    void meta_info() const noexcept;
    void metadata_configuration();
};

// --- NameplateData Class ---
class NameplateData {
private:
    string manufacturer_{"Unknown"};
    int manufacture_year_{2000};
    string serial_number_{"000001"};
    optional<string> country_of_origin_{"USA"};
    optional<string> model_{"dv.1.0"};

    [[nodiscard]] static bool is_valid_manufacture_year(int manufacture_year) noexcept;

public:
    NameplateData() = default;
    
    explicit NameplateData(string manufacturer, int manufacture_year, string serial_number,
                          optional<string> country_of_origin = nullopt,
                          optional<string> model = nullopt);

    // Getters
    [[nodiscard]] const string& get_manufacturer() const noexcept {return manufacturer_;}
    [[nodiscard]] int get_manufacture_year() const noexcept {return manufacture_year_;}
    [[nodiscard]] const string& get_serial_number() const noexcept {return serial_number_;}
    [[nodiscard]] const optional<string>& get_country_of_origin() const noexcept {return country_of_origin_;}
    [[nodiscard]] const optional<string>& get_model() const noexcept {return model_;}

    // Setters
    NameplateData& set_manufacturer(string new_manufacturer) noexcept;
    NameplateData& set_manufacture_year(int new_man_year);
    NameplateData& set_serial_number(string new_serial_number) noexcept;
    NameplateData& set_country_of_origin(optional<string> new_country_origin);
    NameplateData& set_model(optional<string> new_model) noexcept;

    void nameplate_info() const noexcept;
    void nameplate_configuration();
};




// --- Interfaces ---
class IRemote {
protected:
    bool is_remote_enabled_{true};
public:
    virtual ~IRemote() = default;
    virtual void toggle_remote(bool status) noexcept { is_remote_enabled_ = status; }
    virtual bool is_remote_active() const { return is_remote_enabled_; }
    virtual void send_signal() const = 0;
};





// --- Base Device Class ---
class Device {
protected:
    string nickname_;
    MetaData meta_;
    NameplateData nameplate_;

public:
    Device(string nickname, MetaData meta, NameplateData nameplate);
    explicit Device(string nickname);
    virtual ~Device() = default;

    [[nodiscard]] MetaData& meta() noexcept { return meta_; }
    [[nodiscard]] NameplateData& nameplate() noexcept { return nameplate_; }
    
    [[nodiscard]] const string& get_nickname() const noexcept { return nickname_; }

    void config_meta();
    void config_nameplate();
    virtual void identify() const;
    virtual void perform_action() = 0;
};








// --- Concrete Devices ---
class SmartTV : public Device, public IRemote {
private:
    int current_channel_{1};
    string provider_{"Kyivstar"};
public:
    using Device::Device;
    
    void perform_action() override;
    void send_signal() const override;
    void change_channel(const int new_channel);
    void change_provider(const string new_provider);
    void identify() const override;
    
    
};


class SmartLock : public Device, public IRemote {
private:
    bool locked_{true};
public:
    using Device::Device;
    
    void perform_action() override;
    void send_signal() const override;
    void identify() const override;
    
    
    
};



class SmartLights : public Device, public IRemote {
private:
    int brightness_{100};
public:
    using Device::Device;
    
    void perform_action() override;
    void send_signal() const override;
    void set_brightness(const int new_brightness) noexcept;
    void identify() const override;
    
    
};
