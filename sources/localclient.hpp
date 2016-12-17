#pragma once
#include <webdav/client.hpp>
#include <memory>
#include <boost/filesystem/path.hpp>

using ustring = std::basic_string<unsigned char>;

namespace WebDAV {
	namespace LocalClient {
		auto check_hash(const std::string & path, const std::string & remote_path, std::unique_ptr<WebDAV::Client> & client) -> bool;
		auto check_sha(std::vector<std::string> &) -> std::vector<std::string>;
		auto check_not_sha(std::vector<std::string> &) -> std::vector<std::string>;
		auto clear_encrypted(const std::string & path) -> void;
		auto clear_hashes(const std::string & path) -> void;
		auto cut_path(std::string & path) -> const std::string;
		auto encrypt(std::string name) -> std::string;
		auto get_file_sha256(std::string path) -> const std::string;
		auto get_string_sha256(std::string path) -> const std::string;
		auto info_to_string(const std::map<std::string, std::string> & info) -> const std::string;
		auto is_dir(const std::string & check) -> bool;
		auto file_list(const std::string & path) -> std::vector<std::string>;
		auto resources_to_string(const std::vector<std::string> & resources) -> const std::string;
		auto set_options(const std::string & cs, const std::string & password, std::map<std::string, std::string> & options) -> void;
		auto upload(const std::string & local_dir, const std::string & disk_dir, std::unique_ptr<WebDAV::Client> & client) -> void;
	}
}
