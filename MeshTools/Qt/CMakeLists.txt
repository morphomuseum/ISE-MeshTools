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
  mqCreateLMKDialogReaction.h
  mqCreateLMKDialogReaction.cxx
  mqCreateLMKDialog.h
  mqCreateLMKDialog.cxx
  mqEditLMKDialogReaction.h
  mqEditLMKDialogReaction.cxx
  mqEditLMKDialog.h
  mqEditLMKDialog.cxx
  mqEditAlphaDialogReaction.h
  mqEditAlphaDialogReaction.cxx
  mqEditAlphaDialog.h
  mqEditAlphaDialog.cxx
  mqEditFLGDialogReaction.h
  mqEditFLGDialogReaction.cxx
  mqEditFLGDialog.h
  mqEditFLGDialog.cxx
  mqEditACTORDialogReaction.h
  mqEditACTORDialogReaction.cxx
  mqEditACTORDialog.h
  mqEditACTORDialog.cxx
  mqEditAllFLGDialogReaction.h
  mqEditAllFLGDialogReaction.cxx
  mqEditAllFLGDialog.h
  mqEditAllFLGDialog.cxx
  mqGridSizeDialogReaction.h
  mqGridSizeDialogReaction.cxx
  mqGridSizeDialog.h
  mqGridSizeDialog.cxx
  mqOpenDataReaction.cxx
  mqOpenDataReaction.h
  mqSetName.cxx
  mqSaveDataReaction.cxx
  mqSaveDataReaction.h
  mqSetName.h
  mqMeshToolsMenuBuilders.cxx
  mqMeshToolsMenuBuilders.h
  mqChangeNodeReaction.h
  mqChangeNodeReaction.cxx
  mqColorChooserButton.cxx
  mqColorChooserButton.h
  mqColorDialogReaction.h
  mqColorDialogReaction.cxx
  mqCameraReaction.h
  mqDisplayReaction.cxx
  mqDisplayReaction.h
  mqCameraReaction.cxx
  mqColorDialog.cxx
  mqColorDialog.h
  mqLandmarkDialogReaction.h
  mqLandmarkDialogReaction.cxx
  mqLandmarkDialog.cxx
  mqLandmarkDialog.h
  mqMoveCurveHandlesDialog.h
  mqMoveCurveHandlesDialog.cxx
  mqUndoRedoReaction.h
  mqUndoRedoReaction.cxx
  mqUndoStack.h
  mqUndoStack.cxx
  mqMainControlsToolbar.h
  mqMainControlsToolbar.cxx
  mqLightControlsToolbar.h
  mqLightControlsToolbar.cxx
  mqScalarsControlsToolbar.h
  mqScalarsControlsToolbar.cxx
  mqMeshToolsCore.h
  mqMeshToolsCore.cxx
  mqSavePLYDialog.cxx
  mqSavePLYDialog.h
  mqSavePLYDialogReaction.cxx
  mqSavePLYDialogReaction.h
  mqSaveVTPDialog.cxx
  mqSaveVTPDialog.h
  mqSaveVTPDialogReaction.cxx
  mqSaveVTPDialogReaction.h
  mqSaveSTLDialog.cxx
  mqSaveSTLDialog.h
  mqSaveSTLDialogReaction.cxx
  mqSaveSTLDialogReaction.h
  mqSaveLandmarksDialog.cxx
  mqSaveLandmarksDialog.h
  mqSaveLandmarksDialogReaction.cxx
  mqSaveLandmarksDialogReaction.h
  mqSaveFlagsDialog.cxx
  mqSaveFlagsDialog.h
  mqSaveFlagsDialogReaction.cxx
  mqSaveFlagsDialogReaction.h
  mqSaveCURDialog.cxx
  mqSaveCURDialog.h
  mqSaveCURDialogReaction.cxx
  mqSaveCURDialogReaction.h
  mqSaveCURasVERDialogReaction.cxx
  mqSaveCURasVERDialogReaction.h
  mqSaveCURasVERDialog.cxx
  mqSaveCURasVERDialog.h
  mqSaveSTVDialog.cxx
  mqSaveSTVDialog.h
  mqSaveSTVDialogReaction.cxx
  mqSaveSTVDialogReaction.h
  mqSaveNTWDialog.cxx
  mqSaveNTWDialog.h
  mqSaveNTWDialogReaction.cxx
  mqSaveNTWDialogReaction.h
mqSelectLandmarkRangeDialog.h
mqSelectLandmarkRangeDialog.cxx
mqSelectLandmarkRangeDialogReaction.h
mqSelectLandmarkRangeDialogReaction.cxx
 mqOrientationLabelsDialog.cxx
  mqOrientationLabelsDialog.h
  mqOrientationLabelsDialogReaction.cxx
  mqOrientationLabelsDialogReaction.h
)

set (Qt_MOC_HDRS
  mqAboutDialogReaction.h
  mqAboutDialog.h
)

set (Qt_UIs mqAboutDialog.ui 
mqColorDialog.ui 
mqLandmarkDialog.ui  
mqMainControlsToolbar.ui 
mqSavePLYDialog.ui 
mqSaveVTPDialog.ui 
mqSaveSTLDialog.ui 
mqSaveLandmarksDialog.ui 
mqSaveFlagsDialog.ui 
mqSaveCURDialog.ui 
mqSaveCURasVERDialog.ui 
mqSaveSTVDialog.ui
mqSaveNTWDialog.ui
mqMoveCurveHandlesDialog.ui
mqEditLMKDialog.ui
mqEditAlphaDialog.ui
mqCreateLMKDialog.ui
mqEditFLGDialog.ui
mqEditACTORDialog.ui
mqEditAllFLGDialog.ui
mqSelectLandmarkRangeDialog.ui
mqLightControlsToolbar.ui
mqScalarsControlsToolbar.ui
mqGridSizeDialog.ui
mqOrientationLabelsDialog.ui
)

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




