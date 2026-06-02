#pragma once
#include <string>
#include "Stack.hpp"
using namespace std;

struct Robot {
    string robotID;
    string currentTask;
    string status;
    Stack history;
};