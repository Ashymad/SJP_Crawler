cmake_minimum_required(VERSION 2.8.12) # first version with add_compile_options()
project(SJP_Crawler)

add_subdirectory(SQLiteCpp)

set(SOURCE_FILES 
        src/main.cpp
        src/IconvString.cpp
        src/IconvString.h
        src/cURLCpp.cpp
        src/cURLCpp.h
        src/SJPCrawler.cpp
        src/SJPCrawler.h
        src/stringhelpers.cpp
        src/stringhelpers.h
)

add_executable(SJP_Crawler ${SOURCE_FILES})

if(UNIX AND NOT APPLE)
   # Linux
   target_link_libraries(SJP_Crawler SQLiteCpp pthread dl)
elseif(UNIX AND APPLE)
   # Mac OS
   target_link_libraries(SJP_Crawler SQLiteCpp pthread)
else()
   # Windows
   target_link_libraries(SJP_Crawler SQLiteCpp)
endif()

target_link_libraries(SJP_Crawler 
        sqlite3
        curl
)
