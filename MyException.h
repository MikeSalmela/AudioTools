#ifndef MY_EXCEPTION_H
#define MY_EXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class MyException : public exception{
public:
  MyException(string msg);

  string get_msg();

private:
  string msg_;


};

#endif
