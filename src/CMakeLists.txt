# depends
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
#find_package(SDL_gfx REQUIRED)

find_path(SDLGFX_INCLUDE_DIR NAMES SDL_rotozoom.h PATHS
	$ENV{SDLIMAGEDIR}/include
	$ENV{SDLDIR}/include
	~/Library/Frameworks/SDL_image.framework/Headers
	/Library/Frameworks/SDL_image.framework/Headers
	/usr/local/include/SDL
	/usr/include/SDL
	/usr/local/include/SDL12
	/usr/local/include/SDL11 # FreeBSD ports
	/usr/include/SDL12
	/usr/include/SDL11
	/usr/local/include
	/usr/include
	/sw/include/SDL # Fink
	/sw/include
	/opt/local/include/SDL # DarwinPorts
	/opt/local/include
	/opt/csw/include/SDL # Blastwave
	/opt/csw/include 
	/opt/include/SDL
	/opt/include
)

find_library(SDLGFX_LIBRARY NAMES SDL_gfx PATHS
	$ENV{SDLIMAGEDIR}/lib
	$ENV{SDLDIR}/lib
	/usr/local/lib
	/usr/lib
	/sw/lib
	/opt/local/lib
	/opt/csw/lib
	/opt/lib
)

if(NOT SDLGFX_INCLUDE_DIR OR NOT SDLGFX_LIBRARY)
	MESSAGE(FATAL_ERROR "Could not find SDL_gfx library ${SDLGFX_INCLUDE_DIR} ${SDLGFX_LIBRARY}")
endif(NOT SDLGFX_INCLUDE_DIR OR NOT SDLGFX_LIBRARY)

# defs
add_definitions(-DDATADIR="\\"${CMAKE_INSTALL_PREFIX}/${DATADIR}\\"")
add_definitions(`sdl-config --cflags`)

# sources
file(GLOB zenitka_SRCS *.c)

# targets
add_executable(zenitka ${zenitka_SRCS})
target_link_libraries(zenitka `sdl-config --libs` ${SDLIMAGE_LIBRARY} ${SDLGFX_LIBRARY})

if(WITH_INSTALL)
	install(PROGRAMS zenitka DESTINATION ${BINDIR})
endif(WITH_INSTALL)
