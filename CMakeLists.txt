cmake_minimum_required(VERSION 3.13)
project(openbay C)

set(CMAKE_C_STANDARD 11)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

find_package(MySQL REQUIRED)

add_executable(openbay
        src/main.c
        src/controller/login.c
        src/controller/admin.c
	    src/controller/user.c
        src/model/db.c
        src/utils/db.c
        src/utils/dotenv.c
        src/utils/io.c
        src/utils/validation.c
        src/view/login.c
        src/view/admin.c
	    src/view/user.c
)

include_directories(${MYSQL_INCLUDE_DIR})
target_link_libraries(openbay ${MYSQL_LIBRARIES})

if(MSVC)
    add_compile_options(PRIVATE /W4 /WX)
else()
    add_compile_options(PRIVATE -Wall -Wextra -Wpedantic -Werror -O2)
endif()
