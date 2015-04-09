cmake_minimum_required (VERSION 2.8)
project (tinyxml)

SET (LIBRARY_OUTPUT_PATH ../../../lib)

SET (HEADER_FILES
 ../tinystr.h
 ../tinyxml.h
)

SET (SOURCE_FILES  	
 ../tinystr.cpp
 ../tinyxml.cpp   
 ../tinyxmlerror.cpp  
 ../tinyxmlparser.cpp  
)
 
add_library( tinyxml   STATIC  
              ${HEADER_FILES}  ${SOURCE_FILES}
  	 )   	 
  	  
 
 
 SET_TARGET_PROPERTIES( tinyxml PROPERTIES DEBUG_OUTPUT_NAME "tinyxmld")
