#include "SpecialRoller2.h"
/*#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Slider.H>
#include <stdio.h>
#include <string.h>*/


SpecialRoller2::SpecialRoller2(int x,int y,int w,int h,const char* label)
: Fl_Roller(x, y, w, h, label) {
 //end(); 
}

int SpecialRoller2::handle(int event) 
{
  int handled = 1;
  Fl_Group* Pointeur0 = (Fl_Group*)(this->parent());
  Fl_Group* Pointeur1 = (Fl_Group*)(Pointeur0->parent());
  MeshTools* Pointeur = (MeshTools*)(Pointeur1->child(0));
  
  switch(event) {
  case FL_RELEASE:
    Pointeur->rollinit_camera();
	Pointeur->rollinit_objects();

	//this->maximum(this->max);
	//this->minimum(this->min);
	this->value(0);
	break;
	
	break;
  case FL_PUSH:
    this->max = this->maximum();
	this->min = this->minimum();
	this->val = this->value();
  default:
    // pass other events to the base class...
	  handled= Fl_Roller::handle(event);
  }
 
  
  this->redraw();
  return (handled);  // Returning one acknowledges that we handled this event
}

int SpecialRoller2::changevalue(float newval)
 {
	 this->delta =  newval - this->value();
	this->maximum(this->maximum() + this->delta);
	this->minimum(this->minimum() + this->delta);
	this->value(newval);
	this->redraw();
	return 1;
 }