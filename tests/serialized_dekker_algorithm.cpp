/* 
 * Name: serialized_dekker_algorithm.cpp
 * Title: Simulates a serializer that ensures program order,
 * to prevent race in the dekker's algorithm using a two masters
 * Author: Vignesh Suresh
 * Date: May 30, 2020
 *
 */

#include <iostream>
#include <serializer.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// A struct that contains the memory object and serializer object.
// This is because we can only pass one argument to a pthread_create.
// We have stored only the pointers to the two datatypes instead of the
// data itself.
struct mem_ser_package
{
	Memory *mem;
	Serializer *ser;
};

// Using the entire std library to avoid importing libraries individually
using namespace std;

// The pthread function expects the argument to be a void type pointer.
// Once inside the function, we can typecast the pointer to the struct
// or class type that we want
void *fn_a(void *arg)
{
	struct mem_ser_package *mem_ser = (struct mem_ser_package *) arg;
	
	while(mem_ser->ser->get_word_ordered(*(mem_ser->mem), 0x3) != 0x1);

	mem_ser->ser->set_word_ordered(*(mem_ser->mem), 0x1, 0x1);

	if(mem_ser->ser->get_word_ordered(*(mem_ser->mem), 0x2) != 0x1)
	{
		mem_ser->ser->set_word_ordered(*(mem_ser->mem), 0x0, 0x1);
	}
	
	pthread_exit(NULL);
}

void *fn_b(void *arg)
{
	struct mem_ser_package *mem_ser = (struct mem_ser_package *) arg;
	
	while(mem_ser->ser->get_word_ordered(*(mem_ser->mem), 0x3) != 0x1);

	mem_ser->ser->set_word_ordered(*(mem_ser->mem), 0x2, 0x1);

	if(mem_ser->ser->get_word_ordered(*(mem_ser->mem), 0x1) != 0x1)
	{
		mem_ser->ser->set_word_ordered(*(mem_ser->mem), 0x0, 0x2);
	}	
	
	pthread_exit(NULL);
}

int main(void)
{
	cout << "[MAIN] Starting the main code!" << endl;

	int count_1 = 0, count_2 = 0;

	while(1)
	{
		Memory mem;
		Serializer ser;

#ifdef DEBUG
		cout << "[MAIN] First 4 elements of memory before algorithm:" << endl;

		for(unsigned int i = 0; i < 4; i++)
		{
			cout << "0x" << std::hex << mem.memory_array[i] << endl;
		}
#endif

		// Two processes proc_a and proc_b that simulate two parallel running
		// threads in a multiprocessor system.
		pthread_t proc_a, proc_b;
		pthread_attr_t attr;
		int err;
		void *status;
		struct mem_ser_package mem_ser;

#ifdef DEBUG
		cout << "[MAIN] Created mem_ser struct." << endl;
#endif

		mem_ser.mem = &mem;
		mem_ser.ser = &ser;

		// Configure that the two threads can be joined back by the calling
		// thread.
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		// As observed, the order in which is affecting the number of race
		// resolutions for either proc_a or proc_b.
		err = pthread_create(&proc_a, &attr, fn_a, (void *)&mem_ser);
		err = pthread_create(&proc_b, &attr, fn_b, (void *)&mem_ser);
		
		srand(time(NULL));

		// A random delay before starting the two threads
		usleep(rand()%50 + 50);

		// A mailbox write from the calling thread to start the other two
		// threads.
		mem.set_word_direct(0x3, 0x1);

		err = pthread_join(proc_a, &status);
		err = pthread_join(proc_b, &status);

#ifdef DEBUG
		cout << "[MAIN] First 3 elements of memory after algorithm:" << endl;

		for(unsigned int i = 0; i < 4; i++)
		{
			cout << "0x" << std::hex << mem.memory_array[i] << endl;
		}
#endif

		// Seeing which thread won the data race.
		if(mem.get_word_direct(0x0) == 0x1)
		{
			count_1++;
		}
		else if(mem.get_word_direct(0x0) == 0x2)
		{
			count_2++;
		}

		cout << "Count 1 = " << count_1 << " Count 2 = " << count_2 << endl;

		usleep(rand()%50 + 50);

#ifdef DEBUG
		break;
#endif
	}

	cout << "[MAIN] Ending the main code!" << endl;

	return 0;
}
