#pragma once
#include<Windows.h>
#include<iostream>
#include<vector>
#include<functional>
#include<map>
#include<set>
#include<thread>
#include<assert.h>
#include<tuple>
#include "WinApiTypes.h"
#include "util.h"
#include "ThreadManager.h"

#define DEBUG_LOG(location, msg) std::cerr<< "[DEBUG] " << location << " - " << msg << '\n';
