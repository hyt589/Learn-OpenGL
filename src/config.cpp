#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iomanip>

#ifdef WINDOWS
#define CONFIG_PATH "resources\\config.json"
#else
#define CONFIG_PATH "resources/config.json"
#endif

using JSON = nlohmann::json;

JSON loadConfig(){
    JSON config;
    std::ifstream in(CONFIG_PATH);
    in >> config;
    return config;
}

void updateConfig(JSON config){
    std::ofstream out(CONFIG_PATH);
    out << std::setw(4) << config;
}