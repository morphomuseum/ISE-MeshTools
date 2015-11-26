cmake_minimum_required(VERSION 2.6)
PROJECT( MeshTools )

#  Define the list of source files
#
SET(APPLICATION_SOURCES
  MeshToolsMain.cxx  
  font.cpp    
  StdAfx.cpp
  Container.cxx
  Container_Mesh.cxx
  Fl_Value_Roller.cxx
  CDataFile.cpp
  Object.cxx
  Object_Mesh.cxx
  Object_Log.cxx
  Object_Landmark.cxx
  SpecialSlider2.cxx
  SpecialSlider3.cxx
  SpecialRoller2.cxx
  SpecialRoller3.cxx
  SpecialRoller4.cxx
  Struct.cxx
  MeshTools.cxx
  MeshToolsUI.cxx
  vtkOBJWriter.cxx
  Icp.cpp
  KDtree.cpp
  LandmarksTransform.cpp
 )

SET(APPLICATION_HEADERS
  font.h
  StdAfx.h
  Container.h
  Container_Mesh.h
  Fl_Value_Roller.h
  CDataFile.h
  Object.h
  Object_Mesh.h
  Object_Log.h 
  Object_Landmark.h
  SpecialSlider2.h
  SpecialSlider3.h
  SpecialRoller2.h
  SpecialRoller3.h
  SpecialRoller4.h
  Struct.h
  Struct2.h
  MeshTools.h
  MeshToolsUI.h
  vtkOBJWriter.h
  Icp.h
  KDtree.h
  LandmarksTransform.h
 )

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

FIND_PACKAGE(OpenGL)
 IF(OPENGL_INCLUDE_PATH)
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_PATH})
 ENDIF(OPENGL_INCLUDE_PATH)

  FIND_PACKAGE(FLTK)
  IF(FLTK_FOUND)
    INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})    
  ENDIF(FLTK_FOUND)

# define needed VTK Modules 
set( required_vtk_modules
  vtkCommonComputationalGeometry
  vtkCommonCore
  vtkCommonDataModel
  vtkCommonExecutionModel
  vtkCommonMath
  vtkCommonMisc
  vtkCommonSystem
  vtkCommonTransforms
  vtkFiltersCore
  vtkFiltersGeneral
  vtkIOCore
  vtkIOGeometry
  vtkIOLegacy
  vtkIOXML
  vtkIOXMLParser
  vtkFiltersModeling
  vtkFiltersHybrid
  vtkIOImport
  vtkRenderingCore
  vtkRenderingOpenGL
  vtkIOPLY
  )
find_package( VTK REQUIRED COMPONENTS ${required_vtk_modules} )

OPTION(CMAKE_INSTALL_PREFIX ".")
INCLUDE(${VTK_USE_FILE})
IF(VTK_FOUND)
  INCLUDE_DIRECTORIES(${VTK_INCLUDE_DIR})    
ENDIF(VTK_FOUND)

  IF(OPENGL_glu_LIBRARY)
      SET( OPENGL_GLU_FOUND "YES" )
      SET( OPENGL_LIBRARIES ${OPENGL_glu_LIBRARY} ${OPENGL_LIBRARIES} )
  ENDIF(OPENGL_glu_LIBRARY)
  

ADD_EXECUTABLE( MeshTools ${APPLICATION_SOURCES} ${APPLICATION_HEADERS})


LINK_LIBRARIES(${OPENGL_LIBRARIES})  
TARGET_LINK_LIBRARIES( MeshTools 
	${FLTK_LIBRARIES} 
	${OPENGL_LIBRARIES}
	${VTK_LIBRARIES}
	
  )
INSTALL(TARGETS MeshTools
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)  


