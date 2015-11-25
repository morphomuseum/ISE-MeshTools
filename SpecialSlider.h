//#include <config.h>
//#include "Struct.h"
#ifndef _INC_SPSLIDER
#define _INC_SPSLIDER


#include <FL/Fl.H>
#include <FL/Fl_Slider.H>

class  SpecialSlider : public  Fl_Slider {
public:
	SpecialSlider(int x, int y, int w, int h, const char *label = 0);
	int handle(int event);
	int changevalue(float new_val);
	float min; float max; float val;
	float delta;


    /* Draw the cube boundaries
     * Draw the faces of the cube using the boxv[] vertices, using
     * GL_LINE_LOOP for the faces. The color is #defined by
     * CUBECOLOR.
     */
    
    
};
//The Special Slider class
#endif  /* _INC_SPSLIDER*/

