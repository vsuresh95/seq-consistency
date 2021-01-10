/* 
 * Name: memory.h
 * Title: Memory class declaration
 * Author: Vignesh Suresh
 * Date: May 29, 2020
 *
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>

class Memory
{
	public:
		Memory();
		~Memory();
		Memory(const Memory &);

	public:
		unsigned int *memory_array;

	public:
		void set_word_direct(unsigned int, unsigned int);
		unsigned int get_word_direct(unsigned int);
};
