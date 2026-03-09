cmake_minimum_required(VERSION 3.10)
project(laba_1 C)

set(CMAKE_C_STANDARD 11)                # устанавливается стандарт компиляции
set(CMAKE_C_STANDARD_REQUIRED ON)       # если стандарт будет отличаться сборка прервется

# Добавляем флаги компиляции
if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
endif()

# Проверяем наличие Unity
set(UNITY_DIR ${CMAKE_SOURCE_DIR}/test/unity)
if (NOT EXISTS ${UNITY_DIR}/unity.h OR NOT EXISTS ${UNITY_DIR}/unity.c)
    message(WARNING "Unity framework not found in ${UNITY_DIR}. Please download Unity")
endif()

# Создаем библиотеку для общих компонентов
add_library(project_lib
        type_info.h type_info.c
        utils.h utils.c
        dynamic_array.h dynamic_array.c
        console_utils.h console_utils.c
        vector3d.h vector3d.c
)

# Устанавливаем свойства библиотеки
target_include_directories(project_lib
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Добавляем основное приложение
add_executable(laba_1
        main.c
)

# Связываем основное приложение с библиотекой
target_link_libraries(laba_1 project_lib)

# Добавляем тесты
add_executable(test_laba_1
        test/test_dynamic_array.c
        ${UNITY_DIR}/unity.c
)

# Связываем тесты с библиотекой и настраиваем пути к Unity
target_link_libraries(test_laba_1 project_lib)
target_include_directories(test_laba_1 PRIVATE ${UNITY_DIR})

# Добавляем цель для запуска тестов
add_custom_target(check
        COMMAND test_laba_1
        DEPENDS test_laba_1
        COMMENT "Running tests..."
)

# Добавляем цель для удобного запуска через CTest
include(CTest)
if (BUILD_TESTING)
    add_test(NAME run_tests
            COMMAND test_laba_1
    )
endif()