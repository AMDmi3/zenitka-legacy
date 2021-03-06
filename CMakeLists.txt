# project name
project(zenitka)

# checks
cmake_minimum_required(VERSION 2.4.0)

# options
option(WITH_INSTALL	"Build for systemwide installation"   OFF)
set(BINDIR	"bin" CACHE STRING "Where to install binaries under PREFIX")
set(DATADIR	"share/${PROJECT_NAME}"	CACHE STRING "Where to install data files under PREFIX")

# dirs
if(WITH_INSTALL)
	message(STATUS "Binaries will be installed into ${CMAKE_INSTALL_PREFIX}/${BINDIR}/ (-DBINDIR=${BINDIR})")
	message(STATUS "Data files will be installed into ${CMAKE_INSTALL_PREFIX}/${DATADIR}/ (-DDATADIR=${DATADIR})")
else(WITH_INSTALL)
	set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})
	set(DATADIR "data")

	message(STATUS "Building for inplace run (no installation)")
endif(WITH_INSTALL)

# defs
set(ZENITKA_VERSION "0.0.2")
add_definitions(-Wall -g -DZENITKA_VERSION="\\"${ZENITKA_VERSION}\\"")

# subdirs
subdirs(src)
subdirs(data)
