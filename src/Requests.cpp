#include "Requests.h"

Requests::HttpMethod::HttpMethod(std::string name) :
  _name(name)
{ }

const Requests::HttpMethod Requests::HttpMethod::GET("GET");
const Requests::HttpMethod Requests::HttpMethod::HEAD("HEAD");
const Requests::HttpMethod Requests::HttpMethod::PUT("PUT");
const Requests::HttpMethod Requests::HttpMethod::DELETE("DELETE");
const Requests::HttpMethod Requests::HttpMethod::CONNECT("CONNECT");
const Requests::HttpMethod Requests::HttpMethod::OPTIONS("OPTIONS");
const Requests::HttpMethod Requests::HttpMethod::PATCH("PATCH");


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
  return *this;
}

Requests::Response fetch(Requests::Request request)
{
}
