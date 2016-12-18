#include <webdav/server.hpp>
#include <catch.hpp>

SCENARIO("Uploading", "upload") {
	std::map<std::string, std::string> options;
	WebDAV::Server::set_options("webdav.test.travis", "webdav.test.test", options);
	std::unique_ptr<WebDAV::Client> client(WebDAV::Client::Init(options));

	WebDAV::Server::download("/tmp_dir/", "upload/test", client);

	WebDAV::Server::decrypt_and_clear((std::string)"upload/test");

	REQUIRE(boost::filesystem::exists("upload/1.txt"));
	REQUIRE(boost::filesystem::exists("upload/2.txt"));
	REQUIRE(boost::filesystem::exists("upload/test/3.txt"));
	REQUIRE(boost::filesystem::exists("upload/test/5.txt"));
	REQUIRE(boost::filesystem::exists("upload/test/test1/3.txt"));
	REQUIRE(boost::filesystem::exists("upload/test/test1/4.txt"));
	REQUIRE(boost::filesystem::exists("upload/test/test2/4.txt"));
	REQUIRE(boost::filesystem::exists("upload/test/test2/5.txt"));

	REQUIRE(!boost::filesystem::exists("upload/1.txt.sha256"));
	REQUIRE(!boost::filesystem::exists("upload/2.txt.sha256"));
	REQUIRE(!boost::filesystem::exists("upload/test/3.txt.sha256"));
	REQUIRE(!boost::filesystem::exists("upload/test/5.txt.sha256"));
	REQUIRE(!boost::filesystem::exists("upload/test/test1/3.txt.sha256"));
	REQUIRE(!boost::filesystem::exists("upload/test/test1/4.txt.sha256"));
	REQUIRE(!boost::filesystem::exists("upload/test/test2/4.txt.sha256"));
	REQUIRE(!boost::filesystem::exists("upload/test/test2/5.txt.sha256"));

	REQUIRE(!boost::filesystem::exists("upload/1.txt.enc"));
	REQUIRE(!boost::filesystem::exists("upload/2.txt.enc"));
	REQUIRE(!boost::filesystem::exists("upload/test/3.txt.enc"));
	REQUIRE(!boost::filesystem::exists("upload/test/5.txt.enc"));
	REQUIRE(!boost::filesystem::exists("upload/test/test1/3.txt.enc"));
	REQUIRE(!boost::filesystem::exists("upload/test/test1/4.txt.enc"));
	REQUIRE(!boost::filesystem::exists("upload/test/test2/4.txt.enc"));
	REQUIRE(!boost::filesystem::exists("upload/test/test2/5.txt.enc"));
}
