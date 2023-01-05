#include <iostream>
#include <fstream>
#include <sstream>

#include "config.hpp"
#include "macros.hpp"

struct ConfigEntry {
    std::string key;
    std::string value;
};

// split the line by ':' delimiter, get key and value.
ConfigEntry get_entry(std::string &line) {
    std::stringstream ss(line);
    std::string key;
    std::string value;
    getline(ss, key, ':');
    getline(ss, value, ':');
    return (struct ConfigEntry) {
        key,
            value
    };
}

Config::Config() {
    std::string line;
    std::ifstream cfg_file ("src/termemu.config");
    if (cfg_file.is_open()) {
        while (getline (cfg_file,line)) {

            struct ConfigEntry cfg = get_entry(line);
            if (cfg.key.compare("font") == 0) {
                this->font_path = cfg.value;
            }

        }
        cfg_file.close();
    } else {
        errExit("failed to open configuration file 'termemu.config'");
    }
}

std::string Config::get_font_path() {
    return this->font_path;
}
