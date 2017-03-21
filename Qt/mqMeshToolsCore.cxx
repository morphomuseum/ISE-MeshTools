/*=========================================================================

  Program:   MeshTools
  Module:    MeshToolsCore.cxx


=========================================================================*/
#include "mqMeshToolsCore.h"
#include "vtkMTActor.h"
#include "vtkLMActor.h"
#include "vtkOrientationHelperActor.h"
#include "vtkOrientationHelperWidget.h"
#include "vtkBezierCurveSource.h"
#include <vtkActor.h>

#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkPolyDataMapper.h>
#include <vtkSTLWriter.h>
#include <vtkPLYWriter.h>
#include <vtkPolyDataWriter.h>

#include <vtkCubeAxesActor.h>
#include <vtkAppendPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>

#include "mqUndoStack.h"

#define NORMAL_LMK 0
#define TARGET_LMK 1
#define NODE_LMK 2
#define HANDLE_LMK 3
#define FLAG_LMK 4

#define NORMAL_NODE 0
#define STARTING_NODE 1
#define MILESTONE_NODE 2
#define CONNECT_NODE 3

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
//-----------------------------------------------------------------------------
mqMeshToolsCore* mqMeshToolsCore::Instance = 0;

//-----------------------------------------------------------------------------
mqMeshToolsCore* mqMeshToolsCore::instance()
{
	return mqMeshToolsCore::Instance;
}


mqMeshToolsCore::mqMeshToolsCore()
{

	mqMeshToolsCore::Instance = this;
	this->MainWindow = NULL;
	this->OrientationHelperWidget = vtkOrientationHelperWidget::New();
	this->mui_DefaultLandmarkMode = this->mui_LandmarkMode = 0;

	this->mui_LandmarkBodyType = this->mui_DefaultLandmarkBodyType = 0;
	this->mui_LandmarkRenderingSize=this->mui_DefaultLandmarkRenderingSize=1;
	this->mui_AdjustLandmarkRenderingSize= this->mui_DefaultAdjustLandmarkRenderingSize=1;
	this->mui_FlagRenderingSize= this->mui_DefaultFlagRenderingSize=5;
	this->mui_AdjustScaleFactor = this->mui_DefaultAdjustScaleFactor = 1;

	this->mui_Anaglyph = this->mui_DefaultAnaglyph = 0;
	this->mui_ShowGrid = this->mui_DefaultShowGrid = 1;
	
	this->mui_FlagColor[0] = this->mui_DefaultFlagColor[0] = 0;
	this->mui_FlagColor[1] = this->mui_DefaultFlagColor[1] = 0.7;
	this->mui_FlagColor[2] = this->mui_DefaultFlagColor[2] = 0.7;

	this->mui_MeshColor[0] = this->mui_DefaultMeshColor[0] = 0.631373;
	this->mui_MeshColor[1] = this->mui_DefaultMeshColor[1] = 0.572549;
	this->mui_MeshColor[2] = this->mui_DefaultMeshColor[2] = 0.372549;
	this->mui_MeshColor[3] = this->mui_DefaultMeshColor[3] = 1;
	
	this->mui_BackGroundColor2[0] = this->mui_DefaultBackGroundColor2[0] = 0;
	this->mui_BackGroundColor2[1] = this->mui_DefaultBackGroundColor2[1] = 0;
	this->mui_BackGroundColor2[2] = this->mui_DefaultBackGroundColor2[2] = 0;

	this->mui_BackGroundColor[0] = this->mui_DefaultBackGroundColor[0] = 0.5;
	this->mui_BackGroundColor[1] = this->mui_DefaultBackGroundColor[1] = 0.5;
	this->mui_BackGroundColor[2] = this->mui_DefaultBackGroundColor[2] = 1;

	this->mui_ShowOrientationHelper = this->mui_DefaultShowOrientationHelper = 1;

	this->mui_X1Label = this->mui_DefaultX1Label = "Anterior";
	this->mui_X2Label = this->mui_DefaultX2Label = "Posterior";

	this->mui_Y1Label = this->mui_DefaultY1Label = "Left";
	this->mui_Y2Label = this->mui_DefaultY2Label = "Right";
	this->mui_Z1Label = this->mui_DefaultZ1Label = "Dorsal";
	this->mui_Z2Label = this->mui_DefaultZ2Label = "Ventral";

	this->mui_CameraOrtho = this->mui_DefaultCameraOrtho = 1;
	this->mui_CameraCentreOfMassAtOrigin = this->mui_DefaultCameraCentreOfMassAtOrigin = 0;
	//this->UndoStack = vtkSmartPointer<vtkUndoStack>::New();
	mqUndoStack* undoStack = new mqUndoStack();
	this->setUndoStack(undoStack);
	//this->mUndoStack = undoStack;
	//MeshTools::testint = 10;
	//MeshTools::Instance = this;
	//this->SetUndoCount(0);
	//vtkUndoStack* undoStack = vtkUndoStack::New();
	//vtkUndoSet* undoSet = vtkUndoSet::New();
	//vtkUndoElement* undoElement = vtkUndoElement::New();
	//undoStack->Push("Test", undoSet);

	/*
	vtkSMSession* session = vtkSMSession::New();
	vtkSMSessionProxyManager* pxm = session->GetSessionProxyManager();

	vtkSMProxy* sphere = pxm->NewProxy("sources", "SphereSource");
	sphere->UpdateVTKObjects();
	QVERIFY(sphere != NULL);
	QCOMPARE(vtkSMPropertyHelper(sphere, "Radius").GetAsDouble(), 0.5);

	vtkSMUndoStack* undoStack = vtkSMUndoStack::New();
	vtkUndoSet* undoSet = vtkUndoSet::New();
	vtkSMRemoteObjectUpdateUndoElement* undoElement = vtkSMRemoteObjectUpdateUndoElement::New();
	undoElement->SetSession(session);

	vtkSMMessage before;
	before.CopyFrom(*sphere->GetFullState());
	vtkSMPropertyHelper(sphere, "Radius").Set(1.2);
	sphere->UpdateVTKObjects();
	vtkSMMessage after;
	after.CopyFrom(*sphere->GetFullState());
	undoElement->SetUndoRedoState(&before, &after);

	undoSet->AddElement(undoElement);
	undoElement->Delete();
	undoStack->Push("ChangeRadius", undoSet);
	undoSet->Delete();

	QVERIFY(static_cast<bool>(undoStack->CanUndo()) == true);
	undoStack->Undo();
	QVERIFY(static_cast<bool>(undoStack->CanUndo()) == false);
	sphere->UpdateVTKObjects();
	QCOMPARE(vtkSMPropertyHelper(sphere, "Radius").GetAsDouble(), 0.5);

	QVERIFY(static_cast<bool>(undoStack->CanRedo()) == true);
	undoStack->Redo();
	sphere->UpdateVTKObjects();
	QCOMPARE(vtkSMPropertyHelper(sphere, "Radius").GetAsDouble(), 1.2);
	QVERIFY(static_cast<bool>(undoStack->CanRedo()) == false);

	undoStack->Delete();

	sphere->Delete();
	session->Delete();
	*/
	//this->RenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	this->RenderWindow = NULL;
	this->Renderer = vtkSmartPointer<vtkRenderer>::New();
	this->ActorCollection = vtkSmartPointer<vtkMTActorCollection>::New();
	this->ActorCollection->SetRenderer(this->Renderer);

	this->BezierCurveSource = vtkSmartPointer<vtkBezierCurveSource>::New();
	this->BezierMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
	this->BezierSelectedMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	BezierMapper->SetInputConnection(this->BezierCurveSource->GetOutputPort(0));
	BezierSelectedMapper->SetInputConnection(this->BezierCurveSource->GetOutputPort(1));
	BezierSelectedActor = vtkSmartPointer<vtkActor>::New();
	this->BezierActor=vtkSmartPointer<vtkActor>::New();

	this->BezierActor->SetMapper(this->BezierMapper);
	this->BezierActor->GetProperty()->SetColor(0, 0.5, 0);
	this->BezierActor->GetProperty()->SetLineWidth(2.0);

	this->BezierSelectedActor->SetMapper(this->BezierSelectedMapper);
	this->BezierSelectedActor->GetProperty()->SetColor(0.8, 0.2, 0);
	this->BezierSelectedActor->GetProperty()->SetLineWidth(3.0);

	this->BezierNHMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	this->BezierNHMapper->SetInputConnection(this->BezierCurveSource->GetOutputPort(2));
	this->BezierNHActor = vtkSmartPointer<vtkActor>::New();

	this->BezierNHActor->SetMapper(this->BezierNHMapper);
	this->BezierNHActor->GetProperty()->SetColor(0, 1, 1);

	

	//vtkSmartPointer<vtkBezierCurveSource> bezierCurve =
	this->NormalLandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->NormalLandmarkCollection->SetRenderer(this->Renderer);
	this->TargetLandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->TargetLandmarkCollection->SetRenderer(this->Renderer);
	this->NodeLandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->NodeLandmarkCollection->SetRenderer(this->Renderer);
	this->HandleLandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->HandleLandmarkCollection->SetRenderer(this->Renderer);

	this->FlagLandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->FlagLandmarkCollection->SetRenderer(this->Renderer);

	this->Renderer->SetUseDepthPeeling(1);
	this->Renderer->SetMaximumNumberOfPeels(100);
	this->Renderer->SetOcclusionRatio(0.1);
	this->Camera = this->Renderer->GetActiveCamera();
	this->GridActor = vtkSmartPointer<vtkGridActor>::New();
	this->GridActor->SetGridType(2);	

	


	//this->LandmarkCollection->SetChanged(1);
	
	this->Renderer->AddActor(this->GridActor);

	

	

}
void mqMeshToolsCore::ComputeSelectedNamesLists()
{
	
		g_selected_names.clear();
		g_distinct_selected_names.clear();
	
		this->ActorCollection->InitTraversal();

	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor * myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			g_selected_names.push_back(myActor->GetName());

			int already = 0;

			for (int j = 0; j<g_distinct_selected_names.size(); j++)
			{
				//std::cout<<"i"<<i<<std::endl;
				std::size_t found = g_distinct_selected_names.at(j).find(myActor->GetName());
				size_t length1 = myActor->GetName().length();
				size_t length2 = g_distinct_selected_names.at(j).length();
				if (length1 == length2 && found != std::string::npos)
				{
					already = 1;
				}
			}
			if (already == 0)
			{
				g_distinct_selected_names.push_back(myActor->GetName());
			}


		}
	}
	
}

int mqMeshToolsCore::context_file_exists(std::string path, std::string ext, std::string postfix)
{
	// used by the save NTW function : 
	//looks whether a non-mesh related file = context file (.ori, .flg, .tag, .ver, .cur, .stv) constructed with only a postfix + extension (project name) already exists 
	std::string filename;
	int exists = 0;

	filename = path.c_str();
	if (postfix.length()>0)
	{
		filename.append(postfix.c_str());
	}
	filename.append(ext.c_str());
	ifstream file(filename.c_str());
	if (file)
	{
		file.close();
		exists = 1;
	}
	return exists;
}

int mqMeshToolsCore::selected_file_exists(std::string path, std::string ext, std::string postfix)
{
	// used by the save NTW function : 
	//looks whether mesh related files (vtk, vtp, stl, ply, pos) constructed with a prefix (object name) and a postfix (project name) already exist 
	std::string filename;
	int exists = 0;
	for (int i = 0; i<g_distinct_selected_names.size(); i++)
	{
		filename = path.c_str();
		filename.append(g_distinct_selected_names.at(i).c_str());
		if (postfix.length()>0)
		{
			filename.append(postfix.c_str());
		}
		filename.append(ext.c_str());
		ifstream file(filename.c_str());
		if (file)
		{
			file.close();
			exists = 1;
			break;
		}
	}

	return exists;

}

void mqMeshToolsCore::SaveORI(QString fileName)
{

	QString X1, X2, Y1, Y2, Z1, Z2;

	std::string ORIext = ".ori";
	std::string ORIext2 = ".ORI";
	std::size_t found = fileName.toStdString().find(ORIext);
	std::size_t found2 = fileName.toStdString().find(ORIext2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".ori");
	}

	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		stream << this->Getmui_Z1Label() << endl;
		stream << this->Getmui_Z2Label() << endl;
		stream << this->Getmui_Y1Label() << endl;
		stream << this->Getmui_Y2Label() << endl;
		stream << this->Getmui_X1Label() << endl;
		stream << this->Getmui_X2Label() << endl;


	}
	file.close();





}

int mqMeshToolsCore::SaveNTWFile(QString fileName, int save_ori, int save_tag, int save_surfaces_as_ply)
{
	std::string NTWext = ".ntw";
	std::string NTWext2 = ".NTW";
	std::size_t found = fileName.toStdString().find(NTWext);
	std::size_t found2 = fileName.toStdString().find(NTWext2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".ntw");
	}

	QFileInfo fileInfo(fileName);
	QString onlyfilename(fileInfo.fileName());



	QFile file(fileName);
	QTextStream stream(&file);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		std::string only_filename = onlyfilename.toStdString();
		std::string project_name = only_filename.c_str();
		size_t nPos = project_name.find_last_of(".");
		if (nPos >0 && nPos<= project_name.length())
		{
			project_name = project_name.substr(0, nPos);
		}
		std::string path = fileName.toStdString().substr(0, (fileName.toStdString().length() - only_filename.length()));
		std::string posExt = ".pos";
		std::string vtpExt = ".vtp";
		std::string plyExt = ".vtp";
		std::string tagExt = ".tag";
		std::string oriExt = ".ori";
		std::string flgExt = ".flg";
		std::string verExt = ".ver";
		std::string stvExt = ".stv";
		std::string curExt = ".cur";
		int overwrite_pos = 1;
		int overwrite_mesh = 1;
		int overwrite_ori = 1;
		int overwrite_tag = 1;
		int overwrite_flg = 1;
		int overwrite_ver = 1;
		int overwrite_cur = 1;
		int overwrite_stv = 1;

		this->ComputeSelectedNamesLists();
		std::string postfix = "_";
		postfix.append(project_name.c_str());
		std::string no_postfix = "";
		int pos_exists = this->selected_file_exists(path, posExt, postfix);

		int mesh_exists = 0;
		if (save_surfaces_as_ply == 0)
		{
			mesh_exists = this->selected_file_exists(path, vtpExt, no_postfix);
		}
		else
		{
			mesh_exists = this->selected_file_exists(path, plyExt, no_postfix);
		}

		if (save_ori == 1)
		{
			std::string _ori_fullpath;
			std::string _ori_file;
			int ori_exists = this->context_file_exists(path, oriExt, project_name);
			if (ori_exists == 1)
			{
				QMessageBox msgBox;
				msgBox.setText("Ori file already exists: overwrite existing orientation file ?");
				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
				msgBox.setDefaultButton(QMessageBox::Yes);
				int ret = msgBox.exec();
				if (ret == QMessageBox::Cancel) { overwrite_ori=0; }

			}
			

			_ori_file = project_name.c_str();
			_ori_file.append(".ori");
			_ori_fullpath = path.c_str();
			_ori_fullpath.append(_ori_file.c_str());
			int write = 1;
			QString qori(_ori_fullpath.c_str());
			
			if (overwrite_ori == 1)
			{
				//write or overwrite ORI file without further question (0)
				this->SaveORI(qori);
			}
			stream << _ori_file.c_str() << endl;

		}
		if (mesh_exists == 1)
		{
			QMessageBox msgBox;
			msgBox.setText("At least one mesh file already exists: update existing mesh files?");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Yes);
			int ret = msgBox.exec();
			if (ret == QMessageBox::Cancel) { overwrite_mesh = 0; }

			
		}
		if (pos_exists == 1)
		{
			QMessageBox msgBox;
			msgBox.setText("At least one position file already exists: update existing position files?");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Yes);
			int ret = msgBox.exec();
			if (ret == QMessageBox::Cancel) { overwrite_pos = 0; }

			
		}

		if (save_tag == 1)
		{
			std::string _tag_fullpath;
			std::string _tag_file;
			int tag_exists = this->context_file_exists(path, tagExt, project_name);
			if (tag_exists == 1)
			{
				QMessageBox msgBox;
				msgBox.setText("Tag file already exists: overwrite existing tag colours and labels file ?");
				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
				msgBox.setDefaultButton(QMessageBox::Yes);
				int ret = msgBox.exec();
				if (ret == QMessageBox::Cancel) { overwrite_tag = 0; }
				
			}

			_tag_file = project_name.c_str();
			_tag_file.append(".tag");
			_tag_fullpath = path.c_str();
			_tag_fullpath.append(_tag_file.c_str());
			int write = 1;
			
			if (overwrite_tag == 1)
			{
				//write or overwrite TAG file without further question (0)
				//@@ TODO!
				//this->SaveTAG(_tag_fullpath);
			}
			stream << _tag_file.c_str() << endl;
			

		}
		
		//flags 
		vtkIdType selectedflags = this->getFlagLandmarkCollection()->GetNumberOfSelectedActors();
		if (selectedflags>0)
		{
			std::string _flg_fullpath;
			std::string _flg_file;
			int flg_exists = this->context_file_exists(path, flgExt, project_name);
			if (flg_exists == 1)
			{
				QMessageBox msgBox;
				msgBox.setText("Flag file already exists: overwrite existing flag file ?");
				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
				msgBox.setDefaultButton(QMessageBox::Yes);
				int ret = msgBox.exec();
				if (ret == QMessageBox::Cancel) { overwrite_flg = 0; }
				
			}

			_flg_file = project_name.c_str();
			_flg_file.append(".flg");
			_flg_fullpath = path.c_str();
			_flg_fullpath.append(_flg_file.c_str());
			if (overwrite_flg == 1)			
			{
				cout << "Try to save FLG file" << endl;
				//write or overwrite FLG file without further question (0)
				QString qflg(_flg_fullpath.c_str());
				this->SaveFlagFile(qflg, 1);
			}
			stream << _flg_file.c_str() << endl;

		}

		int nselnor = this->NormalLandmarkCollection->GetNumberOfSelectedActors();
		int nseltar = this->TargetLandmarkCollection->GetNumberOfSelectedActors();
		int nselnod = this->NodeLandmarkCollection->GetNumberOfSelectedActors();
		int nselhan = this->HandleLandmarkCollection->GetNumberOfSelectedActors();
		if (nselnor > 0 || nseltar > 0|| nselnod > 0|| nselhan > 0)
		{
			std::string _stv_fullpath;
			std::string _stv_file;
			int stv_exists = this->context_file_exists(path, stvExt, project_name);
			if (stv_exists == 1)
			{
				QMessageBox msgBox;
				msgBox.setText("Landmark/Curve file already exists: overwrite existing STV file ?");
				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
				msgBox.setDefaultButton(QMessageBox::Yes);
				int ret = msgBox.exec();
				if (ret == QMessageBox::Cancel) { overwrite_stv = 0; }
				
			}

			_stv_file = project_name.c_str();
			_stv_file.append(".stv");
			_stv_fullpath = path.c_str();
			_stv_fullpath.append(_stv_file.c_str());
			int write = 1;
			if (overwrite_stv == 1)
			{
				//write or overwrite stv file without further question (0)
				QString qstv(_stv_fullpath.c_str());
				this->SaveSTVFile(qstv,1);
			}
			stream << _stv_file.c_str() << endl;
			
		}

		this->ActorCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMTActor * myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				std::string _mesh_fullpath;
				std::string _pos_fullpath;
				std::string _mesh_file;
				std::string _pos_file;												
				_mesh_file = myActor->GetName();
				_pos_file = myActor->GetName();
				if (save_surfaces_as_ply == 0)
				{
					_mesh_file.append(".vtp");
				}
				else
				{
					_mesh_file.append(".ply");
				}
				_pos_file.append(postfix.c_str());
				_pos_file.append(".pos");

				_mesh_fullpath = path.c_str();
					_mesh_fullpath.append(_mesh_file.c_str());

				_pos_fullpath = path.c_str();
				_pos_fullpath.append(_pos_file.c_str());

				int write = 1;
				if (overwrite_mesh == 0)
				{
					// in that case, check if file exists...								
					ifstream file2(_mesh_fullpath.c_str());
					if (file2)
					{
						write = 0;
						file2.close();
					}
				}
				if (write == 1)
				{
					//int write_type = 0 : binary or binary LE;
					int File_type = 1; //: vtk-vtp

					if (save_surfaces_as_ply == 1) { File_type = 2; }//2 = PLY
	
					this->SaveSurfaceFile(QString(_mesh_fullpath.c_str()), 0, 0, File_type, 0, myActor);
				}

				

				write = 1;
				if (overwrite_pos == 0)
				{
					// in that case, check if file exists...								
					ifstream file2(_pos_fullpath.c_str());
					if (file2)
					{
						write = 0;
						file2.close();
					}
				}

				if (write == 1)
				{
					vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();
					this->SavePOS(Mat, QString(_pos_fullpath.c_str()));
				}
				stream << _mesh_file.c_str() << endl;
				stream << _pos_file.c_str() << endl;
				
				double colors[4];
				myActor->GetmColor(colors);
				stream << colors[0]<<" "<< colors[1]<<" "<< colors[2]<<" "<< colors[3] << endl;

					
				}
				

			}
				
	}
	file.close();
	return 1;
}

void mqMeshToolsCore::SavePOS(vtkSmartPointer<vtkMatrix4x4> Mat, QString fileName)
{

	std::string POSext = ".pos";
	std::string POSext2 = ".POS";
	std::size_t found = fileName.toStdString().find(POSext);
	std::size_t found2 = fileName.toStdString().find(POSext2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".pos");
	}

	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		stream << "1 0 0 0" << endl;
		stream << "0 1 0 0" << endl;
		stream << "0 0 1 0" << endl;
		stream << "0 0 0 1" << endl;


		stream << Mat->GetElement(0, 0) << " " << Mat->GetElement(1, 0) << " " << Mat->GetElement(2, 0) << " " << Mat->GetElement(3, 0) << endl;
		stream << Mat->GetElement(0, 1) << " " << Mat->GetElement(1, 1) << " " << Mat->GetElement(2, 1) << " " << Mat->GetElement(3, 1) << endl;
		stream << Mat->GetElement(0, 2) << " " << Mat->GetElement(1, 2) << " " << Mat->GetElement(2, 2) << " " << Mat->GetElement(3, 2) << endl;
		stream << Mat->GetElement(0, 3) << " " << Mat->GetElement(1, 3) << " " << Mat->GetElement(2, 3) << " " << Mat->GetElement(3, 3) << endl;


	}
	file.close();



}
//should only be done after main window is initialized.
int mqMeshToolsCore::SaveFlagFile(QString fileName, int save_only_selected)
{

	
		std::string FLGext = ".flg";
		std::string FLGext2 = ".FLG";
		std::size_t found = fileName.toStdString().find(FLGext);
		std::size_t found2 = fileName.toStdString().find(FLGext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".flg");
		}
	

	

	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		vtkSmartPointer<vtkLMActorCollection> myColl = vtkSmartPointer<vtkLMActorCollection>::New();
		 myColl = this->FlagLandmarkCollection; 
		

		myColl->InitTraversal();
		for (vtkIdType i = 0; i < myColl->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(myColl->GetNextActor());
			if (myActor->GetSelected() == 1 || save_only_selected == 0)
			{

				double lmpos[3];
				myActor->GetLMOrigin(lmpos);
				double ori[3];
				myActor->GetLMOrientation(ori);
				double lmori[3] = { lmpos[0] + ori[0],lmpos[1] + ori[1] ,lmpos[2] + ori[2] };
				
				stream << myActor->GetLMLabelText()<< endl;
				stream << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << " " 
					<< lmori[0] << " " << lmori[1] << " " << lmori[2] << " " << 
					myActor->GetLMSize()<<" "<<
					myActor->GetmColor()[0] << " "<<
					myActor->GetmColor()[1] << " " <<
					myActor->GetmColor()[2] << " " <<
					endl;
				
					
				

			}

		}




	}
	file.close();
	return 1;

}

int mqMeshToolsCore::SaveSTVFile(QString fileName, int save_only_selected)
{


	std::string STVext = ".stv";
	std::string STVext2 = ".STV";
	std::size_t found = fileName.toStdString().find(STVext);
	std::size_t found2 = fileName.toStdString().find(STVext2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".stv");
	}




	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		vtkSmartPointer<vtkLMActorCollection> myColl = vtkSmartPointer<vtkLMActorCollection>::New();
		
		for (int j = 0; j < 4; j++)
		{
			if (j == 0)
			{
				myColl = this->NormalLandmarkCollection;
			}
			else if (j == 1)
			{
				myColl = this->TargetLandmarkCollection;
			}
			else if (j == 2)
			{
				myColl = this->NodeLandmarkCollection;
			}
			else if (j == 3)
			{
				myColl = this->HandleLandmarkCollection;
			}


			

			if (myColl->GetNumberOfItems() > 0)
			{
				if (save_only_selected == 0)
				{
					stream <<j<<" "<< myColl->GetNumberOfItems() << endl;
				}
				else
				{
					stream << j << " " << myColl->GetNumberOfSelectedActors() << endl;
				}
				myColl->InitTraversal();

				for (vtkIdType i = 0; i < myColl->GetNumberOfItems(); i++)
				{
					vtkLMActor *myActor = vtkLMActor::SafeDownCast(myColl->GetNextActor());
					
					

					if (myActor->GetSelected() == 1 || save_only_selected == 0) 
					{

						double lmpos[3];
						myActor->GetLMOrigin(lmpos);
						
						
						myActor->GetLMOrigin(lmpos);
						double ori[3];
						myActor->GetLMOrientation(ori);
						double lmori[3] = { lmpos[0] + ori[0],lmpos[1] + ori[1] ,lmpos[2] + ori[2] };
						
						if (j < 2)
						{
							stream << "Landmark" << i << ": " << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << " " << lmori[0] << " " << lmori[1] << " " << lmori[2] << " " << endl;
						}
						else if (j==2)
						{
							int lmnodetype = myActor->GetLMNodeType();
							stream << "CurveNode" << i << ": " << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << " " << lmori[0] << " " << lmori[1] << " " << lmori[2] << " " << lmnodetype <<endl;
						}
						else
						{
							stream << "CurveHandle" << i << ": " << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << " " << lmori[0] << " " << lmori[1] << " " << lmori[2] << " " << endl;
						}

						
						

					}

				}
			}
		}




	}
	file.close();
	return 1;

}


//should only be done after main window is initialized.
int mqMeshToolsCore::SaveCURFile(QString fileName, int save_only_selected)
{


	std::string CURext = ".cur";
	std::string CURext2 = ".CUR";
	std::size_t found = fileName.toStdString().find(CURext);
	std::size_t found2 = fileName.toStdString().find(CURext2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".cur");
	}




	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		vtkSmartPointer<vtkLMActorCollection> myCollN = vtkSmartPointer<vtkLMActorCollection>::New();
		vtkSmartPointer<vtkLMActorCollection> myCollH = vtkSmartPointer<vtkLMActorCollection>::New();
		myCollN = this->NodeLandmarkCollection;
		myCollH = this->HandleLandmarkCollection;

		if (myCollN->GetNumberOfItems() != myCollH->GetNumberOfItems()) { return 0; }
		myCollN->InitTraversal();
		myCollH->InitTraversal();
		for (vtkIdType i = 0; i < myCollH->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActorN = vtkLMActor::SafeDownCast(myCollN->GetNextActor());
			vtkLMActor *myActorH = vtkLMActor::SafeDownCast(myCollH->GetNextActor());
			if (myActorN->GetSelected() == 1 || save_only_selected == 0) // we do not care if H is selected or not. We follow "N".
			{

				double lmposN[3];
				myActorN->GetLMOrigin(lmposN);
				double lmposH[3];
				myActorH->GetLMOrigin(lmposH);
				
				int lmnodetype = myActorN->GetLMNodeType();
				stream << "CurvePart"<<i+1<<": "<< lmposN[0] << " " << lmposN[1] << " " << lmposN[2] << " "
					<< lmposH[0] << " " << lmposH[1] << " " << lmposH[2] << " " << lmnodetype<<
					endl;

			}

		}




	}
	file.close();
	return 1;

}

int mqMeshToolsCore::SaveLandmarkFile(QString fileName, int lm_type, int file_type, int save_only_selected)
{

	if (file_type == 0)
	{
		std::string VERext = ".ver";
		std::string VERext2 = ".VER";
		std::size_t found = fileName.toStdString().find(VERext);
		std::size_t found2 = fileName.toStdString().find(VERext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".ver");
		}
	}

	if (file_type == 1)
	{
		std::string LMKext = ".lmk";
		std::string LMKext2 = ".LMK";
		std::size_t found = fileName.toStdString().find(LMKext);
		std::size_t found2 = fileName.toStdString().find(LMKext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".lmk");
		}
	}

	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		vtkSmartPointer<vtkLMActorCollection> myColl = vtkSmartPointer<vtkLMActorCollection>::New();
		if (lm_type == 0) { myColl = this->NormalLandmarkCollection; }
		else if (lm_type == 1) { myColl = this->TargetLandmarkCollection; }
		else if (lm_type == 2) { myColl = this->NodeLandmarkCollection; }
		else  { myColl = this->HandleLandmarkCollection; }
		
		myColl->InitTraversal();
		for (vtkIdType i = 0; i < myColl->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(myColl->GetNextActor());
			if (myActor->GetSelected() == 1 || save_only_selected==0)
			{
				
				double lmpos[3];
				myActor->GetLMOrigin(lmpos);
				double ori[3];
				myActor->GetLMOrientation(ori);
				double lmori[3] = { lmpos[0] + ori[0],lmpos[1] + ori[1] ,lmpos[2] + ori[2] };
				if (file_type == 0)
				{
					stream << "Landmark" << i << ": " << lmpos[0]<<" "<<lmpos[1]<<" "<<lmpos[2]<<" "<<lmori[0]<< " "<< lmori[1] << " " <<  lmori[2] << " " << endl;
				}
				else
				{
					stream << "Landmark" << i << ": " << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << endl;
				}

			}

		}
		
		


	}
	file.close();
	return 1;

}


int mqMeshToolsCore::SaveSurfaceFile(QString fileName, int write_type, int position_mode, int file_type, int save_norms, vtkMTActor *myActor)
{
	// Write_Type 0 : Binary LE or "Default Binary"
	// Write_Type 1 : Binary BE 
	// Write_Type 2 : ASCII
	

	// Position_mode 0 : orignal position
	// Position_mode 1 : moved position

	// File_type 0 : stl
	// File_type 1 : vtk-vtp
	// File_type 2 : ply
	
	// If myActor is NULL : => what will be saved is an aggregation of all selected surface objects.
	// If myActor is not NULL: => what will be saved is the underlying surface object.


	std::string STLext(".stl");
	std::string STLext2(".STL");
	std::string VTPext(".vtk");
	std::string VTPext2(".VTK");
	std::string VTPext3(".vtp");
	std::string VTPext4(".VTP");
	std::string PLYext(".ply");
	std::string PLYext2(".PLY");


	vtkSmartPointer<vtkAppendPolyData> mergedObjects = vtkSmartPointer<vtkAppendPolyData>::New();
	int Ok = 1;

	
	if (myActor == NULL)
	{

		this->ActorCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMTActor *myActor2 = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor2->GetSelected() == 1)
			{
				if (position_mode == 0)
				{
					vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor2->GetMapper());
					if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
					{
						mergedObjects->AddInputData(vtkPolyData::SafeDownCast(mapper->GetInput()));
					}
				}
				else
				{
					vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor2->GetMapper());
					if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
					{
						vtkSmartPointer<vtkPolyData> toSave = vtkSmartPointer<vtkPolyData>::New();
						toSave->DeepCopy(vtkPolyData::SafeDownCast(mapper->GetInput()));
						double ve_init_pos[3];;
						double ve_final_pos[3];
						vtkSmartPointer<vtkMatrix4x4> Mat = myActor2->GetMatrix();


						for (vtkIdType i = 0; i < toSave->GetNumberOfPoints(); i++) {
							// for every triangle 
							toSave->GetPoint(i, ve_init_pos);
							mqMeshToolsCore::TransformPoint(Mat, ve_init_pos, ve_final_pos);

							toSave->GetPoints()->SetPoint((vtkIdType)i, ve_final_pos);
						}
						mergedObjects->AddInputData(toSave);
					}

				}
			}
		}
	}
	else
	{
		vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
		if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
		{
			mergedObjects->AddInputData(vtkPolyData::SafeDownCast(mapper->GetInput()));
		}
	}

	Ok = 1;
	mergedObjects->Update();
	if (save_norms == 0)
	{
		mergedObjects->GetOutput()->GetPointData()->SetNormals(NULL);
		mergedObjects->GetOutput()->GetCellData()->SetNormals(NULL);
	}
	if (file_type == 0)
	{
		vtkSmartPointer<vtkSTLWriter> Writer =
			vtkSmartPointer<vtkSTLWriter>::New();
		if (write_type == 0)
		{
			Writer->SetFileTypeToBinary();
			
		}

		else
		{
			Writer->SetFileTypeToASCII();
		}
		// test if "extension exists!"
		//
		std::size_t found = fileName.toStdString().find(STLext);
		std::size_t found2 = fileName.toStdString().find(STLext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".stl");

		}
		
			Writer->SetFileName(fileName.toStdString().c_str());
			Writer->SetInputData(mergedObjects->GetOutput());
			//  stlWrite->Update();
			Writer->Write();
		
	}

	if (file_type == 1)
	{
		vtkSmartPointer<vtkPolyDataWriter> Writer =
			vtkSmartPointer<vtkPolyDataWriter>::New();
		if (write_type == 0)
		{
			Writer->SetFileTypeToBinary();
			
		}
		else
		{
			Writer->SetFileTypeToASCII();
		}
		std::size_t found = fileName.toStdString().find(VTPext);
		std::size_t found2 = fileName.toStdString().find(VTPext2);
		std::size_t found3 = fileName.toStdString().find(VTPext3);
		std::size_t found4 = fileName.toStdString().find(VTPext4);
		if (found == std::string::npos && found2 == std::string::npos && found3 == std::string::npos && found4 == std::string::npos)
		{
			fileName.append(".vtp");
		}
		
		Writer->SetFileName(fileName.toStdString().c_str());
		Writer->SetInputData(mergedObjects->GetOutput());
		//  stlWrite->Update();
		Writer->Write();
		
	}

	if (file_type == 2)
	{
		vtkSmartPointer<vtkPLYWriter> Writer =
			vtkSmartPointer<vtkPLYWriter>::New();
		if (write_type == 0)
		{
			Writer->SetFileTypeToBinary();
			Writer->SetDataByteOrderToLittleEndian();
			//std::cout << "\nBinary Little endian";
		}
		else if (write_type == 1)
		{
			Writer->SetFileTypeToBinary();
			Writer->SetDataByteOrderToBigEndian();
			// std::cout << "\nBinary Big endian";

		}
		else
		{
			Writer->SetFileTypeToASCII();
			//std::cout << "\nASCII";
		}
		
		vtkPolyData *MyMergedObject = mergedObjects->GetOutput();

		// Test if RGB scalar exists.
		vtkUnsignedCharArray* test = (vtkUnsignedCharArray*)MyMergedObject->GetPointData()->GetScalars("RGB");
		if (test != NULL)
		{
			// std::cout<<"Colors found!"<<std::endl;

			vtkSmartPointer<vtkUnsignedCharArray> colors =
				vtkSmartPointer<vtkUnsignedCharArray>::New();
			colors->SetNumberOfComponents(4);
			colors = (vtkUnsignedCharArray*)MyMergedObject->GetPointData()->GetScalars("RGB");

			vtkSmartPointer<vtkUnsignedCharArray> colorsRGB =
				vtkSmartPointer<vtkUnsignedCharArray>::New();
			colorsRGB->SetNumberOfComponents(3);
			colorsRGB->SetNumberOfTuples(MyMergedObject->GetNumberOfPoints());
			for (int i = 0; i<MyMergedObject->GetNumberOfPoints(); i++)	// for each vertex 
			{			//@@@@@

				int nr, ng, nb;

				nr = colors->GetTuple(i)[0];
				ng = colors->GetTuple(i)[1];
				nb = colors->GetTuple(i)[2];


				colorsRGB->InsertTuple3(i, nr, ng, nb);
			}
			colorsRGB->SetName("Colors");
			MyMergedObject->GetPointData()->AddArray(colorsRGB);
			//colors->SetName("Colors");	  
			std::cout << "Colors num of tuples :" << colors->GetNumberOfTuples() << std::endl;
			for (int i = 0; i<10; i++)
			{
				//std::cout<<"RGB stuff i:"<<colors->GetTuple(i)[0]<<","<<colors->GetTuple(i)[1]<<","<<colors->GetTuple(i)[2]<<std::endl;
				//std::cout<<"RGB "<<i<<"="<<cur_r<<","<<cur_g<<","<<cur_b<<std::endl;
			}
			Writer->SetArrayName("Colors");
		}


		std::size_t found = fileName.toStdString().find(PLYext);
		std::size_t found2 = fileName.toStdString().find(PLYext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".ply");
		}
		ifstream file(fileName.toStdString().c_str());
		
		
			Writer->SetFileName(fileName.toStdString().c_str());
			//
			Writer->SetInputData(MyMergedObject);
			//Writer->SetInputData((vtkPolyData*)My_Obj);	
			Writer->Write();
		
		//std::cout << "\nWriter should have written : "<<filename.c_str();
	}
	return 1;
}

void mqMeshToolsCore::Setmui_LastUsedDir(QString dir) { this->mui_LastUsedDir = dir; }

QString mqMeshToolsCore::Getmui_LastUsedDir() { return this->mui_LastUsedDir; }
void mqMeshToolsCore::Setmui_X1Label(QString label) { this->mui_X1Label = label;
//cout << "this->mui_X1Label " << this->mui_X1Label.toStdString() << endl;
}
QString mqMeshToolsCore::Getmui_DefaultX1Label() { return this->mui_DefaultX1Label; }
QString mqMeshToolsCore::Getmui_X1Label() { return this->mui_X1Label; }


void mqMeshToolsCore::Setmui_X2Label(QString label) { this->mui_X2Label = label; 
//cout << "this->mui_X2Label " << this->mui_X2Label.toStdString() << endl;
}
QString mqMeshToolsCore::Getmui_DefaultX2Label() { return this->mui_DefaultX2Label; }
QString mqMeshToolsCore::Getmui_X2Label() { return this->mui_X2Label; }

void mqMeshToolsCore::Setmui_Y1Label(QString label) { this->mui_Y1Label = label; 
//cout << "this->mui_Y1Label " << this->mui_Y1Label.toStdString() << endl;
}
QString mqMeshToolsCore::Getmui_DefaultY1Label() { return this->mui_DefaultY1Label; }
QString mqMeshToolsCore::Getmui_Y1Label() { return this->mui_Y1Label; }


void mqMeshToolsCore::Setmui_Y2Label(QString label) { this->mui_Y2Label = label; 

//cout << "this->mui_Y2Label " << this->mui_Y2Label.toStdString() << endl;
}
QString mqMeshToolsCore::Getmui_DefaultY2Label() { return this->mui_DefaultY2Label; }
QString mqMeshToolsCore::Getmui_Y2Label() { return this->mui_Y2Label; }

void mqMeshToolsCore::Setmui_Z1Label(QString label) { this->mui_Z1Label = label; }
QString mqMeshToolsCore::Getmui_DefaultZ1Label() { return this->mui_DefaultZ1Label; }
QString mqMeshToolsCore::Getmui_Z1Label() { return this->mui_Z1Label; }


void mqMeshToolsCore::Setmui_Z2Label(QString label) { this->mui_Z2Label = label; }
QString mqMeshToolsCore::Getmui_DefaultZ2Label() { return this->mui_DefaultZ2Label; }
QString mqMeshToolsCore::Getmui_Z2Label() { return this->mui_Z2Label; }

void mqMeshToolsCore::CreateLandmark(double coord[3], double ori[3], int lmk_type, int node_type)
{
	
	cout << "CreateLandmark:" << lmk_type <<", "<< node_type << endl;
	// lmk_type : 0 for normal landmarks
	// lmk_type : 1 for target landmarks
	// lmk_type : 2 for curve nodes
	// lmk_type : 3 for curve handles
	// lmk_type : 4 for flag landmark


	//node_type: only used if mode ==2, curve node
	//node_type: 1 curve starting point 
	//node_type: 0 normal node
	//node_type: 2 curve milestone
	//node_type: 3 connect to preceding starting point

	VTK_CREATE(vtkLMActor, myLM);
	int num = 0;
	myLM->SetLMType(lmk_type);

	if (lmk_type == NORMAL_LMK)
	{
		num = this->NormalLandmarkCollection->GetNextLandmarkNumber();
	}
	else if (lmk_type == TARGET_LMK)
	{
		num = this->TargetLandmarkCollection->GetNextLandmarkNumber();
	}
	else if (lmk_type == NODE_LMK)
	{
		num = this->NodeLandmarkCollection->GetNextLandmarkNumber();
	}
	else if (lmk_type == HANDLE_LMK)
	{
		num = this->HandleLandmarkCollection->GetNextLandmarkNumber();
	}
	else if (lmk_type== FLAG_LMK)
	{
		num = this->FlagLandmarkCollection->GetNextLandmarkNumber();
	}

	myLM->SetLMOriginAndOrientation(coord, ori);
	//myLM->SetLMOrigin(pos[0], pos[1], pos[2]);
	//myLM->SetLMOrientation(norm[0], norm[1], norm[2]);
	if (lmk_type != FLAG_LMK)
	{
		if (mqMeshToolsCore::instance()->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			myLM->SetLMSize(mqMeshToolsCore::instance()->AdjustedLandmarkSize());
		}
		else
		{
			myLM->SetLMSize(mqMeshToolsCore::instance()->Getmui_LandmarkRenderingSize());
		}
	}
	else
	{
		myLM->SetLMSize(mqMeshToolsCore::instance()->Getmui_FlagRenderingSize());
	}
	/*
		double green[4] = { 0.5, 1, 0, 1 }; // LMType=0
	double yellow[4] = { 1, 1, 0,0.5 }; // LMType = 1 (target LM)
	double darkred[4] = { 0.5, 0, 0, 1 }; // LMType = 2 (curve node: dark red)
	double orange[4] = { 1, 0.5, 0, 1 }; // LMType = 3 (curve handle : orange)
	double red[4] = { 1, 0.4, 0.4, 1 }; // LMType=4 (curve starting point)
	double blue[4] = { 0, 0.5, 1, 1 }; // LMType = 5 (curve milestone)
	double cyan[4] = { 0, 1, 1, 1 }; // LMType = 6 (curve ending point)
	*/
	if (lmk_type == NORMAL_LMK)
	{
		myLM->SetLMType(NORMAL_LMK);
	}
	else if (lmk_type== TARGET_LMK)
	{
		myLM->SetLMType(TARGET_LMK);
	}
	else if (lmk_type == NODE_LMK)
	{
		// to do : 
		myLM->SetLMType(NODE_LMK);
		if (node_type>-1)
		{ 
			//lmtype: 1 curve starting point
			//lmtype: 0 normal node
			//lmtype: 2 curve milestone
			//lmtype: 3 connect to preceding starting point
			if (node_type == NORMAL_NODE) { myLM->SetLMType(NODE_LMK);  myLM->SetLMNodeType(NORMAL_NODE);	}
			if (node_type == STARTING_NODE) { myLM->SetLMType(NODE_LMK);  myLM->SetLMNodeType(STARTING_NODE); }
			if (node_type == MILESTONE_NODE) { myLM->SetLMType(NODE_LMK);  myLM->SetLMNodeType(MILESTONE_NODE); }
			if (node_type == CONNECT_NODE) { myLM->SetLMType(NODE_LMK);  myLM->SetLMNodeType(CONNECT_NODE); }
			


		}
		else
		{			
			if (num > 1)
			{
				vtkLMActor *myPrecedingLM = NULL;
				//@implement GETLMBefore(num)
				//vtkLMActor *myPrecedingLM = this->NodeLandmarkCollection->GetLMBefore(num);
				if (myPrecedingLM != NULL)
				{
					if (myPrecedingLM->GetLMNodeType() == CONNECT_NODE)// if curve ending point
					{
						myLM->SetLMNodeType(STARTING_NODE); // curve starting point
					}
					else
					{
						myLM->SetLMNodeType(NORMAL_NODE); // curve conventional node
					}
				}
				else
				{
					myLM->SetLMNodeType(NORMAL_NODE); // curve conventional node
				}
			}
			else // num ==1
			{
				myLM->SetLMNodeType(STARTING_NODE); //curve starting point
			}
		}
	}
	else if(lmk_type == HANDLE_LMK)
	{
		myLM->SetLMType(HANDLE_LMK); //curve handle
	}
	else 
	{
		cout << "Set LM TYPE TO FLAG!" << endl;
		myLM->SetLMType(FLAG_LMK);
		std::string flag = "Flag ";
		std::string flag_num = flag + std::to_string(num);
		myLM->SetLMText(flag_num);
	}
	
	myLM->SetLMNumber(num);
	
	myLM->SetLMBodyType(mqMeshToolsCore::instance()->Getmui_LandmarkBodyType());
	
	myLM->SetSelected(0);

	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(myLM->getLMBody());
	mapper->Update();
	myLM->SetMapper(mapper);


	//myLM->PrintSelf(cout, vtkIndent(1));
	

	END_UNDO_SET();
	if (lmk_type == NORMAL_LMK)
	{
		
		
		this->NormalLandmarkCollection->AddItem(myLM);
		this->NodeLandmarkCollection->ReorderLandmarks();
		this->NormalLandmarkCollection->SetChanged(1);
		std::string action = "Create Normal landmark";
		int mCount = BEGIN_UNDO_SET(action);
		mqMeshToolsCore::instance()->getNormalLandmarkCollection()->CreateLoadUndoSet(mCount, 1);
		END_UNDO_SET();
		
	}
	else if (lmk_type == TARGET_LMK)
	{

		this->TargetLandmarkCollection->AddItem(myLM);
		this->NodeLandmarkCollection->ReorderLandmarks();
		this->TargetLandmarkCollection->SetChanged(1);
		std::string action = "Create Target landmark";
		int mCount = BEGIN_UNDO_SET(action);
		mqMeshToolsCore::instance()->getTargetLandmarkCollection()->CreateLoadUndoSet(mCount, 1);
		END_UNDO_SET();
		
	}
	else if (lmk_type == NODE_LMK)
	{
		this->NodeLandmarkCollection->AddItem(myLM);
		this->NodeLandmarkCollection->ReorderLandmarks();
		this->NodeLandmarkCollection->SetChanged(1);
		std::string action = "Create Curve Node";
		int mCount = BEGIN_UNDO_SET(action);
		mqMeshToolsCore::instance()->getNodeLandmarkCollection()->CreateLoadUndoSet(mCount, 1);
		END_UNDO_SET();
		
	}
	else if (lmk_type == HANDLE_LMK)
	{
		this->HandleLandmarkCollection->AddItem(myLM);
		this->HandleLandmarkCollection->ReorderLandmarks();
		this->HandleLandmarkCollection->SetChanged(1);
		std::string action = "Create Curve Handle";
		int mCount = BEGIN_UNDO_SET(action);
		mqMeshToolsCore::instance()->getHandleLandmarkCollection()->CreateLoadUndoSet(mCount, 1);
		END_UNDO_SET();
	}
	else if (lmk_type == FLAG_LMK)
	{
		this->FlagLandmarkCollection->AddItem(myLM);		
		this->FlagLandmarkCollection->SetChanged(1);
		std::string action = "Create Flag Landmark";
		int mCount = BEGIN_UNDO_SET(action);
		mqMeshToolsCore::instance()->getFlagLandmarkCollection()->CreateLoadUndoSet(mCount, 1);
		END_UNDO_SET();
	}
	
	
}
void mqMeshToolsCore::ResetOrientationHelperLabels()
{
	this->SetOrientationHelperLabels(this->mui_X1Label.toStdString(), this->mui_X2Label.toStdString(), this->mui_Y1Label.toStdString(), this->mui_Y2Label.toStdString(), this->mui_Z1Label.toStdString(), this->mui_Z2Label.toStdString());

}
void mqMeshToolsCore::SetOrientationHelperLabels(std::string X1, std::string X2, std::string Y1, std::string Y2, std::string Z1, std::string Z2 )
{
	vtkSmartPointer<vtkOrientationHelperActor> axes = vtkOrientationHelperActor::SafeDownCast(this->OrientationHelperWidget->GetOrientationMarker());
	axes->SetXAxisLabelText(X1.c_str());
	axes->SetX2AxisLabelText(X2.c_str());
	axes->SetYAxisLabelText(Y1.c_str());
	axes->SetY2AxisLabelText(Y2.c_str());
	axes->SetZAxisLabelText(Z1.c_str());
	axes->SetZ2AxisLabelText(Z2.c_str());
}
void mqMeshToolsCore::InitializeOrientationHelper()
{
	vtkSmartPointer<vtkOrientationHelperActor> axes =
		vtkSmartPointer<vtkOrientationHelperActor>::New();

	//

	//vtkOrientationMarkerWidget* widget = vtkOrientationMarkerWidget::New();
	// Does not work with a smart pointer, can't figure out why


	this->OrientationHelperWidget->SetOutlineColor(0.9300, 0.5700, 0.1300);
	this->OrientationHelperWidget->SetOrientationMarker(axes);
	this->OrientationHelperWidget->SetDefaultRenderer(this->getRenderer());
	this->OrientationHelperWidget->SetInteractor(this->RenderWindow->GetInteractor());
	this->OrientationHelperWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
	this->OrientationHelperWidget->SetEnabled(1);
	this->OrientationHelperWidget->InteractiveOff();
	this->OrientationHelperWidget->PickingManagedOn();
	this->ResetOrientationHelperLabels();


}
void mqMeshToolsCore::SetMainWindow(QMainWindow *_mainWindow)
{
	this->MainWindow = _mainWindow;
}
QMainWindow* mqMeshToolsCore::GetMainWindow() {
	return this->MainWindow	;
}

//Called to repplace camera and grid positions when switching from "orange grid mode" to "blue grid mode"
//= when camera focalpoint and grid center are changed between 0,0,0 and COM of all opened meshes.
void mqMeshToolsCore::ReplaceCameraAndGrid()
{
	double newcamerafocalpoint[3] = { 0,0,0 };
	if (this->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		this->getActorCollection()->GetCenterOfMass(newcamerafocalpoint);
	}

	double oldcampos[3];
	double newcampos[3];
	this->getCamera()->GetPosition(oldcampos);
	double oldcamerafocalpoint[3];
	this->getCamera()->GetFocalPoint(oldcamerafocalpoint);

	double dispvector[3];
	vtkMath::Subtract(newcamerafocalpoint, oldcamerafocalpoint, dispvector);
	vtkMath::Add(oldcampos, dispvector, newcampos);
	this->getCamera()->SetPosition(newcampos);
	this->getCamera()->SetFocalPoint(newcamerafocalpoint);

	this->getGridActor()->SetGridOrigin(newcamerafocalpoint);
	this->getGridActor()->SetOutlineMode(this->Getmui_CameraCentreOfMassAtOrigin());
	//this->getGridActor()->SetGridType(gridtype);	
	this->Render();


}
void mqMeshToolsCore::AdjustCameraAndGrid()
{
	double newcamerafocalpoint[3] = { 0,0,0 };
	if (this->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		this->getActorCollection()->GetCenterOfMass(newcamerafocalpoint);
		this->getGridActor()->SetGridOrigin(newcamerafocalpoint);


	}

	double multfactor = 1 / tan(this->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	double GlobalBoundingBoxLength = this->getActorCollection()->GetBoundingBoxLength();
	if (GlobalBoundingBoxLength == std::numeric_limits<double>::infinity() || GlobalBoundingBoxLength == 0)
	{
		GlobalBoundingBoxLength = 120;
	}

	double oldcampos[3];
	double newcampos[3];
	this->getCamera()->GetPosition(oldcampos);
	double oldcamerafocalpoint[3];
	this->getCamera()->GetFocalPoint(oldcamerafocalpoint);

	double dispvector[3];

	vtkMath::Subtract(oldcampos, oldcamerafocalpoint, dispvector);
	vtkMath::Normalize(dispvector);
	double newdist = multfactor*GlobalBoundingBoxLength;
	vtkMath::MultiplyScalar(dispvector, newdist);

	vtkMath::Add(newcamerafocalpoint, dispvector, newcampos);

	this->getCamera()->SetPosition(newcampos);
	this->getCamera()->SetFocalPoint(newcamerafocalpoint);

	// now adjust if necessary..
	if (this->Getmui_CameraOrtho() == 1)
	{
		this->getCamera()->SetParallelScale(GlobalBoundingBoxLength);
		this->getRenderer()->ResetCameraClippingRange();
	}

	//this->ui->qvtkWidget->update();
	this->Render();



}

/*void mqMeshToolsCore::RedrawBezierCurves()
{
	
	//this->BezierCurveSource->Update();
	cout << "Render" << endl;
	this->Render();
}*/
void mqMeshToolsCore::ResetCameraOrthoPerspective()
{
	if (this->Getmui_CameraOrtho() == 1)
	{
		this->getCamera()->SetParallelProjection(true);
		this->DollyCameraForParallelScale();
	}
	else
	{

		this->getCamera()->SetParallelProjection(false);
		this->DollyCameraForPerspectiveMode();


	}
	//cout << "Parallel scale"<<this->MeshToolsCore->getCamera()->GetParallelScale()<<endl;
	double dist = 0;


	double campos[3] = { 0,0,0 };
	double foc[3] = { 0,0,0 };
	this->getCamera()->GetPosition(campos);
	//cout << "Camera Position:" << campos[0] <<","<<campos[1]<<","<<campos[2]<< endl;
	this->getCamera()->GetFocalPoint(foc);
	//cout << "Camera Position:" << foc[0] << "," << foc[1] << "," << foc[2] << endl;
	dist = sqrt(pow((campos[0] - foc[0]), 2) + pow((campos[1] - foc[1]), 2) + pow((campos[2] - foc[2]), 2));
	//cout << "Distance between camera and focal point:" << dist << endl;

	//cout << "Camera viewing angle:" << this->MeshToolsCore->getCamera()->GetViewAngle() << endl;

	this->Render(); // update main window!
}
/*
In perspective mode, "zoom" (dolly) in/out changes the position of the camera
("dolly" functions of vtkInteractorStyleTrackballCamera.cxx and of vtkInteractorStyleJoystickCamera )
Beware : no real "Zoom" function is applied in these styles!!!!
=> before I create  MeshTools' own interactor styles, camera's parallel scale (=ortho "zoom") should
be updated when switching from "perspective" to "ortho" to keep track of that change...
=> Once these styles are created, this function should be removed!

*/
void mqMeshToolsCore::DollyCameraForParallelScale()
{
	double campos[3] = { 0,0,0 };
	double foc[3] = { 0,0,0 };

	this->getCamera()->GetPosition(campos);
	this->getCamera()->GetFocalPoint(foc);
	double dist = sqrt(vtkMath::Distance2BetweenPoints(campos, foc));
	//double dist = sqrt(pow((campos[0] - foc[0]), 2) + pow((campos[1] - foc[1]), 2) + pow((campos[2] - foc[2]), 2));
	double multfactor = 1 / tan(this->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);

	double newparallelscale = dist / multfactor;
	this->getCamera()->SetParallelScale(newparallelscale);

}

/*
In parallel mode, "zoom" (dolly) in/out does not change the position of the camera
("dolly" functions of vtkInteractorStyleTrackballCamera.cxx and of vtkInteractorStyleJoystickCamera )
Beware : no real "Zoom" function is applied in these styles!!!!
=> before I create  MeshTools' own interactor styles, camera's position in perspective mode should
be updated when switching from "ortho" to "perspective" to keep track of that change...
=> Once these styles are created, this function should be removed!

*/
void mqMeshToolsCore::DollyCameraForPerspectiveMode()
{
	double campos[3] = { 0,0,0 };
	double foc[3] = { 0,0,0 };
	double dispvector[3];
	this->getCamera()->GetPosition(campos);
	this->getCamera()->GetFocalPoint(foc);
	double multfactor = 3.73; // at 30° vtk : angle = 2*atan((h/2)/d). 
							  // then 2*d  =12/tan(viewangle/2) 
	multfactor = 1 / tan(this->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	//cout << "DollyCameraForPerspectiveMode" << endl;
	//cout << "multfactor" << multfactor << endl;
	//cout << "Old posisition:" << campos[0] << "," << campos[1] << "," << campos[2] << endl;

	vtkMath::Subtract(campos, foc, dispvector);
	//cout<<"Disp Vector:" << dispvector[0] << ","<<dispvector[1] << "," << dispvector[2] << endl;
	vtkMath::Normalize(dispvector);
	//cout << "Normalized Disp Vector:" << dispvector[0] << "," << dispvector[1] << "," << dispvector[2] << endl;

	double newdist = multfactor*this->getCamera()->GetParallelScale();
	//cout << "New dist:" << newdist << endl;
	vtkMath::MultiplyScalar(dispvector, newdist);
	//cout << "Multiplied Disp Vector:" << dispvector[0] << "," << dispvector[1] << "," << dispvector[2] << endl;
	double newpos[3] = { 0,0,0 };
	vtkMath::Add(foc, dispvector, newpos);
	//cout << "New pos:" << newpos[0] << "," << newpos[1] << "," << newpos[2] << endl;

	this->getCamera()->SetPosition(newpos);



}

//On ajoute un indice au nom si le nom existe déjà.
//fonction recurente pour savoir quel indice lui donner.
std::string  mqMeshToolsCore::CheckingName(std::string name_obj) {
	//cout << "check: " << name_obj << endl;
	int cpt_name = 1;
	std::string s_cpt_name = std::to_string(cpt_name);
	std::string name_base = name_obj;
	std::string postfix = "";
	size_t nPos = name_obj.find_first_of("(");
	if (nPos >0 &&nPos<= name_obj.length())
	{
		//cout << "nPos=" << nPos << endl;		
		name_base = name_base.substr(0, nPos);
	}
	//cout << "name_base: " << name_base << endl;
	
	this->ActorCollection->InitTraversal();
	
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor * myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
		std::string a_name_base = myActor->GetName();
		//cout << "actor name: " << a_name_base << endl;
		std::string apostfix = "";
		size_t anPos = a_name_base.find_first_of("(");
		//cout << "AnPos:" << anPos << endl;
		if (anPos > 0 && anPos <= a_name_base.length())
		{
			a_name_base = a_name_base.substr(0, anPos);
			
		}
		//cout << "a_name_base: " << a_name_base<<endl;
		if (a_name_base.compare(name_base) ==0) {// si il existe déjà, on augmente l'indice
			
			cpt_name++;			
			s_cpt_name = std::to_string(cpt_name);
			name_obj = name_base + "(" + s_cpt_name+")";
		}

	}

	
	
	return name_obj;
}

void mqMeshToolsCore::SetGridVisibility()
{
	vtkPropCollection* props = this->getRenderer()->GetViewProps(); //iterate through and set each visibility to 0
	props->InitTraversal();
	std::string str1("vtkGridActor");
	for (int i = 0; i < props->GetNumberOfItems(); i++)
	{
		vtkProp *myprop = props->GetNextProp();
		if (str1.compare(myprop->GetClassName()) == 0)
		{
			if (this->Getmui_ShowGrid() == 1)
			{
				myprop->VisibilityOn();
			}
			else
			{
				myprop->VisibilityOff();
			}
		}

	}
	this->Render();
}
void mqMeshToolsCore::SetOrientationHelperVisibility()
{

	//std::string str1("vtkOrientationHelperActor");
	if (this->Getmui_ShowOrientationHelper() == 1)
	{
		this->OrientationHelperWidget->GetOrientationMarker()->VisibilityOn();
	}
	else
	{
		this->OrientationHelperWidget->GetOrientationMarker()->VisibilityOff();
	}
	this->Render();
}

vtkMTActor * mqMeshToolsCore::GetFirstSelectedActor()
{
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			return myActor;
		}
	}

	return NULL;
}

vtkMTActor* mqMeshToolsCore::GetLastActor()
{
	return vtkMTActor::SafeDownCast(this->getActorCollection()->GetLastActor());
}

vtkLMActor* mqMeshToolsCore::GetLastLandmark(int mode)
{
	if (mode == 0) {return vtkLMActor::SafeDownCast(this->getNormalLandmarkCollection()->GetLastActor());
	}
	else if (mode == 1) {
		return vtkLMActor::SafeDownCast(this->getTargetLandmarkCollection()->GetLastActor());
	}
	else if (mode == 2) {
		return vtkLMActor::SafeDownCast(this->getNodeLandmarkCollection()->GetLastActor());
	}
	else if (mode == 3) {
		return vtkLMActor::SafeDownCast(this->getHandleLandmarkCollection()->GetLastActor());
	}
	//else if (mode == 4) {
	else  {
		return vtkLMActor::SafeDownCast(this->getFlagLandmarkCollection()->GetLastActor());
	}
	
}

void mqMeshToolsCore::ApplyMatrix(vtkSmartPointer<vtkMatrix4x4> Mat, int mode)
{
	// mode : 0 for last inserted mesh
	// mode : 1 for all selected meshes
	// mode : 2 for all selected landmarks/flags
	
	if (mode == 0)
	{
		vtkMTActor *actor = this->GetLastActor();
		actor->ApplyMatrix(Mat);
	}
	else
	{
		if (mode == 1 )
		{
			this->ActorCollection->InitTraversal();
			for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
			{
				vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
				if (myActor->GetSelected() == 1)
				{
					myActor->ApplyMatrix(Mat);;
					myActor->SetSelected(0);
				}
			}

		}
		else // mode ==2
		{

			this->NormalLandmarkCollection->InitTraversal();
			for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
			{
				vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
				if (myActor->GetSelected() == 1 )
				{
					myActor->ApplyMatrix(Mat);;
					myActor->SetSelected(0);
				}
			}
			this->TargetLandmarkCollection->InitTraversal();
			for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
			{
				vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
				if (myActor->GetSelected() == 1)
				{
					myActor->ApplyMatrix(Mat);;
					myActor->SetSelected(0);
				}
			}
			this->NodeLandmarkCollection->InitTraversal();
			for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
			{
				vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
				if (myActor->GetSelected() == 1)
				{
					myActor->ApplyMatrix(Mat);;
					myActor->SetSelected(0);
				}
			}
			this->HandleLandmarkCollection->InitTraversal();
			for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
			{
				vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
				if (myActor->GetSelected() == 1)
				{
					myActor->ApplyMatrix(Mat);;
					myActor->SetSelected(0);
				}
			}
			this->FlagLandmarkCollection->InitTraversal();
			for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
			{
				vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
				if (myActor->GetSelected() == 1)
				{
					myActor->ApplyMatrix(Mat);;
					myActor->SetSelected(0);
				}
			}
		}
	}
}

void mqMeshToolsCore::SelectAll(int Count)
{}
void mqMeshToolsCore::UnselectAll(int Count)
{
	int node_handle_collections_changed = 0;
	
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1 &&Count>0)
		{
			myActor->SaveState(Count);
		}
	}
	this->NormalLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1&&Count>0)
		{
			myActor->SaveState(Count);
		}
	}
	this->TargetLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1 && Count>0)
		{
			myActor->SaveState(Count);

		}
	}
	this->NodeLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1 && Count>0)
		{
			myActor->SaveState(Count);
			node_handle_collections_changed = 1;
		}
	}
	this->HandleLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1 && Count>0)
		{
			myActor->SaveState(Count);
			node_handle_collections_changed = 1;
		}
	}
	this->FlagLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1 && Count>0)
		{
			myActor->SaveState(Count);
		}
	}

	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);
			myActor->SetChanged(1);

		}


	}
	this->NormalLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);
			myActor->SetChanged(1);

		}


	}
	this->TargetLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);
			myActor->SetChanged(1);

		}


	}
	this->NodeLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);
			myActor->SetChanged(1);

		}


	}
	this->HandleLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);
			myActor->SetChanged(1);

		}


	}
	this->FlagLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);
			myActor->SetChanged(1);

		}


	}
	if (node_handle_collections_changed == 1)
	{
		this->NodeLandmarkCollection->Modified();
		this->HandleLandmarkCollection->Modified();
	}
}

void mqMeshToolsCore::Render()
{
	this->RenderWindow->Render();
}

void mqMeshToolsCore::Setmui_ShowOrientationHelper(int orientationHelper) { this->mui_ShowOrientationHelper = orientationHelper; }
int mqMeshToolsCore::Getmui_DefaultShowOrientationHelper() { return this->mui_DefaultShowOrientationHelper; }
int mqMeshToolsCore::Getmui_ShowOrientationHelper() { return this->mui_ShowOrientationHelper; }

void mqMeshToolsCore::Setmui_CameraCentreOfMassAtOrigin(int comao) { this->mui_CameraCentreOfMassAtOrigin = comao; }
int mqMeshToolsCore::Getmui_DefaultCameraCentreOfMassAtOrigin() { return this->mui_DefaultCameraCentreOfMassAtOrigin; }
int mqMeshToolsCore::Getmui_CameraCentreOfMassAtOrigin() { return this->mui_CameraCentreOfMassAtOrigin; }


void mqMeshToolsCore::Setmui_CameraOrtho(int ortho) { this->mui_CameraOrtho = ortho; }
int mqMeshToolsCore::Getmui_DefaultCameraOrtho() { return this->mui_DefaultCameraOrtho; }
int mqMeshToolsCore::Getmui_CameraOrtho() { return this->mui_CameraOrtho; }

void mqMeshToolsCore::Setmui_ShowGrid(int showgrid) { this->mui_ShowGrid = showgrid; }
int mqMeshToolsCore::Getmui_ShowGrid() { return this->mui_ShowGrid; }
int mqMeshToolsCore::Getmui_DefaultShowGrid() { return this->mui_DefaultShowGrid; };

int mqMeshToolsCore::Getmui_DefaultAnaglyph() { return this->mui_DefaultAnaglyph; }
int mqMeshToolsCore::Getmui_Anaglyph() { return this->mui_Anaglyph; }
void mqMeshToolsCore::Setmui_Anaglyph(int anaglyph)
{
	this->mui_Anaglyph = anaglyph;
	if (this->RenderWindow != NULL)
	{
		if (anaglyph == 1)
		{
			this->RenderWindow->StereoRenderOn();
			this->RenderWindow->Render();
		}
		else
		{
			this->RenderWindow->StereoRenderOff();
			this->RenderWindow->Render();
		}
	}
}


void mqMeshToolsCore::Setmui_LandmarkBodyType(int type) {
	this->mui_LandmarkBodyType = type; 
	this->UpdateLandmarkSettings();
}
int mqMeshToolsCore::Getmui_DefaultLandmarkBodyType() { return this->mui_DefaultLandmarkBodyType; }
int mqMeshToolsCore::Getmui_LandmarkBodyType() { return this->mui_LandmarkBodyType; }


void mqMeshToolsCore::Setmui_LandmarkMode(int mode) {
	this->mui_LandmarkMode = mode;
	//this->UpdateLandmarkSettings();
}
int mqMeshToolsCore::Getmui_DefaultLandmarkMode() { return this->mui_DefaultLandmarkMode; }
int mqMeshToolsCore::Getmui_LandmarkMode() { return this->mui_LandmarkMode; }


void mqMeshToolsCore::Setmui_LandmarkRenderingSize(double size)
{ this->mui_LandmarkRenderingSize = size;
this->UpdateLandmarkSettings();
}
double mqMeshToolsCore::Getmui_DefaultLandmarkRenderingSize() { return this->mui_DefaultLandmarkRenderingSize; }
double mqMeshToolsCore::Getmui_LandmarkRenderingSize() { return this->mui_LandmarkRenderingSize; }

void mqMeshToolsCore::Setmui_AdjustLandmarkRenderingSize(int adjust)
{
	this->mui_AdjustLandmarkRenderingSize = adjust;
	this->UpdateLandmarkSettings();
}
int mqMeshToolsCore::Getmui_DefaultAdjustLandmarkRenderingSize() { return this->mui_DefaultAdjustLandmarkRenderingSize; }
int mqMeshToolsCore::Getmui_AdjustLandmarkRenderingSize() { return this->mui_AdjustLandmarkRenderingSize; }

void mqMeshToolsCore::Setmui_AdjustScaleFactor(double factor) {
	this->mui_AdjustScaleFactor = factor; 
	this->UpdateLandmarkSettings();
}
double mqMeshToolsCore::Getmui_DefaultAdjustScaleFactor() { return this->mui_DefaultAdjustScaleFactor; }
double mqMeshToolsCore::Getmui_AdjustScaleFactor() { return this->mui_AdjustScaleFactor; }


void mqMeshToolsCore::Setmui_FlagRenderingSize(double size) { 
	this->mui_FlagRenderingSize = size; 
	cout << mui_FlagRenderingSize << endl;
}
double mqMeshToolsCore::Getmui_DefaultFlagRenderingSize() { return this->mui_DefaultFlagRenderingSize; }

double mqMeshToolsCore::Getmui_FlagRenderingSize() { return this->mui_FlagRenderingSize; 
//cout << "Default f r z" << this->mui_FlagRenderingSize<<endl;
}



double* mqMeshToolsCore::Getmui_MeshColor() { return this->mui_MeshColor; }
void mqMeshToolsCore::Getmui_MeshColor(double c[4])
{
	double *co = this->Getmui_MeshColor();

	c[0] = co[0];
	c[1] = co[1];
	c[2] = co[2];
	c[3] = co[3];
}
double* mqMeshToolsCore::Getmui_DefaultMeshColor() { return this->mui_DefaultMeshColor; }
void mqMeshToolsCore::Getmui_DefaultMeshColor(double c[4])
{
	double *co = this->Getmui_DefaultMeshColor();

	c[0] = co[0];
	c[1] = co[1];
	c[2] = co[2];
	c[3] = co[3];
}
void mqMeshToolsCore::Setmui_MeshColor(double c1, double c2, double c3, double c4)
{
	double c[4];
	c[0] = c1;
	c[1] = c2;
	c[2] = c3;
	c[3] = c4;
	

	this->Setmui_MeshColor(c);
}
void mqMeshToolsCore::Setmui_MeshColor(double c[4])
{
	this->mui_MeshColor[0] = c[0];
	this->mui_MeshColor[1] = c[1];
	this->mui_MeshColor[2] = c[2];
	this->mui_MeshColor[3] = c[3];
	//cout << "Core: this->mui_MeshColor[3]="<<this->mui_MeshColor[3] << endl;
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected()==1)
		{
			
			myActor->SetmColor(this->mui_MeshColor);
			myActor->SetSelected(0);
		}
	}
}

double* mqMeshToolsCore::Getmui_FlagColor() { return this->mui_FlagColor; }
void mqMeshToolsCore::Getmui_FlagColor(double c[3])
{
	double *co = this->Getmui_FlagColor();

	c[0] = co[0];
	c[1] = co[1];
	c[2] = co[2];	
}

double* mqMeshToolsCore::Getmui_DefaultFlagColor() { return this->mui_DefaultFlagColor; }

void mqMeshToolsCore::Getmui_DefaultFlagColor(double c[3])
{
	double *co = this->Getmui_DefaultFlagColor();

	c[0] = co[0];
	c[1] = co[1];
	c[2] = co[2];
	
}
void mqMeshToolsCore::Setmui_FlagColor(double c1, double c2, double c3)
{
	double c[3];
	c[0] = c1;
	c[1] = c2;
	c[2] = c3;
	


	this->Setmui_FlagColor(c);
}
void mqMeshToolsCore::Setmui_FlagColor(double c[3])
{
	this->mui_FlagColor[0] = c[0];
	this->mui_FlagColor[1] = c[1];
	this->mui_FlagColor[2] = c[2];
	
	//cout << "Core: this->mui_MeshColor[3]="<<this->mui_MeshColor[3] << endl;
	/*this->FlagCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{

			//myActor->SetmColor(this->mui_FlagColor);
			//myActor->SetSelected(0);
		}
	}*/
}



double* mqMeshToolsCore::Getmui_BackGroundColor() { return this->mui_BackGroundColor; }
void mqMeshToolsCore::Getmui_BackGroundColor(double bg[3])
{
	double *bgr = this->Getmui_BackGroundColor();

	bg[0] = bgr[0];
	bg[1] = bgr[1];
	bg[2] = bgr[2];
	
}


double* mqMeshToolsCore::Getmui_DefaultBackGroundColor() { return this->mui_DefaultBackGroundColor; }
void mqMeshToolsCore::Getmui_DefaultBackGroundColor(double bg[3])
{
	double *bgr = this->Getmui_DefaultBackGroundColor();

	bg[0] = bgr[0];
	bg[1] = bgr[1];
	bg[2] = bgr[2];

}


double* Getmui_DefaultBackGroundColor();
void Getmui_DefaultBackGroundColor(double bg[3]);
void mqMeshToolsCore::Setmui_BackGroundColor(double bg1, double bg2, double bg3)
{
	double background[3];
	background[0] = bg1;
	background[1] = bg2;
	background[2] = bg3;

	this->Setmui_BackGroundColor(background);
}
void mqMeshToolsCore::Setmui_BackGroundColor(double background[3])
{
	this->mui_BackGroundColor[0] = background[0];
	this->mui_BackGroundColor[1] = background[1];
	this->mui_BackGroundColor[2] = background[2];
	this->Renderer->SetBackground(background);
	//this->RenderWindow->Render();
}

double* mqMeshToolsCore::Getmui_BackGroundColor2() { return this->mui_BackGroundColor2; }
void mqMeshToolsCore::Getmui_BackGroundColor2(double bg[3])
{
	double *bgr = this->Getmui_BackGroundColor2();

	bg[0] = bgr[0];
	bg[1] = bgr[1];
	bg[2] = bgr[2];

}


double* mqMeshToolsCore::Getmui_DefaultBackGroundColor2() { return this->mui_DefaultBackGroundColor2; }
void mqMeshToolsCore::Getmui_DefaultBackGroundColor2(double bg[3])
{
	double *bgr = this->Getmui_DefaultBackGroundColor2();

	bg[0] = bgr[0];
	bg[1] = bgr[1];
	bg[2] = bgr[2];

}

void mqMeshToolsCore::Setmui_BackGroundColor2(double bg1, double bg2, double bg3)
{
	double background[3];
	background[0] = bg1;
	background[1] = bg2;
	background[2] = bg3;

	this->Setmui_BackGroundColor2(background);
}
void mqMeshToolsCore::Setmui_BackGroundColor2(double background[3])
{
	this->mui_BackGroundColor2[0] = background[0];
	this->mui_BackGroundColor2[1] = background[1];
	this->mui_BackGroundColor2[2] = background[2];
	this->Renderer->SetBackground2(background);
	//this->RenderWindow->Render();
}
double mqMeshToolsCore::AdjustedLandmarkSize()
{

	double bbl = this->ActorCollection->GetBoundingBoxLength();
	
	double adjusted_size = this->Getmui_AdjustScaleFactor()*bbl / 50;
	if (adjusted_size > 0 && bbl < DBL_MAX)
	{
		return adjusted_size;
	}
	else
	{
		if (this->Getmui_LandmarkRenderingSize() > 0)
		{
			return this->Getmui_LandmarkRenderingSize();

		}
		else
		{
			return this->Getmui_DefaultLandmarkRenderingSize();
		}
	}

}
void mqMeshToolsCore::UpdateLandmarkSettings(vtkLMActor *myActor)
{
	myActor->SetLMBodyType(this->Getmui_LandmarkBodyType());
	if (myActor->GetLMType() != FLAG_LMK)
	{

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			//myActor->SetLMSize(this->Getmui_LandmarkRenderingSize());
			myActor->SetLMSize(this->AdjustedLandmarkSize());
		}
		else
		{
			myActor->SetLMSize(this->Getmui_LandmarkRenderingSize());
			//Change landmark size for all landmarks but flags.

		}
	}
	else
	{
		//Do not change size!!!
		myActor->SetLMSize(myActor->GetLMSize());
	}


	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(myActor->getLMBody());
	mapper->Update();
	myActor->SetMapper(mapper);

}
void mqMeshToolsCore::UpdateLandmarkSettings()
{
	this->NormalLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
		UpdateLandmarkSettings(myActor);
		
	}
	this->TargetLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
		UpdateLandmarkSettings(myActor);

	}
	this->NodeLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
		UpdateLandmarkSettings(myActor);

	}
	this->HandleLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
		UpdateLandmarkSettings(myActor);

	}
	this->FlagLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
		UpdateLandmarkSettings(myActor);

	}
}

void mqMeshToolsCore::Undo()
{
	// a Set is only a label (action) and an id
	//vtkUndoSet *MyUndoSet = this->UndoStack->GetNextUndoSet();
	//this->ActorCollection->Undo(MySet);
	//cout << "Root Undo!" << endl;
	this->UndoStack->undo(); // removes the next undo set.. 

}
void mqMeshToolsCore::Undo(int Count)
{
	//cout << "Undo(" <<Count<<")"<< endl;
	//Calls for the Undo method in vtkActorCollection for this particular Count etc.. 
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
	//	cout << "MyActor undo!" << endl;
		myActor->Undo(Count);		
	}
	this->ActorCollection->Undo(Count);

	
	this->NormalLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Undo(Count);
	}
	// To update to take into account reorder!
	this->NormalLandmarkCollection->Undo(Count);

	this->TargetLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Undo(Count);
	}
	// To update to take into account reorder!
	this->TargetLandmarkCollection->Undo(Count);

	this->NodeLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Undo(Count);
	}
	//@@ dirty! Recompute bezier curve whatever we do!
	// To update to take into account reorder!
	this->NodeLandmarkCollection->Undo(Count);
	this->NodeLandmarkCollection->ReorderLandmarks();
	this->NodeLandmarkCollection->Modified();

	this->HandleLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Undo(Count);
	}
	
	// To update to take into account reorder!
	this->HandleLandmarkCollection->Undo(Count);

	//@@ dirty! Recompute bezier curve whatever we do!
	this->HandleLandmarkCollection->ReorderLandmarks();
	this->HandleLandmarkCollection->Modified();


	this->FlagLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Undo(Count);
	}
	// To update to take into account reorder!
	this->FlagLandmarkCollection->Undo(Count);
}
void mqMeshToolsCore::Redo()
{
	//cout << "Root Redo!" << endl;
	this->UndoStack->redo(); // removes the next undo set.. 
}

void mqMeshToolsCore::Redo(int Count)
{
	//cout << "Redo(" << Count << ")" << endl;
	//Calls for the Undo method in vtkActorCollection for this particular Count etc.. 
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
		//cout << "MyActor Redo!" << endl;
		myActor->Redo(Count);
	}
	this->ActorCollection->Redo(Count);

	
	this->NormalLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor redo from core!" << endl;
		myActor->Redo(Count);
	}
	// To update to take into account reorder!
	this->NormalLandmarkCollection->Redo(Count);

	this->TargetLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Redo(Count);
	}
	// To update to take into account reorder!
	this->TargetLandmarkCollection->Redo(Count);

	this->NodeLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Redo(Count);
	}
	// To update to take into account reorder!
	this->NodeLandmarkCollection->Redo(Count);
	//@@ dirty! Recompute bezier curve whatever we do!
	this->NodeLandmarkCollection->ReorderLandmarks();
	this->NodeLandmarkCollection->Modified();

	this->HandleLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Redo(Count);
	}

	// To update to take into account reorder!
	this->HandleLandmarkCollection->Redo(Count);
	//@@ dirty! Recompute bezier curve whatever we do!
	this->HandleLandmarkCollection->ReorderLandmarks();
	this->HandleLandmarkCollection->Modified();

	this->FlagLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Redo(Count);
	}
	// To update to take into account reorder!
	this->FlagLandmarkCollection->Redo(Count);

}

void mqMeshToolsCore::Erase(int Count)
{
	//cout << "Erase(" << Count << ")" << endl;
	//Calls for the Undo method in vtkActorCollection for this particular Count etc.. 
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMTActor *myActor = vtkMTActor::SafeDownCast(this->ActorCollection->GetNextActor());
		//cout << "MyActor Erase!" << endl;
		myActor->Erase(Count);
	}
	this->ActorCollection->Erase(Count);

	this->NormalLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor redo from core!" << endl;
		myActor->Erase(Count);
	}
	// To update to take into account reorder!
	this->NormalLandmarkCollection->Erase(Count);

	this->TargetLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Erase(Count);
	}
	// To update to take into account reorder!
	this->TargetLandmarkCollection->Erase(Count);

	this->NodeLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Erase(Count);
	}
	// To update to take into account reorder!
	this->NodeLandmarkCollection->Erase(Count);

	this->HandleLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Erase(Count);
	}
	// To update to take into account reorder!
	this->HandleLandmarkCollection->Erase(Count);

	this->FlagLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Erase(Count);
	}
	// To update to take into account reorder!
	this->FlagLandmarkCollection->Erase(Count);


}
void mqMeshToolsCore::setUndoStack(mqUndoStack* stack)
{
	if (stack != this->UndoStack)
	{
		this->UndoStack = stack;
		/*if (stack)
		{
			stack->setParent(this);
		}*/
		//emit this->undoStackChanged(stack);
	}
}

//-----------------------------------------------------------------------------
mqMeshToolsCore::~mqMeshToolsCore()
{
	//this->ActorCollection->Delete();
	if (mqMeshToolsCore::Instance == this)
	{
		mqMeshToolsCore::Instance = 0;
	}
}
/*vtkSmartPointer<vtkUndoStack> mqMeshToolsCore::getUndoStack()
{
	return this->UndoStack;
}*/

mqUndoStack* mqMeshToolsCore::getUndoStack()
{
return this->UndoStack;
}

vtkSmartPointer<vtkActor> mqMeshToolsCore::getBezierActor()
{
	return this->BezierActor;
}
vtkSmartPointer<vtkActor> mqMeshToolsCore::getBezierSelectedActor()
{
	return this->BezierSelectedActor;
}
vtkSmartPointer<vtkActor> mqMeshToolsCore::getBezierNHActor()
{
	return this->BezierNHActor;
}
vtkSmartPointer<vtkBezierCurveSource> mqMeshToolsCore::getBezierCurveSource()
{
	return this->BezierCurveSource;
}
vtkSmartPointer<vtkMTActorCollection> mqMeshToolsCore::getActorCollection()
{
	return this->ActorCollection;
}
vtkSmartPointer<vtkLMActorCollection> mqMeshToolsCore::getNormalLandmarkCollection()
{
	return this->NormalLandmarkCollection;
}
vtkSmartPointer<vtkLMActorCollection> mqMeshToolsCore::getTargetLandmarkCollection()
{
	return this->TargetLandmarkCollection;
}
vtkSmartPointer<vtkLMActorCollection> mqMeshToolsCore::getNodeLandmarkCollection()
{
	return this->NodeLandmarkCollection;
}
vtkSmartPointer<vtkLMActorCollection> mqMeshToolsCore::getHandleLandmarkCollection()
{
	return this->HandleLandmarkCollection;
}
vtkSmartPointer<vtkLMActorCollection> mqMeshToolsCore::getFlagLandmarkCollection()
{
	return this->FlagLandmarkCollection;
}
/*
vtkMTActorCollection* mqMeshToolsCore::getActorCollection()
{
	return this->ActorCollection;
}*/

vtkSmartPointer<vtkRenderer> mqMeshToolsCore::getRenderer()
{
	return this->Renderer;
}
vtkSmartPointer<vtkCamera> mqMeshToolsCore::getCamera()
{
	return this->Camera;
}
vtkSmartPointer<vtkGridActor> mqMeshToolsCore::getGridActor()
{
	return this->GridActor;
}

void mqMeshToolsCore::TransformPoint(vtkMatrix4x4* matrix, double pointin[3], double pointout[3]) {
	double pointPred[4]; double pointNew[4] = { 0, 0, 0, 0 };
	pointPred[0] = pointin[0];
	pointPred[1] = pointin[1];
	pointPred[2] = pointin[2];
	pointPred[3] = 1;

	matrix->MultiplyPoint(pointPred, pointNew);
	pointout[0] = pointNew[0];
	pointout[1] = pointNew[1];
	pointout[2] = pointNew[2];
}
