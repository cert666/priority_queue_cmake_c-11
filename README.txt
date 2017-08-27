************************************
* How to build project
************************************
- extract cmake.7z
- download mingw32.7z from https://drive.google.com/open?id=0B3rypyI1vg1gQWU5VmdjaU9mckk
- extract mingw32.7z
- set Visual Studio 2014 path in build.bat
- run build.bat

- run priority_queue.exe from build_MinGW folder - g++ build
- run priority_queue.exe from build_MSVC\Debug folder - Visual Studio 2014 build

************************************
* How to test queue
************************************
- application creates two producer and two consumer threads
- user can set following variables

	/// queue size
	const uint16_t QUEUE_SIZE = 100;

	/// low water mark threshold 0 - 100%
	const uint8_t LOW_WM_THRESHOLD = 10;

	/// high water mark threshold 0 - 100%
	const uint8_t HIGH_WM_THRESHOLD = 90;

	/// value which will be send on queue from thread "producer_first_foo"
	const int16_t VAL_PROD_1 = 99;
	/// value which will be send on queue from thread "producer_second_foo"
	const int16_t VAL_PROD_2 = -5;

- VAL_PROD_1 is a value which is send on queue by "producer_first_foo" thread
- VAL_PROD_2 is a value which is send on queue by "producer_second_foo" thread

- consumer_first_foo and consumer_second_foo receive values from queue in "priority" order
- at the end is compared count of VAL_PROD_1 and VAL_PROD_2 received by consumer with an 
	original set of values used by producers