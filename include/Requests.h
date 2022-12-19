#ifndef REQUESTS_REQUESTS_H
#define REQUESTS_REQUESTS_H

#include <string>
#include <vector>
#include <tuple>
#include <ostream>
#include <mutex>

namespace Requests
{
  class DataStream
  {
  private:
    std::vector<std::string> _buffer;
    size_t _length;
    size_t _bufferSize;
    bool _open;

    // for thread safety
    std::mutex _mut;

  public:
    DataStream();
    DataStream(std::string str);

    void put(std::string data);
    void put(const char* data, size_t length);

    size_t get(char* buffer, size_t maxlen, bool fill);
    void get(std::string& buffer, size_t maxlen, bool fill);

    std::string toString();

    inline size_t size()
    {
      std::unique_lock<std::mutex>(_mut);
      return _length;
    }

    inline bool isOpen()
    {
      std::unique_lock<std::mutex>(_mut);
      return _open;
    }

    void close()
    {
      std::unique_lock<std::mutex>(_mut);
      _open = false;
    }
  };

  class HttpMethod
  {
  private:
    std::string _name;
  public:
    HttpMethod(std::string name);

    static const HttpMethod GET;
    static const HttpMethod HEAD;
    static const HttpMethod PUT;
    static const HttpMethod DELETE;
    static const HttpMethod CONNECT;
    static const HttpMethod OPTIONS;
    static const HttpMethod PATCH;
  };

  typedef std::vector<std::pair<std::string, std::string>> Headers;

  class Response
  {
  protected:
    std::string _url;
    HttpMethod _method;
    Headers _headers;
    std::string data;

  public:
  };

  class Request
  {
  protected:
    std::string _url;
    HttpMethod _method;
    Headers _headers;
    std::string _body;
    std::string _sendFileName;

  public:
    Request(std::string url, HttpMethod method);
    Request(Request&& req);
    
    // Move / assignment operator
    Request& operator=(Request&& other);

    inline std::string& url()
    {
      return _url;
    }

    inline HttpMethod& method()
    {
      return _method;
    }

    inline Headers headers()
    {
      return _headers;
    }

    void send(std::string bytes);
    void send(const char* bytes, size_t length);
    void sendFile(std::string filename);

    // To stream lots of data
    void startStream();
    void endStream();
    void streamData(const char* bytes, size_t length);
    void streamData(std::string bytes);
  };

  Response fetch(Request request);
};

#endif
