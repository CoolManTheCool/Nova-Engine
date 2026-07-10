#pragma once

#include <memory>
#include "objects/object.hpp"

namespace Nova {

class Window;
class Device;
class Renderer;

struct RenderData;

class System {
public:
    System(Window& window, Device& device, Renderer& renderer);
    virtual void init() = 0;

	/**
	 * @brief Update logic, called once per frame.
	 * @param deltaTime Time elapsed since last frame in seconds.
	 */
	virtual void update(double deltaTime);

	/**
	 * @brief Render logic, called once per frame.
	 * Note that this is only callable internally
	 * @param renderData Secret struct containing rendering context data.
	 */
	virtual void render(RenderData& renderData);
    
protected: 
    // Children have access
    // Private does not give children access... I understand it now.
    // It's like guns and stuff are protected but not private.

    Window& window;
    Device& device;
    Renderer& renderer;
};

}