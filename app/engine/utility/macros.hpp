#ifndef MACROS_HPP
#define MACROS_HPP

#ifdef _WIN32
  #ifdef NOVA_ENGINE_BUILD
    #define NOVA_ENGINE_API __declspec(dllexport)
  #else
    #define NOVA_ENGINE_API __declspec(dllimport)
  #endif
#else
  #define NOVA_ENGINE_API
#endif

#endif // MACROS_HPP