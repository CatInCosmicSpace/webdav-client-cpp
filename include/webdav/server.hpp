#ifndef SERVER_HPP
#define SERVER_HPP

#pragma once
#include <webdav/client.hpp>
#include "thread_pool.hpp"
#include <memory>
#include <boost/filesystem/operations.hpp>
#include <boost/thread.hpp>

namespace WebDAV {
	namespace Server {
		auto check_not_sha(std::vector<std::string> &) -> std::vector<std::string>;
		auto check_sha(std::vector<std::string> &) -> std::vector<std::string>;
		auto decrypt(std::string & name) -> const std::string;
		auto decrypt_and_clear(std::string path) -> void;
		auto download(const std::string & dir, const std::string & down_dir, std::unique_ptr<WebDAV::Client> & client) -> void;
		auto file_list(const std::string & path)->std::vector<std::string>;
		auto info_to_string(const std::map<std::string, std::string> & info) -> const std::string;
		auto is_dir(const std::string & check) -> bool;
		auto resources_to_string(const std::vector<std::string> & resources) -> const std::string;
		auto set_options(const std::string & cs, const std::string & password, std::map<std::string, std::string> & options) -> void;

		//extern threadPool pool;
	}
}

#endif
