#include <stdio.h>
#include <sys/socket.h>
#include <stdint.h>
#include <errno.h>

#include <netinet/in.h>
#include <netinet/ip.h>

#include <arpa/inet.h>

#include <fcntl.h>

#include <unistd.h>

#include <poll.h>

#include <stdlib.h>

#include <memory.h>

#include <ctype.h>

#include <errno.h>

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <exception>
using namespace std;

#include "connection.h"

conn_except::conn_except(const string& who, const char* str) : msg(who + " " + string(str)) {}

sock_obj::sock_obj() : sock_(socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) {  
	// invariant: socket should be active
	if ( -1 == sock_) {
		throw conn_except(string("socket"), strerror(errno));
	}
}

sock_obj::~sock_obj() {
	close(sock_);
}

connection::connection(const in_addr& ip) {
	int flags = fcntl(sock.sock(), F_GETFL, 0);
	if ( -1 == flags) {
		throw conn_except(string("flags get"), strerror(errno));
	}

	flags |= O_NONBLOCK;
	if ( -1 == fcntl(sock.sock(), F_SETFL, flags)) {
		throw conn_except(string("flags set"), strerror(errno));
	}

	int yes=1;
	if (setsockopt(sock.sock(),SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) {
		throw conn_except(string("setsockopt"), strerror(errno));
	}

	sockaddr_in sa = {AF_INET,0,ip};

	if ( -1 == bind(sock.sock(), (struct sockaddr*)&sa, sizeof(struct sockaddr_in))) {
		throw conn_except(string("bind"), strerror(errno));
	}

}

bool
connection::do_test() noexcept {
	int ret;

	char buf[1];
	ret = send(sock.sock(), buf, sizeof(buf), 0);
	if ( ret < 0) {
		return false;
	}

	shutdown(sock.sock(), 2);

	return true;	
}

bool
connection::do_connect(sockaddr_in& sa) noexcept {
	int ret_ = connect(sock.sock(), (struct sockaddr*)&sa, sizeof(struct sockaddr_in));
	if ( (-1 == ret_) && (EINPROGRESS != errno)) {
		return false;
	}

	return true;
}

