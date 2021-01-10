/* 
 * Name: serializer.cpp
 * Title: Serializer class definition
 * Author: Vignesh Suresh
 * Date: May 30, 2020
 *
 */

#include <serializer.h>

// Using the entire std library to avoid importing libraries individually
using namespace std;

// Constructor for Serializer class
Serializer::Serializer()
{
	cout << "[MEMORY CONSTRUCTOR] Serializer has been created!" << endl;
}

// Destructor for Serializer class
Serializer::~Serializer(void)
{
	cout << "[MEMORY DESTRUCTOR] Serializer has been deleted!" << endl;
}

// Copy constructor for Serializer class
Serializer::Serializer(const Serializer &obj)
{
	cout << "[MEMORY COPY CONSTRUCTOR] Serializer has been copied." << endl;
}

void Serializer::set_word_ordered(Memory &memory, unsigned int byte_address, unsigned int value)
{
#ifdef DEBUG
	cout << "[SERIALIZER SET WORD] Writing 0x" << std::hex << value;
	cout << " to 0x" << std::hex << byte_address << "." << endl;
#endif

	*(memory.memory_array + byte_address) = value;
}

unsigned int Serializer::get_word_ordered(Memory &memory, unsigned int byte_address)
{
	unsigned int value;

	value = *(memory.memory_array + byte_address);

#ifdef DEBUG
	cout << "[SERIALIZER GET WORD] Read 0x" << std::hex << value;
	cout << " from 0x" << std::hex << byte_address << "." << endl;
#endif

	return value;
}

// NOTES:
// Although the sequenctial consistency primer does illustrate scenarios
// where SC is violated, it does not yet discuss ways to implement SC.
