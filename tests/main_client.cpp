//#include "client.hpp"
//#include <memory>
//#include <string>
//#include <cstdlib>
//#include <stdexcept>
//#include <direct.h> 
//#include <boost/filesystem.hpp>
//#include <boost/filesystem/operations.hpp>
//#include <boost/filesystem/path.hpp>
//#include "localclient.hpp"
//
//#pragma comment(lib, "libcurl.lib")
//
//int main(int argc, char * argv[]) {
//	std::ifstream config("config");
//	std::string login, password;
//	config >> login >> password;
//
//	//argv[0] - current path, where the programm runs
//	std::string upload_path = static_cast<std::string>(argv[1]);
//	std::string disk_directory = static_cast<std::string>(argv[2]);
//	//options to init client
//	std::map<std::string, std::string> options;
//	WebDAV::LocalClient::set_options(login, password, options);
//	//init client
//	std::unique_ptr<WebDAV::Client> client(WebDAV::Client::Init(options));
//	
//	if (boost::filesystem::is_directory(upload_path)) {
//		_chdir(upload_path.c_str());
//		//std::string cut = WebDAV::LocalClient::cut_path(upload_path);
//		//client->create_directory(WebDAV::LocalClient::cut_path(upload_path));
//		//if (!(client->check(WebDAV::LocalClient::cut_path(upload_path))))
//		//	client->create_directory(WebDAV::LocalClient::cut_path(upload_path), true);
//		if (!client->check(disk_directory))
//			client->create_directory(disk_directory, true);
//		WebDAV::LocalClient::upload(upload_path, disk_directory/* + WebDAV::LocalClient::cut_path(upload_path)*/, client);
//	}
//	else {
//		if (client->check(disk_directory)) {
//			std::string cut = WebDAV::LocalClient::cut_path(upload_path);
//			client->upload(disk_directory + WebDAV::LocalClient::cut_path(upload_path), upload_path);
//		}
//	}
//	WebDAV::LocalClient::clear_encrypted(upload_path);
//	WebDAV::LocalClient::clear_hashes(upload_path);
//	return 0;
//}