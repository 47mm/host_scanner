#pragma once
#ifndef NC_H_
#define NC_H_

#include <arpa/inet.h>

#include <list>
#include <memory>
#include <fstream>

void nc(const in_addr&, std::list<std::shared_ptr<host>>& hosts, std::ofstream& logfile);

#endif /* NC_H_ */
