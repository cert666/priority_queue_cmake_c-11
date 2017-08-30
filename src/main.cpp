#include "priority_queue.hpp"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <atomic>
#include <thread>
#include <iterator>  


using namespace PRIOR_QUEUE;



//////////////////////////////////////////////////////////////////////////////////
///  USER DEFINITIONS
//////////////////////////////////////////////////////////////////////////////////

/// queue size
const uint16_t QUEUE_SIZE = 100;

/// low water mark threshold 0 - 100%
const uint8_t LOW_WM_THRESHOLD = 10;

/// high water mark threshold 0 - 100%
const uint8_t HIGH_WM_THRESHOLD = 20;

/// value which will be send on queue from thread "producer_first_foo"
const int16_t VAL_PROD_1 = 99;
/// value which will be send on queue from thread "producer_second_foo"
const int16_t VAL_PROD_2 = -5;



//////////////////////////////////////////////////////////////////////////////////
///  GLOBAL VARIABLES
//////////////////////////////////////////////////////////////////////////////////
std::atomic<uint16_t> counter_ten;
std::atomic<uint16_t> counter_five;
std::atomic<bool> flag;
std::vector<int16_t> produc_1_vect(QUEUE_SIZE / 2);
std::vector<int16_t> produc_2_vect(QUEUE_SIZE / 2);
std::vector<int16_t> consum_1_vect;
std::vector<int16_t> consum_2_vect;


//////////////////////////////////////////////////////////////////////////////////
///  GLOBAL PROTOTYPES
//////////////////////////////////////////////////////////////////////////////////

/// water mark callback function
/// param [in]  pause - indicates if write thread should
///						pause writing process
inline void __stdcall water_mark_callback(bool pause)
{
	std::cout << "water_mark has been called: " << pause << std::endl;
	flag = pause;

	std::this_thread::yield();
}

/// producer task
/// param [in]  queue - pointer to queue
void producer_first_foo(p_Prior_queue_t queue);

/// producer task
/// param [in]  queue - pointer to queue
void producer_second_foo(p_Prior_queue_t queue);

/// consumer task
/// param [in]  queue - pointer to queue
void consumer_first_foo(p_Prior_queue_t queue);

/// consumer task
/// param [in]  queue - pointer to queue
void consumer_second_foo(p_Prior_queue_t queue);

/// test case 1
void test_case_1(void);

/// test case 2
void test_case_2(void);

//////////////////////////////////////////////////////////////////////////////////
/// main function
//////////////////////////////////////////////////////////////////////////////////
int main()
{

   test_case_1();
   //test_case_2();
	
	return 0;
}

/// test case 1
void test_case_1(void)
{

   uint16_t count_prod, count_consum;


   try {
      /// create queue
      p_Prior_queue_t queue = Priority_queue::create_priority_queue(nullptr,
         LOW_WM_THRESHOLD,
         HIGH_WM_THRESHOLD,
         QUEUE_SIZE);

      /// fill producer vectors	
      std::generate(produc_1_vect.begin(), produc_1_vect.end(), std::rand); // Using the C function rand()

      /// print out unsorted random values of the producer vector
      std::cout << "produc_1_vect: ";
      for (auto iv : produc_1_vect) {
         std::cout << iv << " ";
      }
      std::cout << "\n";


      /// create producer thread
      std::thread producer_first(producer_first_foo, queue);    
      /// join thread
      producer_first.join();      

      /// create consumer thread
      std::thread consumer_first(consumer_first_foo, queue);
      /// join thread
      consumer_first.join();    

    
      /// print out sorted values of the consumer vector
      std::cout << "consum_1_vect: ";
      for (auto iv : consum_1_vect) {
         std::cout << iv << " ";
      }
      std::cout << "\n";

      
   }
   catch (...)
   {
      std::cout << "something wrong" << std::endl;
   }

}

/// test case 2
void test_case_2(void)
{

   uint16_t count_prod, count_consum;


   try {
      /// create queue
      p_Prior_queue_t queue = Priority_queue::create_priority_queue(water_mark_callback,
         LOW_WM_THRESHOLD,
         HIGH_WM_THRESHOLD,
         QUEUE_SIZE);

      /// fill producer vectors	
      std::fill(produc_1_vect.begin(), produc_1_vect.end(), VAL_PROD_1);
      std::fill(produc_2_vect.begin(), produc_2_vect.end(), VAL_PROD_2);

      /// create threads
      std::thread producer_first(producer_first_foo, queue);
      std::thread producer_second(producer_second_foo, queue);
      std::thread consumer_first(consumer_first_foo, queue);
      std::thread consumer_second(consumer_second_foo, queue);

      /// datach threads
      producer_first.detach();
      producer_second.detach();
      consumer_first.detach();
      consumer_second.detach();

      std::this_thread::sleep_for(std::chrono::milliseconds(10000));

      /// merge two producer vectors
      std::vector<int16_t> produc_12_vect(produc_1_vect);
      produc_12_vect.insert(produc_12_vect.end(), produc_2_vect.begin(), produc_2_vect.end());

      std::vector<int16_t> consum_12_vect(consum_1_vect);
      consum_12_vect.insert(consum_12_vect.end(), consum_2_vect.begin(), consum_2_vect.end());


      /// count items of the value VAL_PROD_1 in producer buffer
      count_prod = std::count(produc_12_vect.begin(), produc_12_vect.end(), VAL_PROD_1);
      /// count items of the value VAL_PROD_1 in consumer buffer
      count_consum = std::count(consum_12_vect.begin(), consum_12_vect.end(), VAL_PROD_1);

      if (count_prod != count_consum)
      {
         std::cout << "count of received values: " << VAL_PROD_1 << " is different - test failed" << std::endl;
      }
      else
      {
         std::cout << "count of received values: " << VAL_PROD_1 << " is the same - test passed" << std::endl;
      }

      /// count items of the value VAL_PROD_2 in producer buffer
      count_prod = std::count(produc_12_vect.begin(), produc_12_vect.end(), VAL_PROD_2);
      /// count items of the value VAL_PROD_2 in consumer buffer
      count_consum = std::count(consum_12_vect.begin(), consum_12_vect.end(), VAL_PROD_2);

      if (count_prod != count_consum)
      {
         std::cout << "count of received values: " << VAL_PROD_2 << "  is different - test failed" << std::endl;
      }
      else
      {
         std::cout << "count of received values: " << VAL_PROD_2 << "  is the same - test passed" << std::endl;
      }
   }
   catch (...)
   {
      std::cout << "something wrong" << std::endl;
   }

}


/// producer task
/// param [in]  queue - pointer to queue
void producer_first_foo(p_Prior_queue_t queue)
{
	try {
		std::vector<int16_t>::iterator it = produc_1_vect.begin();	

		while (it!= produc_1_vect.end())
		{
			if (!flag)
			{			
				queue->push(*it);
				++it;
			}
		
		}
		std::cout << "producer_first_foo STOPED" << std::endl;
	}
	catch (...)
	{
		std::cout << "something wrong" << std::endl;
	}
}

/// producer task
/// param [in]  queue - pointer to queue
void producer_second_foo(p_Prior_queue_t queue)
{
	try {
		std::vector<int16_t>::iterator it = produc_2_vect.begin();
	
		while (it != produc_2_vect.end())
		{
			if (!flag)
			{			
				queue->push(*it);
				++it;
			}
		
		}

		std::cout << "producer_second_foo STOPED" << std::endl;
	}
	catch (...)
	{
		std::cout << "something wrong" << std::endl;
	}
}

/// consumer task
/// param [in]  queue - pointer to queue
void consumer_first_foo(p_Prior_queue_t queue)
{
	try {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		while (!queue->empty())
		{
			//if (!queue->empty()) {
			
				int16_t result = queue->pop();

				consum_1_vect.push_back(result);

				std::cout << "consumer_first_foo " << result << " " << std::endl;

		//	}
		
		}
	}
	catch (...)
	{
		std::cout << "something wrong" << std::endl;
	}
}

/// consumer task
/// param [in]  queue - pointer to queue
void consumer_second_foo(p_Prior_queue_t queue)
{
	try {
		std::this_thread::sleep_for(std::chrono::milliseconds(11));
      while (!queue->empty())
		{
            int16_t result = queue->pop();

				consum_2_vect.push_back(result);

				std::cout << "consumer_second_foo " << result << " " << std::endl;			
		
		}
	}
	catch (...)
	{
		std::cout << "something wrong" << std::endl;
	}
}