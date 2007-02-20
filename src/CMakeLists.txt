# depends
set(SDL_CONFIG "sdl-config" CACHE STRING "Path to sdl-config script")

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)

if(NOT SDL_FOUND)
	MESSAGE(FATAL_ERROR "Could not find SDL library")
endif(NOT SDL_FOUND)

if(NOT SDLIMAGE_FOUND)
	MESSAGE(FATAL_ERROR "Could not find SDL_image library")
endif(NOT SDLIMAGE_FOUND)

find_path(SDLGFX_INCLUDE_DIR NAMES SDL_rotozoom.h PATHS ${SDLIMAGE_INCLUDE_DIR})
find_library(SDLGFX_LIBRARY NAMES SDL_gfx)

if(NOT SDLGFX_INCLUDE_DIR OR NOT SDLGFX_LIBRARY)
	MESSAGE(FATAL_ERROR "Could not find SDL_gfx library")
endif(NOT SDLGFX_INCLUDE_DIR OR NOT SDLGFX_LIBRARY)

# defs
add_definitions(-DDATADIR="\\"${CMAKE_INSTALL_PREFIX}/${DATADIR}\\"")
add_definitions(`${SDL_CONFIG} --cflags`)

# sources
file(GLOB zenitka_SRCS *.c)

# targets
add_executable(zenitka ${zenitka_SRCS})
target_link_libraries(zenitka `${SDL_CONFIG} --libs` ${SDLIMAGE_LIBRARY} ${SDLGFX_LIBRARY})

if(WITH_INSTALL)
	install(PROGRAMS zenitka DESTINATION ${BINDIR})
endif(WITH_INSTALL)
