cmake_minimum_required(VERSION 3.20.0)
set(project_name dconf_app)
project(${project_name})
project(${project_name} CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif ()

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/module/")

include_directories(bdb-lib)
include_directories(dconf-lib)
include_directories(util-lib)

include(UseDoxygenDoc)
add_subdirectory(bdb-lib)
add_subdirectory(dconf-lib)
add_subdirectory(util-lib)

add_doxygen_doc(
        BUILD_DIR
        ${CMAKE_CURRENT_BINARY_DIR}/_build
        DOXY_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
        TARGET_NAME
        docs
        COMMENT
        "HTML documentation"
)
## link libs

set(BUILD_SHARED_LIBS NO)
set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
message("FOUND: ${Threads}")

# OpenSSL::SSL
# https://cmake.org/cmake/help/v3.6/module/FindOpenSSL.html
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

# https://github.com/cloudshark/cshark/blob/master/build/modules/FindJSON-C.cmake
set(JSON-C_USE_STATIC_LIBS TRUE)
find_package(JSON-C REQUIRED)

# https://github.com/sum01/FindBerkeleyDB
set(BerkeleyDB_USE_STATIC_LIBS TRUE)
find_package(BerkeleyDB REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/FindBerkeleyDB")

# ${PROJECT_NAME}
add_executable(${PROJECT_NAME} apps/${PROJECT_NAME}.cpp)

target_link_libraries(${PROJECT_NAME}
        PRIVATE dconf-lib bdb-lib util-lib
        PUBLIC Oracle::BerkeleyDB
        PUBLIC ${JSON-C_LIBRARIES}
        PUBLIC OpenSSL::SSL
        PUBLIC OpenSSL::Crypto
        PUBLIC Threads::Threads
        )

