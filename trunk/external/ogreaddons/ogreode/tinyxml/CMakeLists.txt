#build tinyxml library

set(SRC_FILES   "tinystr.cpp"
                "tinyxml.cpp"
                "tinyxmlerror.cpp"
                "tinyxmlparser.cpp"
                "xmltest.cpp")

add_library( tinyxml ${SRC_FILES} )

