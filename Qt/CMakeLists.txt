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
  mqColorChooserButton.cxx
  mqColorChooserButton.h
  mqColorDialogReaction.h
  mqColorDialogReaction.cxx
  mqColorDialog.cxx
  mqColorDialog.h
  mqLandmarkDialogReaction.h
  mqLandmarkDialogReaction.cxx
  mqLandmarkDialog.cxx
  mqLandmarkDialog.h
  mqUndoRedoReaction.h
  mqUndoRedoReaction.cxx
  mqUndoStack.h
  mqUndoStack.cxx
  mqMainControlsToolbar.h
  mqMainControlsToolbar.cxx
  mqMeshToolsCore.h
  mqMeshToolsCore.cxx


)

set (Qt_MOC_HDRS
  mqAboutDialogReaction.h
  mqAboutDialog.h
)

set (Qt_UIs mqAboutDialog.ui mqColorDialog.ui mqLandmarkDialog.ui  mqMainControlsToolbar.ui)

set( Qt_RCs ../Icons/icons.qrc )


# We have ui files, this will bring in the macro: qt5_wrap_ui
find_package(Qt5 COMPONENTS Widgets REQUIRED QUIET)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
qt5_wrap_ui(QtUI_Srcs ${Qt_UIs})
qt5_add_resources(QRC_Srcs ${Qt_RCS} )


 source_group("Resources" FILES
    ${Qt_UIs}
    ${Qt_RCs}
    
  )

  source_group("Generated" FILES
    ${QtUI_Srcs}   
    ${QRC_Srcs}   

   
  )
#for some reason, otherwise wrapped ui related .h files are not read
include_directories ("${PROJECT_BINARY_DIR}/Qt")

include_directories ("${PROJECT_SOURCE_DIR}/Helpers")



ADD_LIBRARY (MeshToolsQt ${Qt_SRCS} ${QtUI_Srcs} ${Qt_UIs}   
 

 
#${Qt_HDRS}
)




