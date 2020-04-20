if (DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    message(" [INFO] VCPKG CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif ()


set(CXX_STANDARD 11)

include_directories(/usr/local/include)

find_package(Libevent CONFIG REQUIRED)
add_executable(server_cpp main.cpp UserData.h Client.cpp Client.h uuid.h)
target_link_libraries(server_cpp PRIVATE event_static event_core_static event_extra_static event_pthreads_static)