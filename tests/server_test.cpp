#include <webdav/server.hpp>
#include "catch.hpp"

SCENARIO("Downloading", "download") {
	std::map<std::string, std::string> options;
	WebDAV::Server::set_options("webdav.test.travis", "webdav.test.test", options);
	std::unique_ptr<WebDAV::Client> client(WebDAV::Client::Init(options));

	WebDAV::Server::download("/tmp_dir/", "upload", client);
	const std::string path = "upload/test";
	WebDAV::Server::decrypt_and_clear(path);

	REQUIRE(boost::filesystem::is_regular_file("upload/test/1.txt"));
	REQUIRE(boost::filesystem::is_regular_file("upload/test/2.txt"));
	REQUIRE(boost::filesystem::is_regular_file("upload/test/test/3.txt"));
	REQUIRE(boost::filesystem::is_regular_file("upload/test/test/5.txt"));
	REQUIRE(boost::filesystem::is_regular_file("upload/test/test/test1/3.txt"));
	REQUIRE(boost::filesystem::is_regular_file("upload/test/test/test1/4.txt"));
	REQUIRE(boost::filesystem::is_regular_file("upload/test/test/test2/4.txt"));
	REQUIRE(boost::filesystem::is_regular_file("upload/test/test/test2/5.txt"));

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
