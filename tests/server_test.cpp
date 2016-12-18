#include <webdav/server.hpp>
#include "catch.hpp"

SCENARIO("Downloading", "download") {
	std::map<std::string, std::string> options;
	WebDAV::Server::set_options("webdav.test.travis", "webdav.test.test", options);
	std::unique_ptr<WebDAV::Client> client(WebDAV::Client::Init(options));
	boost::filesystem::create_directories("download");
	chdir((boost::filesystem::current_path().generic_string() + "/" + "download").c_str());
	WebDAV::Server::download("/tmp_dir/", "download", client);
	const std::string path = "download";
	WebDAV::Server::decrypt_and_clear(path);
	REQUIRE(boost::filesystem::exists(boost::filesystem::system_complete("download/1.txt")));
	REQUIRE(boost::filesystem::is_regular_file(boost::filesystem::system_complete("download/2.txt")));
	REQUIRE(boost::filesystem::is_regular_file(boost::filesystem::system_complete("download/test/3.txt")));
	REQUIRE(boost::filesystem::is_regular_file(boost::filesystem::system_complete("download/test/5.txt")));
	REQUIRE(boost::filesystem::is_regular_file(boost::filesystem::system_complete("download/test/test1/3.txt")));
	REQUIRE(boost::filesystem::is_regular_file(boost::filesystem::system_complete("download/test/test1/4.txt")));
	REQUIRE(boost::filesystem::is_regular_file(boost::filesystem::system_complete("download/test/test2/4.txt")));
	REQUIRE(boost::filesystem::is_regular_file(boost::filesystem::system_complete("download/test/test2/5.txt")));
}
