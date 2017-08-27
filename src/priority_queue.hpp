#pragma once


#include <mutex>
#include <vector>
#include <cstdint>
#include <thread>
#include <iostream>

namespace PRIOR_QUEUE
{
	class Priority_queue;
	typedef std::shared_ptr<Priority_queue> p_Prior_queue_t;
	typedef void (__stdcall *callback_t)(bool) ;

	class Priority_queue
	{
	public:

		/// factory method
		/// param [out] wm_callback - water mark callback function pointer
		/// param [in]  low_wm_thrsh - low water mark threshold 0 - 100%
		/// param [in]  hi_wm_thrsh  - high water mark threshold 0 - 100%
		/// param [in]  queue_size    - queue size
		static p_Prior_queue_t create_priority_queue( callback_t wm_callback,
													  uint8_t low_wm_thrsh,
													  uint8_t hi_wm_thrsh,
													  uint16_t queue_size );

		/// destructor
		~Priority_queue();

		/// checks whether the underlying container is empty
		bool empty();

		/// returns the number of elements
		std::size_t size();

		/// inserts element and sorts the underlying container
		/// param [in]  item - item to be added to queue
		void push(int16_t item);

		/// returns and removes the top element
      int16_t pop();

		/// set callback function for high water mark
		/// param [out] callback - water mark callback function pointer
		void set_wm_callback(callback_t callback);

		/// set thresholds for high and low water mark
	   /// param [in]  low_wm_thrsh - low water mark threshold 0 - 100%
		/// param [in]  hi_wm_thrsh  - high water mark threshold 0 - 100%
		bool set_wm_thresholds(uint8_t hi_wm_threshold, uint8_t low_wm_threshold);

	private:
		/// constructor
		Priority_queue(	callback_t wm_callback,
						uint8_t low_wm_thrsh,
						uint8_t hi_wm_thrsh,
						uint16_t buff_size);


		std::vector<int16_t> buffer;
		std::size_t buffer_size;
		std::mutex  mtx;
		callback_t ptr_wm_callback;
		uint8_t low_wm_threshold;
		uint8_t hi_wm_threshold;


	};


}



