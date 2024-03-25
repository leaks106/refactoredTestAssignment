#ifndef _STR_BUILDER_H__
#define _STR_BUILDER_H__

#include <random>
#include <string>
#include <vector>
#include <iostream>
#include <numeric>

typedef std::vector<std::string> v_strings;

//Refactoring this class we can get rid of cyclomatic complexity using C++ 20 algorithms, this will improve readability

class CStringBuilder {
public:
	CStringBuilder() {}

	////////////////////////////////////////////////////////////////////////////////
	// generate nStrCount test strings in vector m_vStrings
	void GenerateTestData(size_t nStrCount) {
		m_vStrings.clear();
		m_vStrings.resize(nStrCount);

		std::random_device rd;
		std::uniform_int_distribution<size_t> rnd_generator(1, 2000);

		std::generate(begin(m_vStrings), end(m_vStrings), [&rd, &rnd_generator]() { // C++ 20
			return std::string(rnd_generator(rd), 'a');
			});
	}

	////////////////////////////////////////////////////////////////////////////////
	// returns string concatenated from m_vStrings
	std::string GetData() const {
		return std::accumulate(begin(m_vStrings), end(m_vStrings), std::string("")); //C++ 20
	}

private:
	v_strings m_vStrings;
};

#endif // _STR_BUILDER_H__
