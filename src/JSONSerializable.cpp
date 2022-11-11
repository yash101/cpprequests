#include "JSONSerializable.h"

json requests::JSONSerializable::marshal()
{
  json j;
  for (auto it = _jsonSerMapping.begin(); it != _jsonSerMapping.end(); ++it)
  {
    const auto& field = it->first;
    auto& mapping = it->second;
    
    j[field] = mapping.toJson(mapping.dataPointer);
  }

  return j;
}

bool requests::JSONSerializable::unmarshal(basic_json<>& j, char flags)
{
}

template <>
void requests::JSONSerializable::mapJson
<requests::JSONSerializable>
(std::string field, requests::JSONSerializable& object)
{
  _jsonSerMappingData mapping;
  mapping.dataPointer = &object;
  mapping.toJson = [this, field](void* ref) -> json {
    requests::JSONSerializable* r = reinterpret_cast<requests::JSONSerializable*>(ref);
    return r->marshal();
  };
  mapping.fromJson = [this, field](void* ref, basic_json<>& json) {
    requests::JSONSerializable* r = reinterpret_cast<requests::JSONSerializable*>(ref);
    r->unmarshal(json, 0);
  };
  _jsonSerMapping[field] = mapping;
}

template <>
void requests::JSONSerializable::mapJson
<basic_json<>>
(std::string field, basic_json<>& json)
{
}

class Test : public requests::JSONSerializable
{
  int a;
  float b;
  std::vector<int> c;

protected:
  virtual void defineJsonMapping()
  {
    mapJson("hello", a);
    mapJson("world", b);
    mapJson("listA", c);
  }
};
