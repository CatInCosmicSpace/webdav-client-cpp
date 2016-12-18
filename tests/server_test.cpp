#include <webdav/server.hpp>
#include "catch.hpp"

SCENARIO("Downloading", "download") {
	std::map<std::string, std::string> options;
	WebDAV::Server::set_options("webdav.test.travis", "webdav.test.test", options);
	std::unique_ptr<WebDAV::Client> client(WebDAV::Client::Init(options));
	boost::filesystem::create_directory("download");
	WebDAV::Server::download("/tmp_dir/", "download", client);
	const std::string path = "download";
	WebDAV::Server::decrypt_and_clear(path);

	REQUIRE(boost::filesystem::is_regular_file("download/test/1.txt"));
	REQUIRE(boost::filesystem::is_regular_file(boost::filesystem::system_complete("download/test/2.txt")));
	REQUIRE(boost::filesystem::is_regular_file(boost::filesystem::system_complete("download/test/test/3.txt")));
	REQUIRE(boost::filesystem::is_regular_file(boost::filesystem::system_complete("download/test/test/5.txt")));
	REQUIRE(boost::filesystem::is_regular_file(boost::filesystem::system_complete("download/test/test/test1/3.txt")));
	REQUIRE(boost::filesystem::is_regular_file(boost::filesystem::system_complete("download/test/test/test1/4.txt")));
	REQUIRE(boost::filesystem::is_regular_file(boost::filesystem::system_complete("download/test/test/test2/4.txt")));
	REQUIRE(boost::filesystem::is_regular_file(boost::filesystem::system_complete("download/test/test/test2/5.txt")));

	//REQUIRE(!boost::filesystem::exists("1.txt.sha256"));
	//REQUIRE(!boost::filesystem::exists("2.txt.sha256"));
	//REQUIRE(!boost::filesystem::exists("test/3.txt.sha256"));
	//REQUIRE(!boost::filesystem::exists("test/5.txt.sha256"));
	//REQUIRE(!boost::filesystem::exists("test/test1/3.txt.sha256"));
	//REQUIRE(!boost::filesystem::exists("test/test1/4.txt.sha256"));
	//REQUIRE(!boost::filesystem::exists("test/test2/4.txt.sha256"));
	//REQUIRE(!boost::filesystem::exists("test/test2/5.txt.sha256"));

	//REQUIRE(!boost::filesystem::exists("1.txt.enc"));
	//REQUIRE(!boost::filesystem::exists("2.txt.enc"));
	//REQUIRE(!boost::filesystem::exists("test/3.txt.enc"));
	//REQUIRE(!boost::filesystem::exists("test/5.txt.enc"));
	//REQUIRE(!boost::filesystem::exists("test/test1/3.txt.enc"));
	//REQUIRE(!boost::filesystem::exists("test/test1/4.txt.enc"));
	//REQUIRE(!boost::filesystem::exists("test/test2/4.txt.enc"));
	//REQUIRE(!boost::filesystem::exists("test/test2/5.txt.enc"));
}
