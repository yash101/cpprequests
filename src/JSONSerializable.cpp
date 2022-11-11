#include "JSONSerializable.h"
#include <stdio.h>

requests::JSONSerializable::JSONSerializable()
{
}

requests::JSONSerializable::~JSONSerializable()
{
}

void requests::JSONSerializable::defineJsonMapping()
{
}

json requests::JSONSerializable::marshal()
{
  if (_jsonSerMapping.empty())
    defineJsonMapping();
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
  for (auto it = _jsonSerMapping.begin(); it != _jsonSerMapping.end(); ++it)
  {
    const auto& field = it->first;
    auto& mapping = it->second;
    
    _jsonShadowMap[field] = 0;

    if (!j.contains(field))
    {
      _jsonShadowMap[field] = FIELD_MISSING | FIELD_UNDEFINED;
    }

    if (j[field].is_null())
    {
      _jsonShadowMap[field] = FIELD_NULL;
      continue;
    }

    mapping.fromJson(mapping.dataPointer, j[field]);
  }
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
(std::string field, basic_json<>& j)
{
  _jsonSerMappingData mapping;
  mapping.dataPointer = &j;
  mapping.toJson = [this, field](void* ref) -> json {
    basic_json<>& r = *reinterpret_cast<basic_json<>*>(ref);
    return r;
  };
  mapping.fromJson = [this, field](void* ref, basic_json<>& j) {
    basic_json<>& r = *reinterpret_cast<basic_json<>*>(ref);
    r = j;
  };
  _jsonSerMapping[field] = mapping;
}


