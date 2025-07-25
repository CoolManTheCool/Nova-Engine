#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

/**
 * @class Physics System
 * @brief Independant, Non-global, Physics Engine  
 * 
 * Provides an algorithmic approach to solving physics in real time.
 * This class is designed to be used within the Nova Engine framework.
 * The system is designed to be multi-threaded and can be extended to support various physics simulations.
 */
class Physics_System {
public:
    /**
     * @brief Ray-Triangle Intersection Test.
     * 
     *  Möller–Trumbore Intersection Algorithm
     * 
     * @return True/False if the ray colides with the triangle 
     */
    bool RayTriangleIntersection();

private:
    // Store Simulation Data

};

#endif