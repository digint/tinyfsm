#ifndef DEBUG_HPP_INCLUDED
#define DEBUG_HPP_INCLUDED


#ifdef DEBUG
#include <iostream>
#define DBG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DBG(str) do { } while ( false )
#endif

#endif
