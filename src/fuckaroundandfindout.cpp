#include <iostream>

#include "JSONSerializable.h"

class Child: public requests::JSONSerializable
{
public:
  int z;
  float x;

  virtual void defineJsonMapping()
  {
    mapJson("z", z);
    mapJson("x", x);
  }
};

class Test : public requests::JSONSerializable
{
public:
  int a;
  float b;
  std::vector<int> c;
  Child ch;

protected:
  virtual void defineJsonMapping()
  {
    mapJson("hello", a);
    mapJson("world", b);
    mapJson("listA", c);
    mapJson<requests::JSONSerializable>("childClass", ch);
  }
};

int main(int argc, char** argv)
{
  Test t;
  t.a = 1;
  t.b = 6.9f;
  t.c.push_back(2);
  t.c.push_back(3);
  t.ch.z = 69;
  t.ch.x = 6.9f;

  std::cout << t.marshal().dump() << std::endl;
}
