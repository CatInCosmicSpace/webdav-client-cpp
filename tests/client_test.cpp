#include "catch.hpp"
#include <webdav/localclient.hpp>
#include <boost/filesystem/operations.hpp>

SCENARIO("Clear encrypted and hashes", "clear_encrypted, clear_hashes") {
	std::string path = "test/";
	std::vector<std::string> files;
	for (boost::filesystem::recursive_directory_iterator dir_end, dir(path); dir != dir_end; ++dir) {
		boost::filesystem::path _path(*dir);
		files.push_back(_path.generic_string());
	}
	auto check_encrypted = true;
	for (auto i : files) {
		if (i.rfind(".enc") != std::string::npos) {
			check_encrypted = false;
		}
	}
	auto check_hashes = true;
	for (auto i : files) {
		if (i.rfind(".sha256") != std::string::npos) {
			check_hashes = false;
		}
	}

	REQUIRE(check_encrypted);
	REQUIRE(check_hashes);
}

std::string ToHex(const std::string& s, bool upper_case /* = true */) {
	std::ostringstream ret;

	for (std::string::size_type i = 0; i < s.length(); ++i)
		ret << std::setw(1) << std::hex << (upper_case ? std::uppercase : std::nouppercase) << static_cast<unsigned int>(static_cast<unsigned char>(s[i]));

	return ret.str();
}

SCENARIO("Check hash", "get_file_sha256") {
	std::string hash = "f63dba41a1978798e28ee51144d658c8";
	std::string other_hash = ToHex(WebDAV::LocalClient::get_string_sha256("test/1.txt"), false);
	//std::remove("test/1.txt.sha256");
	REQUIRE(hash == other_hash.substr(0, 32));
}

SCENARIO("Cutting path", "cut_path") {
	std::string some_path_to_file = "C:/try/test/text.txt";
	std::string other_path_to_dir = "/usr/local/test_path/";
	auto cutted_first_path = WebDAV::LocalClient::cut_path(some_path_to_file);
	auto cutted_second_file = WebDAV::LocalClient::cut_path(other_path_to_dir);

	REQUIRE(cutted_first_path == "text.txt");
	REQUIRE(cutted_second_file == "test_path");
}

SCENARIO("Options", "set_options") {
	std::map<std::string, std::string> options;
	WebDAV::LocalClient::set_options("login", "password", options);
	std::map<std::string, std::string> check = 
	{
		{"webdav_hostname", "https://webdav.yandex.ru"},
		{ "webdav_login", "login" },
		{ "webdav_password", "password" }
	};
	REQUIRE(options == check);
}

SCENARIO("Uploading", "upload") {
	std::map<std::string, std::string> options;
	WebDAV::LocalClient::set_options("webdav.test.travis", "webdav.test.test", options);
	std::unique_ptr<WebDAV::Client> client(WebDAV::Client::Init(options));

	WebDAV::LocalClient::upload("test/", "/tmp_dir/", client);

	WebDAV::LocalClient::clear_encrypted("test/");
	WebDAV::LocalClient::clear_hashes("test/");

	REQUIRE(client->check("/tmp_dir/1.txt.enc"));
	REQUIRE(client->check("/tmp_dir/2.txt.enc"));
	REQUIRE(client->check("/tmp_dir/test/3.txt.enc"));
	REQUIRE(client->check("/tmp_dir/test/5.txt.enc"));
	REQUIRE(client->check("/tmp_dir/test/test1/3.txt.enc"));
	REQUIRE(client->check("/tmp_dir/test/test1/4.txt.enc"));
	REQUIRE(client->check("/tmp_dir/test/test2/4.txt.enc"));
	REQUIRE(client->check("/tmp_dir/test/test2/5.txt.enc"));

	REQUIRE(client->check("/tmp_dir/1.txt.sha256"));
	REQUIRE(client->check("/tmp_dir/2.txt.sha256"));
	REQUIRE(client->check("/tmp_dir/test/3.txt.sha256"));
	REQUIRE(client->check("/tmp_dir/test/5.txt.sha256"));
	REQUIRE(client->check("/tmp_dir/test/test1/3.txt.sha256"));
	REQUIRE(client->check("/tmp_dir/test/test1/4.txt.sha256"));
	REQUIRE(client->check("/tmp_dir/test/test2/4.txt.sha256"));
	REQUIRE(client->check("/tmp_dir/test/test2/5.txt.sha256"));
}
