#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP
/**
 * @class Threaded Physics
 */
class Threaded_Physics_Wrapper {
public:
    Threaded_Physics_Wrapper(bool isThreaded);
private:
    bool isThreaded;
};

/**
 * @class Physics System
 * @brief Independant, Non-global, Physics Engine  
 * 
 * Provides an algorithmic approach to solving physics in real time.
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