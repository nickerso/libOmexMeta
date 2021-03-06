# raptor/_CMakeLists.txt
#
# This file allows building Raptor2 with the CMake configuration tool.
# Download CMake in source or binary form from http://www.cmake.org/
#
# Original listfile by Daniel Richard G. <skunk@iSKUNK.ORG>
# This file is in the public domain.
#

SET(RAPTOR_VERSION_MAJOR 2)
SET(RAPTOR_VERSION_MINOR 0)
SET(RAPTOR_VERSION_RELEASE 15)
SET(RAPTOR_MIN_VERSION_DECIMAL 20015)

SET(VERSION "${RAPTOR_VERSION_MAJOR}.${RAPTOR_VERSION_MINOR}.${RAPTOR_VERSION_RELEASE}")
MATH(EXPR RAPTOR_VERSION_DECIMAL "${RAPTOR_VERSION_MAJOR} * 10000 + ${RAPTOR_VERSION_MINOR} * 100 + ${RAPTOR_VERSION_RELEASE}")

PROJECT(Raptor2 C)

CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

SET(CMAKE_INCLUDE_CURRENT_DIR 1)

################################################################

# Third-party libraries

#FIND_PACKAGE(CURL)
#FIND_PACKAGE(LibXml2)
#FIND_PACKAGE(LibXslt)
#FIND_PACKAGE(YAJL REQUIRED)


if (EXISTS ${CURL_INCLUDE_DIRS})
    SET(CURL_FOUND TRUE)
endif ()
if (EXISTS ${LIBXML2_STATIC_LIBRARY})
    SET(LIBXML2_FOUND TRUE)
endif ()
if (EXISTS ${LIBXSLT_STATIC_LIBRARY})
    SET(LIBXSLT_FOUND TRUE)
endif ()
if (EXISTS ${ZLIB_STATIC_LIBRARY})
    SET(ZLIB_FOUND TRUE)
endif ()
if (EXISTS ${YAJL_STATIC_LIBRARY})
    SET(YAJL_FOUND TRUE)
endif ()

if (NOT EXISTS ${EXPAT_INCLUDE_DIRS})
    # NON ESSENTIAL, LIBXML2 USED INSTEAD
    MESSAGE(WARNING "EXPAT_INCLUDE_DIRS ${EXPAT_INCLUDE_DIRS} NOT EXIST")
ENDIF ()

if (NOT EXISTS ${LIBXML2_INCLUDE_DIRS})
    MESSAGE(FATAL_ERROR "LIBXML2_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIRS} NOT EXIST")
ENDIF ()

if (NOT EXISTS ${LIBXSLT_INCLUDE_DIRS})
    MESSAGE(FATAL_ERROR "LIBXSLT_INCLUDE_DIRS ${LIBXSLT_INCLUDE_DIRS} NOT EXIST")
ENDIF ()

if (NOT EXISTS ${ZLIB_INCLUDE_DIRS})
    MESSAGE(FATAL_ERROR "ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS} NOT EXIST")
ENDIF ()

#if (NOT EXISTS ${LIBICONV_INCLUDE_DIRS})
#    MESSAGE(FATAL_ERROR "LIBICONV_INCLUDE_DIRS ${LIBICONV_INCLUDE_DIRS} NOT EXIST")
#ENDIF()

if (NOT EXISTS ${YAJL_INCLUDE_DIRS})
    MESSAGE(FATAL_ERROR "YAJL_INCLUDE_DIRS ${YAJL_INCLUDE_DIRS} NOT EXIST")
ENDIF ()

INCLUDE_DIRECTORIES(
        #        ${CURL_INCLUDE_DIRS}
        ${EXPAT_INCLUDE_DIRS}
        ${LIBXML2_INCLUDE_DIRS}
        ${LIBXSLT_INCLUDE_DIRS}  # is this dependency actually used anywhere???
        ${ZLIB_INCLUDE_DIRS}
        ${LIBICONV_INCLUDE_DIRS}
        ${YAJL_INCLUDE_DIRS}
)

################################################################

# Configuration checks

INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFiles)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckTypeSize)
INCLUDE(CheckStructHasMember)
INCLUDE(CheckCSourceCompiles)

CHECK_INCLUDE_FILE(errno.h HAVE_ERRNO_H)
CHECK_INCLUDE_FILE(fcntl.h HAVE_FCNTL_H)
CHECK_INCLUDE_FILE(getopt.h HAVE_GETOPT_H)
CHECK_INCLUDE_FILE(limits.h HAVE_LIMITS_H)
CHECK_INCLUDE_FILE(math.h HAVE_MATH_H)
CHECK_INCLUDE_FILE(setjmp.h HAVE_SETJMP_H)
CHECK_INCLUDE_FILE(stddef.h HAVE_STDDEF_H)
CHECK_INCLUDE_FILE(stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILE(string.h HAVE_STRING_H)
CHECK_INCLUDE_FILE(unistd.h HAVE_UNISTD_H)
CHECK_INCLUDE_FILE(sys/param.h HAVE_SYS_PARAM_H)
CHECK_INCLUDE_FILE(sys/stat.h HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE(sys/stat.h HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE(sys/time.h HAVE_SYS_TIME_H)

CHECK_INCLUDE_FILES("sys/time.h;time.h" TIME_WITH_SYS_TIME)

CHECK_FUNCTION_EXISTS(access HAVE_ACCESS)
CHECK_FUNCTION_EXISTS(_access HAVE__ACCESS)
CHECK_FUNCTION_EXISTS(getopt HAVE_GETOPT)
CHECK_FUNCTION_EXISTS(getopt_long HAVE_GETOPT_LONG)
CHECK_FUNCTION_EXISTS(gettimeofday HAVE_GETTIMEOFDAY)
CHECK_FUNCTION_EXISTS(isascii HAVE_ISASCII)
CHECK_FUNCTION_EXISTS(setjmp HAVE_SETJMP)
CHECK_FUNCTION_EXISTS(snprintf HAVE_SNPRINTF)
CHECK_FUNCTION_EXISTS(_snprintf HAVE__SNPRINTF)
CHECK_FUNCTION_EXISTS(stat HAVE_STAT)
CHECK_FUNCTION_EXISTS(strcasecmp HAVE_STRCASECMP)
CHECK_FUNCTION_EXISTS(stricmp HAVE_STRICMP)
CHECK_FUNCTION_EXISTS(_stricmp HAVE__STRICMP)
CHECK_FUNCTION_EXISTS(strtok_r HAVE_STRTOK_R)
CHECK_FUNCTION_EXISTS(vasprintf HAVE_VASPRINTF)
CHECK_FUNCTION_EXISTS(vsnprintf HAVE_VSNPRINTF)
CHECK_FUNCTION_EXISTS(_vsnprintf HAVE__VSNPRINTF)

CHECK_TYPE_SIZE("unsigned char" SIZEOF_UNSIGNED_CHAR)
CHECK_TYPE_SIZE("unsigned short" SIZEOF_UNSIGNED_SHORT)
CHECK_TYPE_SIZE("unsigned int" SIZEOF_UNSIGNED_INT)
CHECK_TYPE_SIZE("unsigned long" SIZEOF_UNSIGNED_LONG)
CHECK_TYPE_SIZE("unsigned long long" SIZEOF_UNSIGNED_LONG_LONG)

IF (LIBXML2_FOUND)

    SET(CMAKE_REQUIRED_INCLUDES ${LIBXML2_INCLUDE_DIRS})
    SET(CMAKE_REQUIRED_LIBRARIES ${LIBXML2_LIBRARY})

    CHECK_FUNCTION_EXISTS(xmlCtxtUseOptions HAVE_XMLCTXTUSEOPTIONS)
    CHECK_FUNCTION_EXISTS(xmlSAX2InternalSubset HAVE_XMLSAX2INTERNALSUBSET)

    CHECK_STRUCT_HAS_MEMBER(
            xmlEntity
            name_length
            libxml/parser.h
            RAPTOR_LIBXML_ENTITY_NAME_LENGTH
    )

    CHECK_STRUCT_HAS_MEMBER(
            xmlEntity
            etype
            libxml/parser.h
            RAPTOR_LIBXML_ENTITY_ETYPE
    )

    CHECK_STRUCT_HAS_MEMBER(
            xmlSAXHandler
            initialized
            libxml/parser.h
            RAPTOR_LIBXML_XMLSAXHANDLER_INITIALIZED
    )

    CHECK_STRUCT_HAS_MEMBER(
            xmlSAXHandler
            externalSubset
            libxml/parser.h
            RAPTOR_LIBXML_XMLSAXHANDLER_EXTERNALSUBSET
    )

    CHECK_C_SOURCE_COMPILES("
#include <libxml/parser.h>
int main(void){ xmlParserOption foo; foo = XML_PARSE_NONET; return 0; }"
            RAPTOR_LIBXML_XML_PARSE_NONET
            )

    CHECK_C_SOURCE_COMPILES("
#include <libxml/HTMLparser.h>
int main(void){ htmlParserOption foo; foo = HTML_PARSE_NONET; return 0; }"
            RAPTOR_LIBXML_HTML_PARSE_NONET
            )

    SET(CMAKE_REQUIRED_INCLUDES)
    SET(CMAKE_REQUIRED_LIBRARIES)
    #    set(LIBXML2_DEFINITIONS "-ml2" "z" "pthread" "m")
    #    LIBXSLT_DEFINITIONS

ENDIF (LIBXML2_FOUND)

################################################################

# User-configurable options
#
# (Note: The CMake GUI displays these in alphabetical order, regardless of
# the order we use here)

SET(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries instead of static ones.")

# change from the original to always docs-build both static and shared
SET(RAPTOR_STATIC TRUE)

SET(RAPTOR_XML_INIT none)
IF (LIBXML2_FOUND)
    SET(RAPTOR_XML_INIT libxml)
ENDIF (LIBXML2_FOUND)

SET(RAPTOR_XML ${RAPTOR_XML_INIT} CACHE STRING "Which XML library to use (any of \"libxml\", \"none\").")

SET(RAPTOR_XML_DEFINE RAPTOR_XML_NONE)
IF (RAPTOR_XML STREQUAL "libxml")
    SET(RAPTOR_XML_DEFINE RAPTOR_XML_LIBXML)
ENDIF (RAPTOR_XML STREQUAL "libxml")

SET(RAPTOR_WWW_INIT none)
IF (LIBXML2_FOUND)
    SET(RAPTOR_WWW_INIT xml)
ENDIF (LIBXML2_FOUND)
IF (CURL_FOUND)
    SET(RAPTOR_WWW_INIT curl)
ENDIF (CURL_FOUND)

SET(RAPTOR_WWW ${RAPTOR_WWW_INIT} CACHE STRING "Which WWW library to use (any of \"curl\", \"xml\", \"none\").")

SET(RAPTOR_WWW_DEFINE RAPTOR_WWW_NONE)
IF (RAPTOR_WWW STREQUAL "curl")
    SET(RAPTOR_WWW_DEFINE RAPTOR_WWW_LIBCURL)
ELSEIF (RAPTOR_WWW STREQUAL "xml")
    SET(RAPTOR_WWW_DEFINE RAPTOR_WWW_LIBXML)
ENDIF (RAPTOR_WWW STREQUAL "curl")

SET(RAPTOR_XML_1_1 FALSE CACHE BOOL
        "Use XML version 1.1 name checking.")

SET(HAVE_RAPTOR_PARSE_DATE 1)
SET(RAPTOR_PARSEDATE 1)

SET(RAPTOR_PARSER_RDFXML_INIT FALSE)
IF (LIBXML2_FOUND)
    SET(RAPTOR_PARSER_RDFXML_INIT TRUE)
ELSE ()
    #    MESSAGE(FATAL_ERROR "LIBXML2 LIBRARY WAS NOT FOUND")
ENDIF ()

SET(RAPTOR_PARSER_GRDDL_INIT FALSE)
IF (LIBXML2_FOUND AND LIBXSLT_FOUND)
    #    set(LIBXSLT_DEFINITIONS "xslt" "xml2" "z" "pthread" "m")
    SET(RAPTOR_PARSER_GRDDL_INIT TRUE)
ENDIF (LIBXML2_FOUND AND LIBXSLT_FOUND)

SET(RAPTOR_PARSER_RDFXML ${RAPTOR_PARSER_RDFXML_INIT} CACHE BOOL "Build RDF/XML parser.")
SET(RAPTOR_PARSER_NTRIPLES TRUE CACHE BOOL "Build N-Triples parser.")
SET(RAPTOR_PARSER_TURTLE TRUE CACHE BOOL "Build Turtle parser.")
SET(RAPTOR_PARSER_TRIG TRUE CACHE BOOL "Build TRiG parser.")
SET(RAPTOR_PARSER_RSS ${LIBXML2_FOUND} CACHE BOOL "Build RSS Tag Soup parser.")
SET(RAPTOR_PARSER_GRDDL ${RAPTOR_PARSER_GRDDL_INIT} CACHE BOOL "Build GRDDL parser.")
SET(RAPTOR_PARSER_GUESS TRUE CACHE BOOL "Build guess parser.")
SET(RAPTOR_PARSER_RDFA ${LIBXML2_FOUND} CACHE BOOL "Build RDFA parser.")
SET(RAPTOR_PARSER_JSON ${YAJL_FOUND} CACHE BOOL "Build JSON parser.")
SET(RAPTOR_PARSER_NQUADS TRUE CACHE BOOL "Build N-Quads parser.")

SET(RAPTOR_SERIALIZER_RDFXML TRUE CACHE BOOL "Build RDF/XML serializer.")
SET(RAPTOR_SERIALIZER_NTRIPLES TRUE CACHE BOOL "Build N-Triples serializer.")
SET(RAPTOR_SERIALIZER_RDFXML_ABBREV TRUE CACHE BOOL "Build RDF/XML-abbreviated serializer.")
SET(RAPTOR_SERIALIZER_TURTLE TRUE CACHE BOOL "Build Turtle serializer.")
SET(RAPTOR_SERIALIZER_RSS_1_0 TRUE CACHE BOOL "Build RSS 1.0 serializer.")
SET(RAPTOR_SERIALIZER_ATOM TRUE CACHE BOOL "Build Atom 1.0 serializer.")
SET(RAPTOR_SERIALIZER_DOT TRUE CACHE BOOL "Build GraphViz DOT serializer.")
SET(RAPTOR_SERIALIZER_HTML TRUE CACHE BOOL "Build HTML Table serializer.")
SET(RAPTOR_SERIALIZER_JSON TRUE CACHE BOOL "Build JSON serializer.")
SET(RAPTOR_SERIALIZER_NQUADS TRUE CACHE BOOL "Build N-Quads serializer.")

################################################################


# On Windows, the test/utility programs usually have DLL dependencies that
# make them cumbersome to invoke within IDE environments, so we enable the
# test suite only when makefiles are being used.
#
# Note that a partial solution is described at
#
#	http://www.cmake.org/pipermail/cmake/2009-May/029464.html
#
# but we don't have a good way of knowing where the third-party DLLs are.
#
IF (CMAKE_GENERATOR MATCHES "Makefiles")
    ENABLE_TESTING()
ENDIF (CMAKE_GENERATOR MATCHES "Makefiles")

SUBDIRS(src)

# comment out utils because 1) we don't want to use them and 2) they
# do not compile with cmake script (yet). Note they work fine with autotools docs-build system
#SUBDIRS(utils)

################################################################

FIND_PROGRAM(JING jing)
IF (NOT JING STREQUAL "JING-NOTFOUND")
    SET(HAVE_JING TRUE)
ENDIF (NOT JING STREQUAL "JING-NOTFOUND")

# commented out because we do not want these executables, only the library.
# and these do not compile via cmake (though they do via makefile?)
#SET(RAPPER ${CMAKE_BINARY_DIR}/utils/rapper)
#SET(RDFDIFF ${CMAKE_BINARY_DIR}/utils/rdfdiff)

MACRO(RAPPER_TEST NAME_PREFIX COMMAND OUTPUT REFERENCE)
    FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/test-${NAME_PREFIX}.cmake "
EXECUTE_PROCESS(
	COMMAND ${COMMAND}
	TIMEOUT 10
	OUTPUT_FILE ${OUTPUT}.raw
	ERROR_QUIET
)

# http://public.kitware.com/Bug/view.php?id=13007

CONFIGURE_FILE(
	${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT}.raw
	${OUTPUT}
	@ONLY
	NEWLINE_STYLE UNIX
)
")
    ADD_TEST("${NAME_PREFIX}:run" ${CMAKE_COMMAND} -P test-${NAME_PREFIX}.cmake)
    ADD_TEST("${NAME_PREFIX}:cmp" ${CMAKE_COMMAND} -E compare_files ${REFERENCE} ${OUTPUT})
ENDMACRO(RAPPER_TEST NAME_PREFIX COMMAND OUTPUT REFERENCE)

MACRO(RAPPER_RDFDIFF_TEST NAME RAPPER_COMMAND RAPPER_OUTPUT RDFDIFF_COMMAND)
    FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/test-${NAME}.cmake "
SET(rdfdiff_status -1)

EXECUTE_PROCESS(
	COMMAND ${RAPPER_COMMAND}
	TIMEOUT 10
	OUTPUT_FILE ${RAPPER_OUTPUT}
	ERROR_QUIET
	RESULT_VARIABLE rapper_status
)

IF(rapper_status EQUAL 0 OR rapper_status EQUAL 2)
	EXECUTE_PROCESS(
		COMMAND ${RDFDIFF_COMMAND}
		TIMEOUT 10
		OUTPUT_QUIET
		ERROR_QUIET
		RESULT_VARIABLE rdfdiff_status
	)
ENDIF(rapper_status EQUAL 0 OR rapper_status EQUAL 2)

IF(NOT rdfdiff_status EQUAL 0)
	MESSAGE(FATAL_ERROR \"Test failed.\")
ENDIF(NOT rdfdiff_status EQUAL 0)
")

    ADD_TEST(${NAME} ${CMAKE_COMMAND} -P test-${NAME}.cmake)
ENDMACRO(RAPPER_RDFDIFF_TEST NAME RAPPER_COMMAND RAPPER_OUTPUT RDFDIFF_COMMAND)

SUBDIRS(tests/feeds)
SUBDIRS(tests/grddl)
SUBDIRS(tests/json)
SUBDIRS(tests/ntriples)
SUBDIRS(tests/rdfa)
SUBDIRS(tests/rdfa11)
SUBDIRS(tests/rdfxml)
SUBDIRS(tests/trig)
#SUBDIRS(tests/turtle) // no cmake in turtle

# end raptor/_CMakeLists.txt

message(STATUS "

    Raptor dependency paths:
    	CURL_STATIC_LIBRARY				    ${CURL_STATIC_LIBRARY}
    	CURL_INCLUDE_DIRS				    ${CURL_INCLUDE_DIRS}
    	LIBXML2_INCLUDE_DIRS				${LIBXML2_INCLUDE_DIRS}
    	LIBXML2_STATIC_LIBRARY				${LIBXML2_STATIC_LIBRARY}
    	LIBXML2_EXTRA_LIBS                  ${LIBXML2_EXTRA_LIBS}
    	LIBXSLT_STATIC_LIBRARY				${LIBXSLT_STATIC_LIBRARY}
    	LIBXSLT_INCLUDE_DIRS				${LIBXSLT_INCLUDE_DIRS}
    	ZLIB_INCLUDE_DIRS                   ${ZLIB_INCLUDE_DIRS}
    	ZLIB_STATIC_LIBRARY                 ${ZLIB_STATIC_LIBRARY}
    	MATH_INCLUDE_DIRS                   ${MATH_INCLUDE_DIRS}
    	LIBM_STATIC_LIBRARY                 ${LIBM_STATIC_LIBRARY}
    	LIBICONV_STATIC_LIBRARY             ${LIBICONV_STATIC_LIBRARY}
    	LIBICONV_INCLUDE_DIRS               ${LIBICONV_INCLUDE_DIRS}

    Dependency Found variables
        CURL_FOUND                          ${CURL_FOUND}
        LIBXSLT_FOUND                       ${LIBXSLT_FOUND}
        LIBXML2_FOUND                       ${LIBXML2_FOUND}
        ZLIB_FOUND                          ${ZLIB_FOUND}

    Raptor options:
    	RAPTOR_PARSER_RDFXML					${RAPTOR_PARSER_RDFXML}
    	RAPTOR_PARSER_NTRIPLES					${RAPTOR_PARSER_NTRIPLES}
    	RAPTOR_PARSER_TURTLE					${RAPTOR_PARSER_TURTLE}
    	RAPTOR_PARSER_TRIG						${RAPTOR_PARSER_TRIG}
    	RAPTOR_PARSER_RSS						${RAPTOR_PARSER_RSS}
    	RAPTOR_PARSER_GRDDL						${RAPTOR_PARSER_GRDDL}
    	RAPTOR_PARSER_GUESS						${RAPTOR_PARSER_GUESS}
    	RAPTOR_PARSER_RDFA						${RAPTOR_PARSER_RDFA}
    	RAPTOR_PARSER_JSON						${RAPTOR_PARSER_JSON}
    	RAPTOR_PARSER_NQUADS					${RAPTOR_PARSER_NQUADS}
    	RAPTOR_SERIALIZER_RDFXML				${RAPTOR_SERIALIZER_RDFXML}
    	RAPTOR_SERIALIZER_NTRIPLES				${RAPTOR_SERIALIZER_NTRIPLES}
    	RAPTOR_SERIALIZER_RDFXML_ABBREV			${RAPTOR_SERIALIZER_RDFXML_ABBREV}
    	RAPTOR_SERIALIZER_TURTLE				${RAPTOR_SERIALIZER_TURTLE}
    	RAPTOR_SERIALIZER_RSS_1_0				${RAPTOR_SERIALIZER_RSS_1_0}
    	RAPTOR_SERIALIZER_ATOM					${RAPTOR_SERIALIZER_ATOM}
    	RAPTOR_SERIALIZER_DOT					${RAPTOR_SERIALIZER_DOT}
    	RAPTOR_SERIALIZER_HTML					${RAPTOR_SERIALIZER_HTML}
    	RAPTOR_SERIALIZER_JSON					${RAPTOR_SERIALIZER_JSON}
    	RAPTOR_SERIALIZER_NQUADS				${RAPTOR_SERIALIZER_NQUADS}


")


#-DCURL_STATIC_LIBRARY=/usr/lib/x86_64-linux-gnu/libcurl.a
#-DCURL_INCLUDE_DIRS=/usr/include/x86_64-linux-gnu
#-DLIBXML2_INCLUDE_DIRS=/usr/include/libxml2
#-DLIBXML2_STATIC_LIBRARY=/usr/lib/x86_64-linux-gnu/libxml2.a
#-DLIBXSLT_STATIC_LIBRARY=/usr/lib/x86_64-linux-gnu/libxslt.a
#-DLIBXSLT_INCLUDE_DIRS=/usr/include
#-DZLIB_INCLUDE_DIRS=/usr/include
#-DZLIB_STATIC_LIBRARY=/usr/lib/x86_64-linux-gnu/libz.a
#-DMATH_INCLUDE_DIRS=/usr/include
#-DLIBM_STATIC_LIBRARY=/usr/lib/x86_64-linux-gnu/libm.a
#-DCMAKE_INSTALL_PREFIX=./install_solo