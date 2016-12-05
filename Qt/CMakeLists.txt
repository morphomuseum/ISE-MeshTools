set (Qt_SRCS
  #mqAboutDialog.cxx
  #mqAboutDialog.h
  #mqAboutDialogReaction.cxx
  #mqAboutDialogReaction.h
  mqDesktopServicesReaction.cxx
  mqDesktopServicesReaction.h
  mqReaction.cxx
  mqReaction.h
  mqSetName.cxx
  mqSetName.h
  mqMeshToolsMenuBuilders.cxx
  mqMeshToolsMenuBuilders.h
)

#set (Qt_MOC_HDRS
#  mqAboutDialogReaction.h
#  mqAboutDialog.h
#)

#set (Qt_UI_FILES    mqAboutDialog.ui )



# We have ui files, this will bring in the macro: qt5_wrap_ui
find_package(Qt5 COMPONENTS Widgets REQUIRED QUIET)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
#qt5_wrap_ui(UI_Srcs ${UIs})
#qt5_add_resources(QRC_Srcs ${Qt_SRCS} )


ADD_LIBRARY (MeshToolsQt ${Qt_SRCS} 
#${Qt_HDRS}
)




