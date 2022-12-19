
# Object libraries
add_library(
  jsonserializable_obj OBJECT
    JSONSerializable.cpp
)

add_library(
  requests_obj OBJECT
    Requests.cpp
    DataStream.cpp
    $<TARGET_OBJECTS:jsonserializable_obj>
)

# Link libraries to the object libraries
target_link_libraries(jsonserializable_obj PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(jsonserializable_obj PUBLIC ${CPPREQUESTS_INCLUDE_DIR})

target_link_libraries(requests_obj PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(requests_obj PUBLIC ${CPPREQUESTS_INCLUDE_DIR})

# Static and shared libraries
add_library(jsonserializable_static STATIC $<TARGET_OBJECTS:jsonserializable_obj>)
add_library(jsonserializable SHARED $<TARGET_OBJECTS:jsonserializable_obj>)

add_library(requests_static STATIC $<TARGET_OBJECTS:requests_obj>)
add_library(requests SHARED $<TARGET_OBJECTS:requests_obj>)

# Link dependencies for the libraries
target_link_libraries(jsonserializable_static PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(jsonserializable PUBLIC nlohmann_json::nlohmann_json)

target_link_libraries(requests_static PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(requests PUBLIC nlohmann_json::nlohmann_json)

## TODO: clean up and delete this trash
# Test executable
add_executable(
  test_executable
    test_executable.cpp
)

target_link_libraries(test_executable PRIVATE jsonserializable)
target_include_directories(test_executable PUBLIC ${CPPREQUESTS_INCLUDE_DIR})
