cmake_minimum_required(VERSION 3.10) 
project(Worksheet5_Installer)
if (WIN32)
set (CPACKGENERATOR ”NSIS”)
else ()
set (CPACKGENERATOR ”ZIP”)
 endif ()

 include(CPack)

 add_library (maths adder.cpp)

add_executable ( calc calc.cpp)
 target_link_libraries ( calc maths)

 install ( TARGETS calc maths
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static )