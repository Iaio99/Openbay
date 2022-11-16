cmake_minimum_required(VERSION 3.13)
project(pms C)

set(CMAKE_C_STANDARD 11)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

find_package(MySQL REQUIRED)

add_executable(pms
        src/main.c
	src/controller/addetto_comunale.c
        src/controller/login.c
        src/controller/teacher.c
        src/controller/piscina.c
	src/controller/user.c
        src/model/db.c
        src/utils/db.c
        src/utils/dotenv.c
        src/utils/io.c
        src/utils/validation.c
	src/view/addetto_comunale.c
        src/view/piscina.c
        src/view/login.c
        src/view/teacher.c
	src/view/user.c
)

include_directories(${MYSQL_INCLUDE_DIR})
target_link_libraries(pms ${MYSQL_LIBRARIES})

if(MSVC)
    add_compile_options(PRIVATE /W4 /WX)
else()
    add_compile_options(PRIVATE -Wall -Wextra -Wpedantic -Werror -O2)
endif()
