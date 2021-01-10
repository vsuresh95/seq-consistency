/* 
 * Name: serializer.h
 * Title: Serializer class declaration
 * Author: Vignesh Suresh
 * Date: May 30, 2020
 *
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <memory.h>

class Serializer
{
	public:
		Serializer();
		~Serializer();
		Serializer(const Serializer &);

	public:
		void set_word_ordered(Memory &, unsigned int, unsigned int);
		unsigned int get_word_ordered(Memory &, unsigned int);
};
