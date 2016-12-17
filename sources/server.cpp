#include "server.hpp"
#include <sstream>
#include <boost/bind/protect.hpp>
#include <unistd.h>
#include <openssl/evp.h>
#define BUFSIZE 1024

threadPool pool;

auto WebDAV::Server::decrypt_and_clear(std::string & path) -> void {
	auto list = file_list(path);
	for (auto i : list) {
		if (i.rfind(".enc") != std::string::npos && boost::filesystem::is_regular_file(i)) {
			pool.addWork(&decrypt, i);
		}
	}
	pool.start(8);
	pool.join();
	for (auto i : list) 
		std::remove(i.c_str());
}

auto WebDAV::Server::check_not_sha(std::vector<std::string> & input) -> std::vector<std::string> {
	std::vector<std::string> sha_files;
	for (auto i : input) {
		if (i.rfind(".sha") == std::string::npos)
			sha_files.push_back(i);
	}
	return sha_files;
}

auto WebDAV::Server::check_sha(std::vector<std::string> & input) -> std::vector<std::string> {
	std::vector<std::string> sha_files;
	for (auto i : input) {
		if (i.rfind(".sha") != std::string::npos)
			sha_files.push_back(i);
	}
	return sha_files;
}

auto WebDAV::Server::decrypt(std::string & name) -> const std::string {
	name = boost::filesystem::system_complete(name).generic_string();
	auto new_name(name.substr(0, name.rfind(".enc")));
	int outlen, inlen;
	FILE * input;
	fopen_s(&input, name.c_str(), "rb");
	FILE * output;
	fopen_s(&output, new_name.c_str(), "wb");
	unsigned char inbuf[BUFSIZE], outbuf[BUFSIZE];
	unsigned char key[32] = "testtestforaescrypttesttesttest";
	unsigned char iv[8] = "testvec";
	const EVP_CIPHER * cipher;
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	cipher = EVP_bf_ofb(); 
	EVP_DecryptInit(&ctx, cipher, key, iv);
	for (;;) {
		inlen = fread(inbuf, 1, BUFSIZE, input);
		if (inlen <= 0) break;
		EVP_DecryptUpdate(&ctx, outbuf, &outlen, inbuf, inlen);
		fwrite(outbuf, 1, outlen, output);
	}
	EVP_DecryptFinal(&ctx, outbuf, &outlen);
	fwrite(outbuf, 1, outlen, output);
	EVP_CIPHER_CTX_cleanup(&ctx);
	fclose(input);
	fclose(output);
	return new_name;
}

auto WebDAV::Server::download(const std::string & dir, const std::string & download_directory, std::unique_ptr<WebDAV::Client> & client) -> void {
	auto resources = client->list(dir);
	auto not_sha_files = check_not_sha(resources);
	auto sha_file = check_sha(resources);
	for (auto i : not_sha_files) {
		if (WebDAV::Server::is_dir(i)) {
			boost::filesystem::create_directory(download_directory + "/" + i);
			chdir((download_directory + "/" + i).c_str());
			download(dir + i, download_directory + "/" + i.substr(0, i.length() - 1), client);
		}
		client->download(dir + i, download_directory + "/" + i);
	}
	chdir("../");
}

auto WebDAV::Server::file_list(const std::string & path) -> std::vector<std::string> {
	std::vector<std::string> files;
	for (boost::filesystem::recursive_directory_iterator dir_end, dir(path); dir != dir_end; ++dir) {
		boost::filesystem::path _path(*dir);
		files.push_back(_path.generic_string());
	}
	return files;
}

auto WebDAV::Server::info_to_string(const std::map<std::string, std::string> & info) -> const std::string {
	std::stringstream ss;
	for (auto option : info) {
		ss << "\t" << option.first << ": " << option.second << std::endl;
	}
	return ss.str();
}

auto WebDAV::Server::is_dir(const std::string & check) -> bool {
	return (check[check.length() - 1] == '/');
}

auto WebDAV::Server::resources_to_string(const std::vector<std::string> & resources) -> const std::string {
	std::stringstream ss;
	for (auto resource : resources) {
		ss << "\t" << "- " << resource << std::endl;
	}
	return ss.str();
}

auto WebDAV::Server::set_options(const std::string & cs, const std::string & password, std::map<std::string, std::string> & options) -> void {
	options =
	{
		{"webdav_hostname", "https://webdav.yandex.ru"},
		{"webdav_login", cs},
		{"webdav_password", password}
	};
}
