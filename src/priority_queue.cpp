#include "priority_queue.hpp"
#include <algorithm>

using namespace PRIOR_QUEUE;

//////////////////////////////////////////////////////////////////////////////////
/// factory method
//////////////////////////////////////////////////////////////////////////////////
p_Prior_queue_t Priority_queue::create_priority_queue(callback_t wm_callback = nullptr,
   uint8_t low_wm_thrsh = 10,
   uint8_t hi_wm_thrsh = 90,
   uint16_t buff_size = 100)
{

   p_Prior_queue_t instance(new Priority_queue(wm_callback, low_wm_thrsh, hi_wm_thrsh, buff_size));

   return instance;
}

//////////////////////////////////////////////////////////////////////////////////
/// constructor
//////////////////////////////////////////////////////////////////////////////////
Priority_queue::Priority_queue(callback_t wm_callback,
   uint8_t low_wm_thrsh,
   uint8_t hi_wm_thrsh,
   uint16_t queue_size) :
   ptr_wm_callback(wm_callback),
   buffer_size(queue_size)
{
   std::cout << "Priority queue has been created" << std::endl;

   if (!set_wm_thresholds(hi_wm_thrsh, low_wm_thrsh))
   {
      std::cout << "low_wm_threshold and hi_wm_threshold set on default values 10 and 100%" << std::endl;
      low_wm_threshold = 10;
      hi_wm_threshold = 100;

   }
   std::make_heap(buffer.begin(), buffer.end());

}

//////////////////////////////////////////////////////////////////////////////////
/// destructor
//////////////////////////////////////////////////////////////////////////////////
Priority_queue::~Priority_queue()
{
}

//////////////////////////////////////////////////////////////////////////////////
/// set water mark callback function
//////////////////////////////////////////////////////////////////////////////////
bool Priority_queue::set_wm_thresholds(uint8_t hi_wm_threshold, uint8_t low_wm_threshold)
{
   if (hi_wm_threshold <= 100 && hi_wm_threshold >= 0
      && low_wm_threshold <= 100 && hi_wm_threshold >= 0
      && low_wm_threshold < hi_wm_threshold)
   {
      this->hi_wm_threshold = hi_wm_threshold;
      this->low_wm_threshold = low_wm_threshold;

      return true;
   }


   std::cout << "incorrect set of values" << std::endl;
   return false;
}

//////////////////////////////////////////////////////////////////////////////////
/// set water mark callback function
//////////////////////////////////////////////////////////////////////////////////
void Priority_queue::set_wm_callback(callback_t callback)
{
   ptr_wm_callback = callback;
}

//////////////////////////////////////////////////////////////////////////////////
/// checks whether the underlying container is empty
//////////////////////////////////////////////////////////////////////////////////
bool Priority_queue::empty()
{
	bool empty;

	/// lock mutex
	mtx.lock();
	empty=buffer.empty();
	/// unlock mutex
	mtx.unlock();

	return empty;

}

//////////////////////////////////////////////////////////////////////////////////
/// returns the number of elements
//////////////////////////////////////////////////////////////////////////////////
std::size_t Priority_queue::size()
{
	std::size_t size;
	/// lock mutex
	mtx.lock();
	size= buffer.size();
	/// unlock mutex
	mtx.unlock();
	return size;
}

//////////////////////////////////////////////////////////////////////////////////
/// inserts element and sorts the underlying container
//////////////////////////////////////////////////////////////////////////////////
void Priority_queue::push(int16_t item)
{
   /// lock mutex
   mtx.lock();
   std::cout << "PUSH " << item << " - occupied: " << buffer.size() * 100 / buffer_size << " %" << std::endl;

   /// check if threshold has been exceeded
   if ((ptr_wm_callback != nullptr) && ((buffer_size != 0) && (buffer.size() * 100 / buffer_size + 1) >= hi_wm_threshold) )
   {
      ptr_wm_callback(true);
   }

   // if exceed defined queue size, do not store the item
   if (buffer.size() < buffer_size)
   {
      buffer.push_back(item);
      std::push_heap(buffer.begin(), buffer.end());
   }
   /// unlock mutex
   mtx.unlock();
}

//////////////////////////////////////////////////////////////////////////////////
/// returns and removes the top element
//////////////////////////////////////////////////////////////////////////////////
int16_t Priority_queue::pop()
{
   int16_t tmp;

   /// lock mutex
   mtx.lock();
   std::cout << "POP - occupied: " << buffer.size() * 100 / buffer_size << " %" << std::endl;
  
	/// check if threshold has been exceeded
	if ((ptr_wm_callback != nullptr) && (buffer_size != 0) && (buffer.size() * 100 / buffer_size - 1 <= low_wm_threshold ))
	{
		ptr_wm_callback(false);
	}


	tmp = buffer.front();
	std::pop_heap(buffer.begin(), buffer.end());
	buffer.pop_back();
   

   /// unlock mutex
   mtx.unlock();

   return tmp;
}
