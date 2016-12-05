cmake_minimum_required(VERSION 2.8.8)
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 NEW) # CMake 3.0
endif()
if(POLICY CMP0053)
  cmake_policy(SET CMP0053 NEW) # CMake 3.1
endif()

project(MeshTools)

find_package(VTK COMPONENTS
  vtkCommonCore
  vtkIOCore
  vtkFiltersCore
  vtkInfovisCore
  vtkInteractionStyle
  vtkViewsQt
  vtkCommonComputationalGeometry
  vtkCommonCore
  vtkCommonDataModel
  vtkCommonExecutionModel
  vtkCommonMath
  vtkCommonMisc
  vtkCommonSystem
  vtkCommonTransforms
  vtkFiltersGeneral  
  vtkIOGeometry
  vtkIOLegacy
  vtkIOXML
  vtkIOXMLParser
  vtkFiltersModeling
  vtkFiltersHybrid
  vtkIOImport
  vtkRenderingCore
  # vtkRenderingOpenGL
  vtkIOPLY
)
include(${VTK_USE_FILE})

if("${VTK_QT_VERSION}" STREQUAL "")
  message(FATAL_ERROR "VTK was not built with Qt")
endif()

# Use the include path and library for Qt that is used by VTK.
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set your files and resources here

set( Srcs main.cxx MeshTools.cxx)

set( Hdrs MeshTools.h )

set( MOC_Hdrs MeshTools.h )

set( UIs MeshTools.ui )

set( QRCs Icons/icons.qrc )




# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# The rest should just work (sure...)
if(VTK_QT_VERSION VERSION_GREATER "4")
  # We have ui files, this will bring in the macro: qt5_wrap_ui
  find_package(Qt5 COMPONENTS Widgets REQUIRED QUIET)
  qt5_wrap_ui(UI_Srcs ${UIs})
  qt5_add_resources(QRC_Srcs ${QRCs} )

  source_group("Resources" FILES
    ${UIs}
    ${QRCs}
    ${EXE_ICON} # Not present
  )

  source_group("Generated" FILES
    ${UI_Srcs}
    ${MOC_Srcs}
    ${QRC_Srcs}
    ${QRC_Srcs}
  )
include_directories ("${PROJECT_SOURCE_DIR}/Helpers")
add_subdirectory (Helpers) 

include_directories ("${PROJECT_SOURCE_DIR}/Qt")
add_subdirectory (Qt) 

set (EXTRA_LIBRARIES ${EXTRA_LIBRARIES} 
MeshToolsHelpers
MeshToolsQt
)

  add_executable(MeshTools MACOSX_BUNDLE
    ${Srcs} ${Hdrs} ${UI_Srcs} ${MOC_Hdrs} ${QRC_Srcs})
  qt5_use_modules(MeshTools Core Gui Widgets)
  target_link_libraries(MeshTools ${VTK_LIBRARIES} ${EXTRA_LIBRARIES} )
else()

  message(FATAL_ERROR "Qt 5 required")

endif()
