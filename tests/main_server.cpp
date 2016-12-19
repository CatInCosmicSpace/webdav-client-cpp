//#include "client.hpp"
//#include <memory>
//#include <string>
//#include <stdexcept>
//#include <direct.h> 
//#include <boost/filesystem.hpp>
//#include <boost/filesystem/operations.hpp>
//#include <boost/filesystem/path.hpp>
//#include "server.hpp"
//
//#pragma comment(lib, "libcurl.lib")
//
//int main(int argc, char * argv[]) {
//	std::vector<std::map<std::string, std::string>> disks;
//	std::unique_ptr<WebDAV::Client> temp_client;
//	std::map<std::string, std::string> temp;
//	std::string login, password;
//	std::ifstream config("config_server");
//	while (config) {
//		config >> login >> password;
//		if (!config)
//			break;
//		WebDAV::Server::set_options(login, password, temp);
//		disks.push_back(temp);
//	}
//	
//	std::string download_directory = "download/";
//	std::string disk_directory = "/";
//	_chdir(boost::filesystem::system_complete(download_directory).generic_string().c_str());
//	for (auto i : disks) {
//		//std::cout << "Enter directories for " << i.at("webdav_login") << std::endl;
//		//std::cin >> disk_directory >> download_directory;
//		std::unique_ptr<WebDAV::Client> client(WebDAV::Client::Init(i));
//		auto c = boost::filesystem::system_complete(i.at("webdav_login")).generic_string();
//		if (!(boost::filesystem::exists(boost::filesystem::complete(i.at("webdav_login"))))) {
//			boost::filesystem::create_directories(i.at("webdav_login"));
//		}
//		_chdir(boost::filesystem::system_complete(i.at("webdav_login")).generic_string().c_str());
//		if ((client->check(disk_directory))) {
//			if (WebDAV::Server::is_dir(disk_directory)) {
//				if (disk_directory != "/") {
//					boost::filesystem::create_directory(i.at("webdav_login"));
//					_chdir((download_directory + i.at("webdav_login")).c_str());
//				}
//				WebDAV::Server::download(disk_directory, i.at("webdav_login"), client);
//				//client->clean(disk_directory);
//			}
//			else {
//				std::string path_to_download = disk_directory;
//				if (disk_directory[0] == '/')
//					path_to_download = disk_directory.substr(1);
//				client->download(disk_directory, path_to_download);
//				//client->clean(disk_directory);
//			}
//		}
//		WebDAV::Server::decrypt_and_clear(i.at("webdav_login"));
//		std::cout << "< Decrypted files for " << i.at("webdav_login") << " successfully downloaded" << std::endl;
//	}
//	return 0;
//}