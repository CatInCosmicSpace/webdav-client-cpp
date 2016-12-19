#include <webdav/localclient.hpp>
#include <string>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <openssl/ssl.h>
#include <boost/iostreams/device/mapped_file.hpp>

#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")

const int BUFSIZE = 1024;
#define AES_BLOCK_SIZE 256

auto WebDAV::LocalClient::check_hash(const std::string & path, const std::string & remote_path, std::unique_ptr<WebDAV::Client> & client) -> bool {
	std::stringstream ss;
	if (client->check(remote_path)) {
		client->download_to(remote_path, ss);
	}
	else {
		return false;
	}
	std::string check_local = get_string_sha256(path).substr(0, 32);
	bool check = ss.str().substr(0, 32) == check_local;
	return check;
}

auto WebDAV::LocalClient::check_local_hash(const std::string & path) -> bool {
	if (boost::filesystem::exists(path + ".sha256")) {
		FILE * input = fopen((path + ".sha256").c_str(), "rb");
		char buffer[32];
		fread(buffer, sizeof(char), 32, input);
		std::stringstream ss;
		ss << buffer;
		fclose(input);
		std::string old_hash = ss.str();

		std::string new_hash = get_string_sha256(path);
		return (old_hash.substr(0, 32) == new_hash.substr(0, 32));
	}
	else {
		get_string_sha256(path);
		return false;
	}
}

auto WebDAV::LocalClient::check_sha(std::vector<std::string> & input) -> std::vector<std::string> {
	std::vector<std::string> sha_files;
	for (auto i : input) {
		if (i.rfind(".sha") != std::string::npos)
			sha_files.push_back(i);
	}
	return sha_files;
}

auto WebDAV::LocalClient::check_not_sha(std::vector<std::string> & input) -> std::vector<std::string> {
	std::vector<std::string> sha_files;
	for (auto i : input) {
		if (i.rfind(".sha") == std::string::npos)
			sha_files.push_back(i);
	}
	return sha_files;
}

auto WebDAV::LocalClient::clear_encrypted(const std::string & path) -> void {
	for (boost::filesystem::recursive_directory_iterator dir_end, dir(path); dir != dir_end; ++dir) {
		boost::filesystem::path _path(*dir);
		if (_path.generic_string().rfind(".enc") != std::string::npos) {
			std::remove(_path.generic_string().c_str());
		}
	}
}

auto WebDAV::LocalClient::cut_path(std::string & path) -> const std::string{
	if (path[path.length() - 1] == '/' || path[path.length() - 1] == '\\')
	path.pop_back();
	size_t index = path.rfind("/");
	std::string new_path(path.substr(index + 1));
	return new_path;
}

auto WebDAV::LocalClient::encrypt(std::string name) -> std::string {
	name = boost::filesystem::system_complete(name).generic_string();
	int outlen, inlen;
	FILE * input = fopen(name.c_str(), "rb");
	FILE * output = fopen((name + ".enc").c_str(), "wb");
	unsigned char inbuf[BUFSIZE], outbuf[BUFSIZE];
	unsigned char key[32] = "testtestforaescrypttesttesttest";
	unsigned char iv[8] = "testvec";
	const EVP_CIPHER * cipher;
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	cipher = EVP_bf_ofb();
	EVP_EncryptInit(&ctx, cipher, key, iv);
	for (;;) {
		inlen = fread(inbuf, 1, BUFSIZE, input);
		if (inlen <= 0)
			break;
		EVP_EncryptUpdate(&ctx, outbuf, &outlen, inbuf, inlen);
		fwrite(outbuf, 1, outlen, output);
	}
	EVP_EncryptFinal(&ctx, outbuf, &outlen);
	fwrite(outbuf, 1, outlen, output);
	EVP_CIPHER_CTX_cleanup(&ctx);
	fclose(input);
	fclose(output);
	return name + ".enc";
}

auto WebDAV::LocalClient::get_file_sha256(std::string path) -> const std::string{
	path = boost::filesystem::system_complete(path).generic_string();
	FILE * input = fopen(path.c_str(), "rb");
	FILE * output = fopen((path + ".sha256").c_str(), "wb");
	if (!input || !output)
		throw;
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	const auto bufSize = 32768;
	auto buffer = malloc(bufSize);
	auto bytesRead = 0;
	if (!buffer)
		throw;
	while ((bytesRead = fread(buffer, 1, bufSize, input))) {
		SHA256_Update(&sha256, buffer, bytesRead);
	}
	SHA256_Final(hash, &sha256);
	fwrite(hash, sizeof(hash), 1, output);
	fclose(input);
	fclose(output);
	free(buffer);
	return (path + ".sha256");
}

auto WebDAV::LocalClient::get_string_sha256(std::string path_to_sha256) -> const std::string{
	path_to_sha256 = get_file_sha256(path_to_sha256);

	FILE * input = fopen(path_to_sha256.c_str(), "rb");
	char buffer[32];
	fread(buffer, sizeof(char), 32, input);
	std::stringstream ss;
	ss << buffer;
	fclose(input);
	return ss.str();
}

auto WebDAV::LocalClient::file_list(const std::string & path) -> std::vector<std::string> {
	boost::filesystem::path p = path;
	std::vector<boost::filesystem::path> files;
	std::copy(boost::filesystem::directory_iterator(p), boost::filesystem::directory_iterator(), std::back_inserter(files));
	std::vector<std::string> file;
	for (auto i : files) {
		if (boost::filesystem::is_regular_file(i)) {
			if (i.generic_string().rfind(".sha256") == std::string::npos) {
				file.push_back(encrypt(i.generic_string()));
				//get_file_sha256(i.generic_string());
			}
		}
		else {
			file.push_back(i.generic_string());
		}
	}
	return file;
}

auto WebDAV::LocalClient::info_to_string(const std::map<std::string, std::string> & info) -> const std::string{
	std::stringstream ss;
for (auto option : info) {
	ss << "\t" << option.first << ": " << option.second << std::endl;
}
return ss.str();
}

auto WebDAV::LocalClient::is_dir(const std::string & check) -> bool {
	return (check[check.length() - 1] == '/');
}

auto WebDAV::LocalClient::resources_to_string(const std::vector<std::string> & resources) -> const std::string{
	std::stringstream ss;
for (auto resource : resources) {
	ss << "\t" << "- " << resource << std::endl;
}
return ss.str();
}

auto WebDAV::LocalClient::set_options(const std::string & cs, const std::string & password, std::map<std::string, std::string> & options) -> void {
	options =
	{
		{ "webdav_hostname", "https://webdav.yandex.ru" },
		{ "webdav_login", cs },
		{ "webdav_password", password }
	};
}

auto WebDAV::LocalClient::upload(const std::string & local_dir, const std::string & disk_dir, std::unique_ptr<WebDAV::Client> & client) -> void {
	auto resources = file_list(local_dir);
	auto not_sha_files = check_not_sha(resources);
	auto sha_files = check_sha(resources);
	for (auto i : not_sha_files) {
		if (boost::filesystem::is_directory(i)) {
			client->create_directory(disk_dir + cut_path(i) + "/", true);
			upload(i, disk_dir + cut_path(i) + "/", client);
		}
		else {
			auto clear_file(i.substr(0, i.rfind(".enc")));
			auto clear_disk(disk_dir + cut_path(i).substr(0, cut_path(i).rfind(".enc")));
			if (!check_local_hash(clear_file)) {
				client->upload(disk_dir + cut_path(i), i);
			}
		}
	}
}
