#ifndef _CPPREQUESTS_STRINGUTIL_H
#define _CPPREQUESTS_STRINGUTIL_H

#include <string>
#include <sstream>

namespace requests
{
  template <typename T>
  inline T from_string(std::string in)
  {
    static thread_local std::istringstream strm;
    strm.str(in);
    T ret;
    strm >> ret;
    return ret;
  }
}

#endif
