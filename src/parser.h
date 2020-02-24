#pragma once
#ifndef PARSER_H_
#define PARSER_H_ 

#include <list>
#include <netinet/in.h>
#include <string>
#include <fstream>

struct host {
	std::string name;
	sockaddr_in addr;
	bool up{false};
	bool first{true};
	void print_state_change(std::ofstream& logfile);
};

std::list<std::shared_ptr<host>> parse_hosts(char* filename);

#endif /* PARSER_H */
