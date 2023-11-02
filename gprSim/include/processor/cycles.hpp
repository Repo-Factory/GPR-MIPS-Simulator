#include <functional>
#include <string>

#define NUM_FUNCTIONAL_UNITS 5
#define NUM_INSTRUCTIONS     11

std::function<int*(bool, const int*)> cyclesPerUnitClosure();
std::function<std::vector<std::vector<int>>()> cyclesTableClosure();
const std::function<std::vector<std::string>()> functionalUnitsClosure();
const std::function<int(bool)> instructionsExecutedClosure();
