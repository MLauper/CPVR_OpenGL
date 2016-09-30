# Macro to loop through all demo project directories

MACRO(GET_DIRECTORIES retval dirname sourcedirectory)
	FILE(GLOB subdir ${sourcedirectory}/${dirname})
	SET(listofdirs "")
	FOREACH(dir ${subdir})
		IF(IS_DIRECTORY ${dir})	
			IF(EXISTS ${dir}/CMakeLists.txt)
				IF(NOT EXISTS ${dir}/ignore.cmake)
					SET(listofdirs ${listofdirs} ${dir})
				ENDIF()
			ENDIF()  
		ENDIF()
	ENDFOREACH()
	SET(${retval} ${listofdirs})
ENDMACRO()
