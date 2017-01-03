# Set your files and resources here
SET (Helpers_SRCS
vtkOrientationHelperActor.cxx vtkGridActor.cxx vtkMTActor.cxx vtkMTActorCollection.cxx vtkOrientationHelperWidget.cxx vtkMTInteractorStyle.cxx )

set( Helpers_HDRS vtkOrientationHelperActor.h vtkMTActor.h vtkMTActorCollection.h vtkGridActor.h vtkOrientationHelperWidget.h vtkMTInteractorStyle.h)

ADD_LIBRARY (MeshToolsHelpers ${Helpers_SRCS} ${Helpers_HDRS})

#TARGET_LINK_LIBRARIES(MeshToolsHelpers  vtkGraphics vtkIOCore vtkCommonCore)



