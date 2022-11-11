add_library(
  jsonserializable_obj OBJECT
    JSONSerializable.cpp
)

target_link_libraries(jsonserializable_obj PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(jsonserializable_obj PRIVATE ${CPPREQUESTS_INCLUDE_DIR})

add_library(
  jsonserializable_static STATIC
  $<TARGET_OBJECTS:jsonserializable_obj>
)

add_library(
  jsonserializable STATIC
  $<TARGET_OBJECTS:jsonserializable_obj>
)

target_link_libraries(jsonserializable_static PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(jsonserializable PRIVATE nlohmann_json::nlohmann_json)

