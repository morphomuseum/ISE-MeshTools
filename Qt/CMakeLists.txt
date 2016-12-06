set (Qt_SRCS
  mqAboutDialog.cxx
  mqAboutDialog.h
  mqAboutDialogReaction.cxx
  mqAboutDialogReaction.h
  mqCoreUtilities.cxx
  mqCoreUtilities.h
  mqDesktopServicesReaction.cxx
  mqDesktopServicesReaction.h
  mqReaction.cxx
  mqReaction.h
  mqSetName.cxx
  mqSetName.h
  mqMeshToolsMenuBuilders.cxx
  mqMeshToolsMenuBuilders.h
)

set (Qt_MOC_HDRS
  mqAboutDialogReaction.h
  mqAboutDialog.h
)

set (Qt_UIs mqAboutDialog.ui )



# We have ui files, this will bring in the macro: qt5_wrap_ui
find_package(Qt5 COMPONENTS Widgets REQUIRED QUIET)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
qt5_wrap_ui(QtUI_Srcs ${Qt_UIs})
#qt5_add_resources(QRC_Srcs ${Qt_SRCS} )


 source_group("Resources" FILES
    ${Qt_UIs}
    
  )

  source_group("Generated" FILES
    ${QtUI_Srcs}   
   
  )
#for some reason, otherwise wrapped ui related .h files are not read
include_directories ("${PROJECT_BINARY_DIR}/Qt")

ADD_LIBRARY (MeshToolsQt ${Qt_SRCS} ${QtUI_Srcs} ${Qt_UIs}   
  
#${Qt_HDRS}
)




