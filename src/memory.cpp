/* 
 * Name: memory.cpp
 * Title: Memory class definition
 * Author: Vignesh Suresh
 * Date: May 29, 2020
 *
 */

#include <memory.h>

// Using the entire std library to avoid importing libraries individually
using namespace std;

// Constructor for Memory class
Memory::Memory()
{
	// the function new returns the address that has been allocated
	// for the pointer
	memory_array = new unsigned int;

	srand(time(NULL));

	// initialize memory_array with 16 random values
	for(unsigned int i = 0; i < 16; i++)
	{
		*(memory_array + i) = rand() % 100 + 3;
	}

	cout << "[MEMORY CONSTRUCTOR] Memory has been created!" << endl;
}

// Destructor for Memory class
Memory::~Memory(void)
{
	cout << "[MEMORY DESTRUCTOR] Memory has been deleted!" << endl;
}

// Copy constructor for Memory class
Memory::Memory(const Memory &obj)
{
	memory_array = new unsigned int;
	memory_array = obj.memory_array;

	cout << "[MEMORY COPY CONSTRUCTOR] Memory has been copied." << endl;
}

void Memory::set_word_direct(unsigned int byte_address, unsigned int value)
{
#ifdef DEBUG
	cout << "[MEMORY SET WORD] Writing 0x" << std::hex << value;
	cout << " to 0x" << std::hex << byte_address << "." << endl;
#endif

	*(memory_array + byte_address) = value;
}

unsigned int Memory::get_word_direct(unsigned int byte_address)
{
	unsigned int value;

	value = *(memory_array + byte_address);

#ifdef DEBUG
	cout << "[MEMORY GET WORD] Read 0x" << std::hex << value;
	cout << " from 0x" << std::hex << byte_address << "." << endl;
#endif

	return value;
}
