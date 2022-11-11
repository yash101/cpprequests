#include "JSONSerializable.h"

#define defineConversionFunctionDefinition(type) \
  std::string requests::JSONSerializable::type##ToString(void* ptr) \
  { \
    return std::to_string(*reinterpret_cast<type*>(ptr)); \
  } \
  \
  void requests::JSONSerializable::type##FromString(void* ptr, std::string& val) \
  { \
    *reinterpret_cast<type*>(ptr) = requests::from_string<type>(val); \
  }

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

