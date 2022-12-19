#include "Requests.h"

using namespace Requests;

using std::move;
using std::string;
using std::mutex;
using std::unique_lock;

DataStream::DataStream() :
  _length(0),
  _open(true)
{
}

DataStream::DataStream(string str) :
  _length(0),
  _open(true)
{
  unique_lock<mutex> lock(_mut);
  _buffer.push_back(move(str));
  _length += str.size();
}

void DataStream::put(std::string data)
{
  if (data.size() == 0)
    return;

  unique_lock<mutex> lock(_mut);
  _buffer.push_back(move(data));
  _length += data.size();
}

void DataStream::put(const char* data, size_t length)
{
  if (!data || length == 0)
    return;

  unique_lock<mutex> lock(_mut);
  _buffer.push_back(move(std::string(data, length)));
  _length += length;
}

size_t DataStream::get(char* buffer, size_t maxlen, bool fill)
{
  size_t fetched_length = 0;
}
