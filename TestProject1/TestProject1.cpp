#include <stdio.h>
#include <string>
#include <memory>
#include <chrono>
#include <string_view>
#include "StrBuilder.h"

/*
	* TASK: please address all potential performance issues in this code
*/

/*
  Change the class to either detect issues at compile time or 
  correct the potential issues.
*/

/* 
(1) Cosmetic warnings: either we use egypt brackets(class and main function are defined with different style), or normal ones

(2) Method "copy" doesn't make sense as it can be replaced with copy ctor and copy operator.
Also, method copy does a shallow copy, not a deep copy

(3) I would make it to check for bad_alloc exception when allocating memory(in case we create a lot of MyFileData or large objects) 
and add unique ptr usage as we throw exception in ctors. Or we can also use std::array if we are 100% sure the array of chars is fixed size.
Approach with unique ptr seems to me more universal in this case.

(4) If we declare one of 3 or 5(after C++11) methods (dtor, copy, copy op, move, move op), it's better to declare all of them (rule of 3/5)

(5) We usually use chrono to calculate time in C++, instead of time.h which is C-style

(6) I would add a universal function that measures time of execution, hence: void measureExecutionTime()

(7) printf is part of the C standard library, while std::cout is part of the C++ standard library. 
Mixing C and C++ library functions can lead to compatibility issues and may not take full advantage of C++ features.

(8) Using namespace std generally considered bad practice due to the potential for naming conflicts and reduced code readability
*/

#define MAX_LENGTH 10

class MyFileData {
	std::unique_ptr<char[]> myText;

public:
	MyFileData() {
		try {
			myText = std::make_unique<char[]>(MAX_LENGTH);
		}
		catch (const std::bad_alloc& e) {
			std::cerr << "[ctor] Memory allocation failed: " << e.what() << std::endl;
			throw;
		}
	} 

	~MyFileData() { /*No need to check if myText is nullptr, std::unique_ptr handles it*/ }

	MyFileData(MyFileData& rhs) {
		if (&rhs == this) {
			return;
		}

		try {
			myText = std::make_unique<char[]>(MAX_LENGTH);
			// Perform deep copy
			std::memcpy(myText.get(), rhs.myText.get(), MAX_LENGTH * sizeof(char));
		}
		catch (const std::bad_alloc& e) {
			std::cerr << "[copy ctor] Memory allocation failed: " << e.what() << std::endl;
			throw; 
		}
	}

	MyFileData& operator=(MyFileData& rhs) {
		if (&rhs == this) {
			return rhs;
		}

		try {
			myText = std::make_unique<char[]>(MAX_LENGTH);
			// Perform deep copy
			std::memcpy(myText.get(), rhs.myText.get(), MAX_LENGTH * sizeof(char));
		}
		catch (const std::bad_alloc& e) {
			std::cerr << "[copy op] Memory allocation failed: " << e.what() << std::endl;
			throw;
		}

		return *this;
	}
};

// Decorator function to calculate the time of execution for any callable object
template<typename Func, typename... Args>
void measureExecutionTime(Func&& func, Args&&... args, const std::string_view& functionName) {
	auto startTime = std::chrono::steady_clock::now();
	std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
	auto endTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> duration = endTime - startTime;
	std::cout << "Execution time of " << functionName << ": " << duration.count() << " seconds" << std::endl;
}

int main(int argc, char* argv[]) {
	std::cout << "String Builder test..." << "\n";

	CStringBuilder StringBuilder;
	// generating test data
	StringBuilder.GenerateTestData(10000);
	measureExecutionTime([&StringBuilder](){ StringBuilder.GetData(); }, "CStringBuilder::GetData()");

	MyFileData data;
	MyFileData dataCopy = data; //implicit copy ctor
	MyFileData data1;
	data1 = data; //copy operator

	return 0;
}

