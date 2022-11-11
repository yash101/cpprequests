#ifndef CPPREQUESTS_JSONCONSTRUCTOR_H
#define CPPREQUESTS_JSONCONSTRUCTOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <functional>

#include <nlohmann/json.hpp>

#include "stringutil.h"

using nlohmann::basic_json;
using nlohmann::json;

namespace requests
{
  class JSONSerializable
  {
  public:
    const static char FIELD_NULL = 1 << 0;
    const static char FIELD_UNDEFINED = 1 << 1;
    const static char FIELD_MISSING = 1 << 2;
    const static char UNKNOWN_FIELD = 1 << 4;
    const static char VALIDATE_SHALLOW = 1 << 5;
    const static char VALIDATE_RECURSIVE = 1 << 6;

  private:

    struct _jsonSerMappingData
    {
      void* dataPointer;                                                  // Pointer to the mapped variable
      std::function<void(void* mappedReference, basic_json<>& json)> fromJson;
      std::function<json(void* mappedReference)> toJson;

      inline _jsonSerMappingData() :
        dataPointer(nullptr)
      { }
    };

    std::unordered_map<std::string, _jsonSerMappingData> _jsonSerMapping;
    std::unordered_map<std::string, char> _jsonShadowMap;                 // Contains details about the fields
    std::unordered_map<std::string, std::string> _jsonUnknownFields;

  protected:
    JSONSerializable();
    virtual ~JSONSerializable();

    inline void setJsonFieldFlag(std::string fieldname, char flags);

    template <typename FieldType>
    void mapJson(std::string field, FieldType& reference)
    {
      _jsonSerMappingData mapping;
      mapping.dataPointer = (void*) &reference;
      mapping.fromJson = [this, field](void* ref, basic_json<>& json) -> void {
        FieldType& r = *reinterpret_cast<FieldType*>(ref);
        r = json[field];
      };
      mapping.toJson = [this, field](void* ref) -> json {
        FieldType& r = *reinterpret_cast<FieldType*>(ref);
        return json(r);
      };

      _jsonSerMapping[field] = mapping;
    }

    template <typename Item>
    void mapJson(std::string field, std::vector<Item>& reference)
    {
      _jsonSerMappingData mapping;
      mapping.dataPointer = &reference;
      mapping.toJson = [this, field](void* ref) -> json {
        std::vector<Item>& r = *reinterpret_cast<std::vector<Item>*>(ref);
        return json(r);
      };
      mapping.fromJson = [this, field](void* ref, basic_json<>& json) {
      };

      _jsonSerMapping[field] = mapping;
    }

    template <typename FieldType>
    void mapJson(
      std::string field,
      FieldType& reference,
      std::function<void(FieldType&, basic_json<>&)> fromJson,
      std::function<json(FieldType&)> toJson
    )
    {
      _jsonSerMappingData mapping;
      mapping.dataPointer = &reference;
      mapping.toJson = [this, field, toJson](void* reference) -> json {
        FieldType& ref = *reinterpret_cast<FieldType*>(reference);
        return toJson(ref);
      };
      mapping.fromJson = [this, field, fromJson](void* reference, basic_json<>& json) {
        FieldType& ref = *reinterpret_cast<FieldType*>(reference);
        return fromJson(ref, json);
      };
      _jsonSerMapping[field] = mapping;
    }

  protected:
    virtual void defineJsonMapping();
  
  public:
    json marshal();
    bool unmarshal(basic_json<>& j, char flags);
  };

  template <>
  void JSONSerializable::mapJson<JSONSerializable>(std::string field, JSONSerializable& object);

  template<>
  inline void JSONSerializable::mapJson<basic_json<>>(std::string field, basic_json<>& j);
}

#endif

/*
Goals:

class JSONSerializable
{
public:
  void marshal(JSON& json);
  void unmarshal(JSON& json);

  void mapToJson(std::string fieldname, int& reference);
  void mapToJson(std::string fieldname, long& reference);
  void mapToJson(std::string fieldname, std::string& reference);
  void mapToJson(std::string fieldname, const char*& reference);
  void mapToJson(std::string fieldname, float& reference);
  void mapToJson(std::string fieldname, double& reference);
  void mapToJson(std::string fieldname, )
}

class Foo : public JSONSerializable
{
private:
  int x;
  std::string y;

public:
  Foo()
  {
    this->mapToJson("x", x);
    this->mapToJson("y", y);
  }
}

 */
