#include <iostream>
#include <algorithm>
#include <memory>
#include <chrono>
#include <thread>
#include <unordered_set>
#include <sstream>
#include <cstring>
#include <fstream>
using namespace std;

#include <arpa/inet.h>
#include <signal.h>

#include "parser.h" 

#include "nc.h"

#include "conn_except.h"

auto wake = chrono::steady_clock::now();

void 
sigHandler(int signo){
	switch(signo){
	case SIGTERM:
	case SIGINT: {
		exit(0);
	} break;
	default: {
	} break;
	}
}

int main(int argc, char** argv) {
	if (argc < 3) {
		std::cout << argv[0] << " hosts log \n";
		return 0;
	}
	
	auto peers = parse_hosts(argv[1]);
	if (peers.empty()) {
		return 0;
	}

	ofstream logfile;
	logfile.open(argv[2], ios::out | ios::app);

	in_addr myaddr;
        memset(&myaddr, 0, sizeof(myaddr));

	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);
	for(;;) {
		auto before = chrono::steady_clock::now();

		nc(myaddr, peers, logfile);
	}


	return 0;
}
