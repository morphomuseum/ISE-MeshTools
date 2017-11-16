//
// "$Id: MeshToolsMain.cxx,v 1.2.2.5.2.4 2004/04/11 04:39:00 easysw Exp $"
//
// MeshToolsUI class definitions for the Fast Light Tool Kit (FLTK).
//
//

#include "config.h"
#include <ctime>

#include <FL/Fl.H>
#define GLEW_STATIC
#include <GL/glew.h>
#include <FL/gl.h>
//#include "Struct.h"
#include "MeshToolsUI.h"
//#include "Struct.h"

void myGLInit(void)
{
  GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_position[] = { 0.0, 0.3, 0.3, 0.0 };

  GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_NORMALIZE);
#if 0
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
#endif

  glShadeModel(GL_SMOOTH);
#if 0
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

//  SetupMaterials();
//  CreateTreeLists();

  glFlush();
} 
int CharToInt(char d)
{
	char c[2];
	c[0] = d;
	c[1] = '\0';
	
	if (strcmp(c, "0")==0){return 0;}
	if (strcmp(c, "1")==0){return 1;}
	if (strcmp(c, "2")==0){return 2;}
	if (strcmp(c, "3")==0){return 3;}
	if (strcmp(c, "4")==0){return 4;}
	if (strcmp(c, "5")==0){return 5;}
	if (strcmp(c, "6")==0){return 6;}
	if (strcmp(c, "7")==0){return 7;}
	if (strcmp(c, "8")==0){return 8;}
	if (strcmp(c, "9")==0){return 9;}
	if (strcmp(c, "A")==0){return 10;}
	if (strcmp(c, "B")==0){return 11;}
	if (strcmp(c, "C")==0){return 12;}
	if (strcmp(c, "D")==0){return 13;}
	if (strcmp(c, "E")==0){return 14;}
	if (strcmp(c, "F")==0){return 15;}	
	return 0;
}




int main(int argc, char **argv) {
	//0.7 = initial 64 bits version
	//0.8 = write a few filters
	//1.0.1 => update_rgb missing dans lasso tag et fill holes
	//1.0.3 => pb with path
	//1.0.4 => small behavious with selected objets
	//      => norms saved inside .ply files
	//      => .flg formats : normal direction normalized.
	// 1.0.5 => does not crash when selecting thousands of objets
	// 1.2.1 => version which should compile under linux, thanks to Stephan
	// 1.3 => crossplatform version (windows, linux, mac), VBO colours corrections, 
	// MeshTools.ini is now located at a user's specific location + when non existing default values are created
	// possibility to reinitialize a tag colours and labels, and object/grid/background colours
	// camera now can rotate around objects center of mass (and not only around 0,0,0), 
	// automatically adjustable landmark display size
	// project files now include tags/landmarks/flags/curves/orientation labels/ 
	// 1.3.1 => Zoom correction (should function correctly with very large and very small objects)
	//       => Colours should be saved correctly now within .PLY files
	//       => Possibility to edit landmarks coordinate
	//       => Possibility to add landmarks at user-defined xyz coordinates.
	// 1.3.2 => Possibility to apply a position/transposed positions to landmarks and flags
	//       => Edit flags 1 by 1
	//       => Get colour of closest vertex for 1 flag or for all flags.
	//       => Some colours are rendered badly for flags... where does it come from ?
	// 1.3.3 => Open .ntw file know accepts file names and paths containing spaces... + minor compatibility issues with vtk 7.X.X
	// 1.3.4 => Acknowledgements to FoRM-IT in the About window

	strcpy (Version,"1.3.4");	
 
	printf("Version : %s\n",Version);
	
	

	
    MeshToolsUI *MeshToolsui=new MeshToolsUI;	
     //MeshToolsui->show(argc, argv);
	//glewInit();
     //return Fl::run();
	

	time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
	int year = now->tm_year + 1900;
	int month = now->tm_mon + 1;
	//printf ("Year : %d Month:%d\n",year, month);
	
	

		// Fl::visual(FL_DOUBLE|FL_INDEX);
		if (argc ==1)
		{
			MeshToolsui->show(argc, argv);
			return Fl::run();
		}
		else
		{
			exit(0);
		}
	
	
}


//
// End of "$Id: CubeMain.cxx,v 1.2.2.5.2.4 2004/04/11 04:39:00 easysw Exp $".
//
