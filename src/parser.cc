#include <fstream>
#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <functional>
#include <chrono>
#include <iomanip>
#include <memory>
#include <fstream>

#include "parser.h"

void host::print_state_change(std::ofstream& logfile) {
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	std::cout << std::put_time(std::localtime(&in_time_t), "%d-%m-%Y %X.") 
		<< std::setfill('0') << std::setw(3) << ms.count() << " "
	       	<< name << (up ? " up" : " down") << '\n';
	if (logfile.is_open()) {
		logfile << std::put_time(std::localtime(&in_time_t), "%d-%m-%Y %X.") 
			<< std::setfill('0') << std::setw(3) << ms.count() << " "
	       		<< name << (up ? " up" : " down") << '\n';
		logfile.flush();
	}
}	

std::list<std::shared_ptr<host>> parse_hosts(char* filename) {
	std::list<std::shared_ptr<host>> hosts;
	std::ifstream config(filename);
	if (config.bad()) {
		return hosts;
	}
	
	for(std::string line; std::getline(config, line);) {
		struct sockaddr_in addr;
		auto peer = std::make_shared<host>();
		std::swap(peer->name, line);

		peer->addr.sin_family = AF_INET;
		peer->addr.sin_port = 0;

		std::string addr_s;
		if (std::size_t n; (n = peer->name.find(":")) != std::string::npos) {
			addr_s = peer->name.substr(0, n);
			auto port = peer->name.substr(n+1);
			peer->addr.sin_port = strtol(port.data(), 0, 10);
			peer->addr.sin_port = htons(peer->addr.sin_port);
		} else {
			addr_s = peer->name;
		}

		if (!inet_pton(AF_INET, addr_s.data(), &peer->addr.sin_addr)) {
			continue;
		}
		hosts.push_back(std::move(peer));
	}

	return hosts;
}


