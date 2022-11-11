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
    const static char FIELD_TYPE_MISMATCH = 1 << 3;
    const static char UNKNOWN_FIELD = 1 << 4;
    const static char VALIDATE_SHALLOW = 1 << 5;
    const static char VALIDATE_RECURSIVE = 1 << 6;

  private:
    #define defineConversionFunctionDefinition(type) \
      static std::string type##ToString (void* tp); \
      static void type##FromString (void* tp, std::string& val)

    defineConversionFunctionDefinition(int8_t);
    defineConversionFunctionDefinition(int16_t);
    defineConversionFunctionDefinition(int32_t);
    defineConversionFunctionDefinition(int64_t);
    defineConversionFunctionDefinition(uint8_t);
    defineConversionFunctionDefinition(uint16_t);
    defineConversionFunctionDefinition(uint32_t);
    defineConversionFunctionDefinition(uint64_t);
    defineConversionFunctionDefinition(float);
    defineConversionFunctionDefinition(double);

    #undef defineConversionFunctionDefinition

  private:

    struct _jsonSerMappingData
    {
      void* dataPointer;                                                  // Pointer to the mapped variable
      std::function<void(void* mappedReference, basic_json<>& json)> fromJson;
      std::function<json(void* mappedReference)> toJson;

      inline _jsonSerMappingData() :
        dataPointer(nullptr)
      { }

      ~_jsonSerMappingData() = default;
    };

    std::unordered_map<std::string, _jsonSerMappingData> _jsonSerMapping;
    std::unordered_map<std::string, char> _jsonShadowMap;                 // Contains details about the fields
    std::unordered_map<std::string, std::string> _jsonUnknownFields;

  protected:
    JSONSerializable() = default;
    virtual ~JSONSerializable() = default;

    inline void setJsonFieldFlag(std::string fieldname, char flags);

    template <typename FieldType>
    void mapJson(std::string field, FieldType& reference)
    {
      _jsonSerMappingData mapping;
      mapping.dataPointer = (void*) &reference;
      mapping.fromJson = [this, field](void* ref, basic_json<>& json) -> void {
        if (!json.is_primitive())
          this->_jsonShadowMap[field] = FIELD_TYPE_MISMATCH;
        *reinterpret_cast<FieldType*>(ref) = requests::from_string<FieldType>(json.get());
      };
      mapping.toJson = [this, field](void* ref) -> json {
        return json(std::to_string(*reinterpret_cast<FieldType*>(ref)));
      };
      _jsonSerMapping[field] = mapping;
    }

    void mapToJson(std::string field, JSONSerializable& object);

    template <typename JSONSerializableBaseType>
    void mapToJson(std::string field, std::vector<JSONSerializableBaseType>& vec)
    {
      _jsonSerMappingData data;
      data.dataPointer = &vec;
      data.isVector = true;

      data.fromJsonToArray =
        [](void* mappedReference, basic_json& json) -> void
      {
        std::vector<JSONSerializableBaseType>& vec = *reinterpret_cast<std::vector<JSONSerializableBaseType>*>(mappedReference);
        vec.clear();
      };

      data.toJsonFromArray =
        [](void* mappedReference) -> json
      {
      };
    }


  protected:
    #define defineMapFunction(type) \
      void mapToJson(std::string field, type& reference); \
      void mapToJson(std::string field, std::vector<type>& reference)

    defineMapFunction(int8_t);
    defineMapFunction(int16_t);
    defineMapFunction(int32_t);
    defineMapFunction(int64_t);
    defineMapFunction(uint8_t);
    defineMapFunction(uint16_t);
    defineMapFunction(uint32_t);
    defineMapFunction(uint64_t);
    defineMapFunction(float);
    defineMapFunction(double);

    #undef defineMapFunction

  protected:
    virtual void defineJsonMapping();
  
  public:
    json marshal();
    bool unmarshal(basic_json<>& j, char flags);
  };
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
