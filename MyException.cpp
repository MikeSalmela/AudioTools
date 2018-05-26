#include "MyException.h"

MyException::MyException(string msg): msg_(msg)
{
}

string MyException::get_msg(){
  return msg_;
}
