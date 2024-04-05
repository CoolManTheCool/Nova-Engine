#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>

struct Settings {
    // Singleton instance
    static Settings* getInstance();
    Settings() = delete; //delete constructor
    Settings(const Settings&) = delete; //delete copy constructor
    Settings(Settings&&) = delete; //delete move constructor
    Settings& operator=(const Settings&) = delete; //delete copy assignment operator
    Settings& operator=(Settings&&) = delete; //delete move assignment operator
    ~Settings() = delete; //delete destructor
    // Example settings
    int width = 1280;
    int height = 720;
    std::string title = "Vulkan Thingy";

};

#endif