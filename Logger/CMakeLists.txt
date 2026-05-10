add_library(Logger SHARED Logger.cpp Logger.h) # Shared:".dll"; Static:".lib" //Что я хочу создать

target_compile_definitions(Logger PRIVATE LOGGER_EXPORTS) # Private: "только библиотеке", Public: "библиотеке и пользователям", Interface: "только пользователям"

target_include_directories(Logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(Logger PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreadedDLL") # флаг /MD
