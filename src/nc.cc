#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
using namespace std;
using namespace std::chrono_literals;

#include "parser.h"

#include "connection.h"

#include "nc.h"

void nc(const in_addr& myip, list<std::shared_ptr<host>>& hosts, std::ofstream& logfile){ 
	std::size_t window_size = hosts.size() / 10;
	if (!window_size) {
		window_size = 1;
	}
	list<std::pair<std::shared_ptr<host>, std::shared_ptr<connection>>> cnx;
	for(;;) {
		std::size_t item = 0;
		for(auto it = hosts.begin(); it != hosts.end(); ++it) {
			++ item;
			try {
				cnx.push_back(std::make_pair(*it, std::make_shared<connection>(myip)));
				cnx.back().second->do_connect((*it)->addr);
			} catch(exception& e) {
				cout<<"exception " << e.what()<<endl;
				return;
			}
			auto next = it;
			++ next;
			if ( (item == window_size) || (next == hosts.end())) {
				std::this_thread::sleep_for(1s);

				for(auto i : cnx) {
					if (bool result = i.second->do_test(); result != i.first->up || i.first->first) {
						i.first->up = result;
						i.first->print_state_change(logfile);
						i.first->first = false;
					}	
				}

				item = 0;
				{
					decltype(cnx) tmp;
					std::swap(tmp, cnx);
				}
			}
		}
	}
}
