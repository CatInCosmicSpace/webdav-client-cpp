#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/bind/protect.hpp>

class threadPool {
	boost::asio::io_service _ios;
	boost::thread_group _threadpool;
	std::unique_ptr<boost::asio::io_service::work> _dummy;
public:
	threadPool();
	void start(size_t n);
	void join();
	void stop();
	template <typename T, typename ... Args>
	void addWork(T func, Args ... args);
};

template <typename T, typename ... Args>
void threadPool::addWork(T func, Args ... args) {
	auto check = boost::protect(boost::bind(func, args...));
	_ios.post(check);
}
