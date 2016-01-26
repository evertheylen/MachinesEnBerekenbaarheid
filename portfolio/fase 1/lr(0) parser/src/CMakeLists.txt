# The source files to compile
SET(SRC main.cpp LRParser.cpp tinyxml/tinystr.cpp tinyxml/tinyxml.cpp tinyxml/tinyxmlerror.cpp tinyxml/tinyxmlparser.cpp)

# The program we're compiling with the sources set above
ADD_EXECUTABLE(exec ${SRC})

INSTALL(TARGETS exec DESTINATION ${CMAKE_SOURCE_DIR}/bin)
