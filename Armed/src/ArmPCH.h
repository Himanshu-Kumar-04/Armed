#pragma once

#include <cstdint>

#include<iostream>
#include<memory>
#include<utility>
#include<algorithm>
#include<functional>
#include<sstream>
#include<fstream>
		
#include<string>
#include<vector>
#include<unordered_map>
#include<unordered_set>

#ifdef ARMED_PLATFORM_WINDOWS
#include<Windows.h>
#endif // ARMED_PLATFORM_WINDOWS

#define ASSERT(x,y) if(!(x)) {std::cout << y; __debugbreak();}
