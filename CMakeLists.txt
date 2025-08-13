cmake_minimum_required(VERSION 3.12)
project(CryptoTracker)

set(CMAKE_CXX_STANDARD 17)


add_library(httplib INTERFACE)
target_include_directories(httplib INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/httplib
)

add_library(crypto INTERFACE)
target_include_directories(crypto INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/crypto
)

find_package(nlohmann_json 3.2.0 REQUIRED)

find_package(OpenSSL REQUIRED)

add_executable(crypto_tracker src/main.cpp)

target_link_libraries(crypto_tracker
    PRIVATE
    httplib
    crypto
    nlohmann_json::nlohmann_json
    OpenSSL::SSL
    OpenSSL::Crypto
)
