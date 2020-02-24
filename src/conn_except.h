#pragma once
#ifndef CONN_EXCEPT_H_
#define CONN_EXCEPT_H_

#include <exception>
#include <string>
using namespace std;

class conn_except : public exception {
	const string msg;
public :
	conn_except(const string& , const char*);

	const char* what() const noexcept final override { return msg.c_str();};
};

#endif /* CONN_EXCEPT_H_ */
