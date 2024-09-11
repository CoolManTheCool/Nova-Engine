#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "gui_system.hpp"

namespace nova {

class Editor_T {
public:
    Editor_T();
    void RegisterBindings();
    std::function<void()> RegisterWindow(nova_Window* window);
    void update();
    
private:

};

}

#endif