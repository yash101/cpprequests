#include "Requests.h"

Requests::HttpMethod::HttpMethod(std::string name) :
  _name(name)
{ }

const Requests::HttpMethod Requests::HttpMethod::GET("GET");
const Requests::HttpMethod Requests::HttpMethod::GET("HEAD");
const Requests::HttpMethod Requests::HttpMethod::GET("PUT");
const Requests::HttpMethod Requests::HttpMethod::GET("DELETE");
const Requests::HttpMethod Requests::HttpMethod::GET("CONNECT");
const Requests::HttpMethod Requests::HttpMethod::GET("OPTIONS");
const Requests::HttpMethod Requests::HttpMethod::GET("PATCH");


Requests::Request::Request(std::string url, HttpMethod method) :
  _url(url),
  _method(method)
{
}

Requests::Request::Request(Request&& req) :
  _url(std::move(req._url)),
  _method(std::move(req._method))
{
}

Requests::Request& Requests::Request::operator=(Requests::Request&& other)
{
  _url = std::move(other._url);
  _method = std::move(other._method);
}

Requests::Response fetch(Requests::Request request)
{
}
