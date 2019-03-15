#include <iostream>
#include "employee.h"

// Overloaded operator for comparison from list.cc
bool Employee :: operator==(Employee& empObj) {
	if (this->empId == empObj.empId) {
		return true;
	}
	else {
		return false;
	}
}