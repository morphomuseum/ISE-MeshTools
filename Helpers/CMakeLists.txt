# Set your files and resources here
SET (Helpers_SRCS
vtkOrientationHelperActor.cxx vtkGridActor.cxx vtkMTActor.cxx vtkMTActorCollection.cxx vtkOrientationHelperWidget.cxx vtkMTInteractorStyle.cxx vtkUndoStack.cxx vtkUndoStackBuilder.cxx vtkMeshToolsCore.cxx)

set( Helpers_HDRS vtkOrientationHelperActor.h vtkMTActor.h vtkMTActorCollection.h vtkGridActor.h vtkOrientationHelperWidget.h vtkMTInteractorStyle.h vtkUndoStack.h vtkUndoStackBuilder.h vtkUndoStackInternal.h vtkMeshToolsCore.h)

#Sorry but we need QT for the undo stack stuff... 
#So far, in the interactor style and in the core
#Something has to be improved
find_package(Qt5 COMPONENTS Widgets REQUIRED QUIET)
include_directories(${Qt5Widgets_INCLUDE_DIRS})


ADD_LIBRARY (MeshToolsHelpers ${Helpers_SRCS} ${Helpers_HDRS})
include_directories ("${PROJECT_SOURCE_DIR}/Qt")

#TARGET_LINK_LIBRARIES(MeshToolsHelpers  vtkGraphics vtkIOCore vtkCommonCore)



