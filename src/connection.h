#pragma once
#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <arpa/inet.h>

#include "conn_except.h"

class sock_obj {
	const int sock_;
public:
	sock_obj();
	~sock_obj();
	int sock() const { return sock_;}
};

class connection {
	const sock_obj sock{};
public:
	connection(const in_addr& );
	bool do_test() noexcept;
	bool do_connect(sockaddr_in& ) noexcept;

};

#endif /* CONNECTION_H_ */
