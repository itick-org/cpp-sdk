cmake_minimum_required(VERSION 3.10)
project(github-sdk)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加源文件
set(SOURCES
    src/client.cpp
    src/api_response.cpp
)

# 添加头文件
set(HEADERS
    src/client.h
    src/api_response.h
)

# 创建库
add_library(github-sdk ${SOURCES} ${HEADERS})

# 包含头文件目录
target_include_directories(github-sdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 查找依赖
find_package(CURL REQUIRED)
target_link_libraries(github-sdk PRIVATE ${CURL_LIBRARIES})
target_include_directories(github-sdk PRIVATE ${CURL_INCLUDE_DIRS})

find_package(jsoncpp REQUIRED)
target_link_libraries(github-sdk PRIVATE jsoncpp_lib)
target_include_directories(github-sdk PRIVATE ${JSONCPP_INCLUDE_DIRS})

# 添加示例
add_executable(example example/main.cpp)
target_link_libraries(example PRIVATE github-sdk)

# 添加测试
add_executable(test_client src/test_client.cpp)
target_link_libraries(test_client PRIVATE github-sdk)
