cmake_minimum_required(VERSION 3.2)

set(src_files
	src/main.cpp
	src/TestCase0.cpp
	src/TestCase0.hpp
	src/TestCase2_0.cpp
	src/TestCase2_0.hpp
	src/TestCase2_1.cpp
	src/TestCase2_1.hpp
	)

set(header_files
	)

# CSaruDir must be set in your environment variables!  It should point to a
#	directory with src/bin/pkg directories (or at least src; others can be
#	created later).  Open CSaru.cmake in a text editor to see what it's doing.
#	CSaru_Lib() has been written with the intent of being broken out in this
#	file if you need finer control over your project.
include($ENV{CSaruDir}/cmake/CSaru.cmake)
# Pass the version of your project; it's not related to the CSaruEnviron.
CSaru_Bin_Console(1.5 "${src_files}" "${header_files}")

# Custom stuff hand-edited into this CMakeLists ...

set(CXX_STANDARD_REQUIRED true)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

CSaru_Depends("csaru-serialization-cpp")
CSaru_Depends("csaru-json-cpp")
CSaru_Depends("csaru-container-cpp")
CSaru_Depends("csaru-core-cpp")

