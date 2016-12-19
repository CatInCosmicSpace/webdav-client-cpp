#include <webdav/server.hpp>
#include "catch.hpp"

SCENARIO("Downloading", "download") {
	std::map<std::string, std::string> options;
	WebDAV::Server::set_options("webdav.test.travis", "webdav.test.test", options);
	std::unique_ptr<WebDAV::Client> client(WebDAV::Client::Init(options));

	WebDAV::Server::download("/tmp_dir/", "upload/test", client);

	WebDAV::Server::decrypt_and_clear((std::string)"upload/test");

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

SCENARIO("Download from 2 disks", "multidownload") {
	std::vector<std::map<std::string, std::string>> disks;
	std::map<std::string, std::string> temp;
	std::string login, password;
	WebDAV::Server::set_options("test.travis", "travis.test", temp);
	disks.push_back(temp);
	WebDAV::Server::set_options("webdav.test.travis", "webdav.test.test", temp);
	disks.push_back(temp);

	std::string download_directory = "download/";
	std::string disk_directory = "/";
	if (!(boost::filesystem::exists(boost::filesystem::complete(download_directory)))) {
		boost::filesystem::create_directories(download_directory);
	}
	chdir(boost::filesystem::system_complete(download_directory).generic_string().c_str());
	for (auto i : disks) {
		//std::cout << "Enter directories for " << i.at("webdav_login") << std::endl;
		//std::cin >> disk_directory >> download_directory;
		std::unique_ptr<WebDAV::Client> client(WebDAV::Client::Init(i));
		auto c = boost::filesystem::system_complete(i.at("webdav_login")).generic_string();
		if (!(boost::filesystem::exists(boost::filesystem::complete(i.at("webdav_login"))))) {
			boost::filesystem::create_directories(i.at("webdav_login"));
		}
		chdir(boost::filesystem::system_complete(i.at("webdav_login")).generic_string().c_str());
		if ((client->check(disk_directory))) {
			if (WebDAV::Server::is_dir(disk_directory)) {
				if (disk_directory != "/") {
					boost::filesystem::create_directory(i.at("webdav_login"));
					chdir((download_directory + i.at("webdav_login")).c_str());
				}
				WebDAV::Server::download(disk_directory, i.at("webdav_login"), client);
				//client->clean(disk_directory);
			}
			else {
				std::string path_to_download = disk_directory;
				if (disk_directory[0] == '/')
					path_to_download = disk_directory.substr(1);
				client->download(disk_directory, path_to_download);
				//client->clean(disk_directory);
			}
		}
		WebDAV::Server::decrypt_and_clear(i.at("webdav_login"));
		std::cout << "< Decrypted files for " << i.at("webdav_login") << " successfully downloaded" << std::endl;
	}

	REQUIRE(boost::filesystem::exists(boost::filesystem::system_complete("test.travis/root_file_from_test.txt")));
	REQUIRE(boost::filesystem::exists(boost::filesystem::system_complete("test.travis/dir_from_test/")));
	REQUIRE(boost::filesystem::exists(boost::filesystem::system_complete("test.travis/dir_from_test/one_more_file.txt")));
	REQUIRE(boost::filesystem::exists(boost::filesystem::system_complete("test.travis/dir_from_test/one_more_dir")));

	REQUIRE(boost::filesystem::exists(boost::filesystem::system_complete("webdav.test.travis/root_file_from_webdav.txt")));
	REQUIRE(boost::filesystem::exists(boost::filesystem::system_complete("webdav.test.travis/webdav_root_dir")));
}
