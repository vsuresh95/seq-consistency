/* 
 * Name: dekker_algorithm.cpp
 * Title: Simulates the dekker's algorithm using a two masters
 * Author: Vignesh Suresh
 * Date: May 29, 2020
 *
 */

#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Using the entire std library to avoid importing libraries individually
using namespace std;

void *fn_a(void *arg)
{
	Memory *mem = (Memory *) arg;
	
	while(mem->get_word_direct(0x3) != 0x1);

	mem->set_word_direct(0x1, 0x1);

	if(mem->get_word_direct(0x2) != 0x1)
	{
		mem->set_word_direct(0x0, 0x1);
	}
	
	pthread_exit(NULL);
}

void *fn_b(void *arg)
{
	Memory *mem = (Memory *) arg;

	while(mem->get_word_direct(0x3) != 0x1);

	mem->set_word_direct(0x2, 0x1);

	if(mem->get_word_direct(0x1) != 0x1)
	{
		mem->set_word_direct(0x0, 0x2);
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

#ifdef DEBUG
		cout << "[MAIN] First 4 elements of memory before algorithm:" << endl;

		for(unsigned int i = 0; i < 4; i++)
		{
			cout << "0x" << std::hex << mem.memory_array[i] << endl;
		}
#endif

		pthread_t proc_a, proc_b;
		pthread_attr_t attr;
		int err;
		void *status;

		// Configure that the two threads can be joined back by the calling
		// thread.
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		// As observed, the order in which is affecting the number of race
		// resolutions for either proc_a or proc_b.
		err = pthread_create(&proc_a, &attr, fn_a, (void *)&mem);
		err = pthread_create(&proc_b, &attr, fn_b, (void *)&mem);
		
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
	}

	cout << "[MAIN] Ending the main code!" << endl;

	return 0;
}
