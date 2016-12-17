#include "thread_pool.hpp"

threadPool::threadPool() {
}

void threadPool::start(size_t n) {
	_dummy.reset(new boost::asio::io_service::work(_ios));
	for (size_t i = 0; i < n; ++i)
		_threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &_ios));
}

void threadPool::join() {
	_dummy.reset(); //+++++++++++++++++++++++++++
	_threadpool.join_all();
}

void threadPool::stop() {
	_dummy.reset(); //+++++++++++++++++++++++++++
	_ios.stop();
	_ios.reset();
}