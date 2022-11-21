#include <iostream>

#include "JSONSerializable.h"

class Child: public requests::JSONSerializable
{
public:
  int z;
  float x;

  Child() {}
  Child(int a, float b) :
    z(a),
    x(b)
  { }

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
  std::vector<Child> children;

protected:
  virtual void defineJsonMapping()
  {
    mapJson("hello", a);
    mapJson("world", b);
    mapJson("listA", c);
    mapJson<requests::JSONSerializable>("childClass", ch);
    mapJsonToSerializableArray("children", children);
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
  t.children.push_back(Child(43, 22));
  t.children.push_back(Child(443, 212));
  t.children.push_back(Child(34, 242));

  std::string json_out = t.marshal().dump();
  std::cout << json_out << std::endl;

  Test newTest;
  json in2 = json::parse(json_out);
  newTest.unmarshal(in2, 0);

  std::string json_out2 = newTest.marshal().dump();

  std::cout << json_out2 << std::endl;

  std::string json_out3 = newTest.marshal().dump();
  std::cout << json_out3 << std::endl;
}
