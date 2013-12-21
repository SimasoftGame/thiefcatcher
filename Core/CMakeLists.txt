
FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(SDL_image REQUIRED)
FIND_PACKAGE(SDL_gfx REQUIRED)
FIND_PACKAGE(SDL_mixer REQUIRED)
FIND_PACKAGE(SDL_ttf REQUIRED)
FIND_PACKAGE(CURL REQUIRED)
# FIND_PACKAGE(TINYXML2 REQUIRED)
# FIND_PACKAGE(sqlite3)

INCLUDE_DIRECTORIES(
	${KANGAROO_INCLUDE_DIR}
	src/
)

IF(DEFINED ENV_WEB)
	FILE(GLOB_RECURSE WTHIM_SOURCES "src/*.cpp" "src_env/web/*.cpp")
	INCLUDE_DIRECTORIES(
		src_env/web
	)
ELSE()
	FILE(GLOB_RECURSE WTHIM_SOURCES "src/*.cpp" "src_env/local/*.cpp")
	INCLUDE_DIRECTORIES(
		src_env/local
	)
ENDIF()

LINK_LIBRARIES(${SDL_LIBRARY} ${SDL_image_LIBRARY})

IF(UNIX)
	ADD_DEFINITIONS(-std=c++11 -pedantic -pedantic-errors -Wall -Werror `sdl-config --cflags` -g)
ELSE(UNIX)
	ADD_DEFINITIONS(-Wall)
ENDIF(UNIX)

SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS 1)

CMAKE_POLICY(SET CMP0003 NEW)

MESSAGE(STATUS "Compiling under ${CMAKE_SYSTEM_NAME}")

IF(WIN32)
	LINK_LIBRARIES(${KANGAROO_LIB} sqlite3 curl SDLmain SDL SDL_image SDL_gfx SDL_mixer SDL_ttf)
ENDIF()

IF(UNIX)
	EXEC_PROGRAM("sdl-config" ARGS "--libs" OUTPUT_VARIABLE SDLCONFIG_LIBS)
	LINK_LIBRARIES(${KANGAROO_LIB} ${SDLCONFIG_LIBS} sqlite3 curl SDL_image SDL_gfx SDL_mixer SDL_ttf tinyxml2 boost_system boost_filesystem boost_locale)
ENDIF()

ADD_EXECUTABLE(
	ThiefCatcher
	WIN32
	MACOSX_BUNDLE
	${WTHIM_SOURCES}
)

#SET_TARGET_PROPERTIES(ThiefCatcher PROPERTIES OUTPUT_NAME "ThiefCatcher")

SET(INSTALL_FOLDER ".")

#FILE(GLOB dataFiles "${CMAKE_CURRENT_SOURCE_DIR}/data/**/*")
SET(CPACK_SOURCE_IGNORE_FILES "/\\\\\\\\.svn/;\\\\\\\\.swp$;")

INSTALL(
	DIRECTORY data resources DESTINATION "${INSTALL_FOLDER}"
	PATTERN ".svn" EXCLUDE
)
INSTALL(FILES config.ini License.txt README.txt CREDITS.txt DESTINATION "${INSTALL_FOLDER}")

INSTALL(
	TARGETS ThiefCatcher
	RUNTIME DESTINATION "${INSTALL_FOLDER}"
	BUNDLE DESTINATION "${INSTALL_FOLDER}"
)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Thief Catcher")
SET(CPACK_PACKAGE_VENDOR "Santiago Lizardo")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
SET(CPACK_PACKAGE_EXECUTABLES "ThiefCatcher;Thief Catcher")
SET(CPACK_PACKAGE_VERSION_MAJOR ${WTHIM_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${WTHIM_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${WTHIM_VERSION_PATCH})

SET(CPACK_NSIS_HELP_LINK "http://www.santiagolizardo.com/page/thiefcatcher")
SET(CPACK_NSIS_URL_INFO_ABOUT "http://www.santiagolizardo.com/page/about")
SET(CPACK_NSIS_CONTACT "http://www.santiagolizardo.com/contact")
SET(CPACK_NSIS_MODIFY_PATH ON)

INCLUDE(CPack)
