# Set your files and resources here
SET (Helpers_SRCS
vtkOrientationHelperActor.cxx vtkGridActor.cxx vtkMTActor.cxx vtkMTActorCollection.cxx vtkOrientationHelperWidget.cxx vtkMTInteractorStyle.cxx vtkUndoElement.cxx vtkUndoSet.cxx vtkUndoStack.cxx)

set( Helpers_HDRS vtkOrientationHelperActor.h vtkMTActor.h vtkMTActorCollection.h vtkGridActor.h vtkOrientationHelperWidget.h vtkMTInteractorStyle.h vtkUndoElement.h vtkUndoSet.h vtkUndoStack.h vtkUndoStackInternal.h)

ADD_LIBRARY (MeshToolsHelpers ${Helpers_SRCS} ${Helpers_HDRS})

#TARGET_LINK_LIBRARIES(MeshToolsHelpers  vtkGraphics vtkIOCore vtkCommonCore)



