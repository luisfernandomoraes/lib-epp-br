cmake_minimum_required(VERSION 3.0)
project(libepp_api)

include_directories("/usr/local/include/libepp_nicbr/")

add_executable(libepp_api main.cpp)

include(FindXercesC)
find_package(XercesC REQUIRED)
include_directories( ${XercesC_INCLUDE_DIR} )

find_package(OpenSSL REQUIRED)

target_link_libraries(libepp_api /usr/local/lib/libepp-nicbr-1.18.a ${XercesC_LIBRARY} OpenSSL::SSL OpenSSL::Crypto)
set(CMAKE_CXX_STANDARD 14)


