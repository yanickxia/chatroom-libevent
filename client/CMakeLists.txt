if (DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    message(" [INFO] VCPKG CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif ()

include_directories(/usr/local/include)


find_package(Libevent CONFIG REQUIRED)
add_executable(client main.c)
target_link_libraries(client PRIVATE event_static event_core_static event_extra_static event_openssl_static)
