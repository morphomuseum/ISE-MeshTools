#include "SpecialRoller4.h"
/*#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Slider.H>
#include <stdio.h>
#include <string.h>*/


SpecialRoller4::SpecialRoller4(int x,int y,int w,int h,const char* label)
: Fl_Value_Roller(x, y, w, h, label) {
 //end(); 
}

int SpecialRoller4::handle(int event) 
{
  //std::cout<<"event"<<event<<std::endl;
  int handled = 1;
  Fl_Group* Pointeur0 = (Fl_Group*)(this->parent());
  
  Fl_Group* Pointeur1 = (Fl_Group*)(Pointeur0->parent());
  Pointeur0 = (Fl_Group*)(Pointeur1->parent());
  MeshTools* Pointeur = (MeshTools*)(Pointeur0->child(0));
  
  switch(event) {
	 // std::cout<<"event"<<event<<std::endl;
  case FL_RELEASE:
    /*Pointeur->rollinit_camera();
	Pointeur->rollinit_objects();

	//this->maximum(this->max);
	//this->minimum(this->min);*/
	Pointeur->rollinit_camera();
	Pointeur->rollinit_objects();
	Pointeur->redraw();
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

int SpecialRoller4::changevalue(float newval)
 {
	/* this->delta =  newval - this->value();
	this->maximum(this->maximum() + this->delta);
	this->minimum(this->minimum() + this->delta);
	this->value(newval);*/
	this->maximum(180);
	this->minimum(-180);
	this->value(0);
	this->redraw();
	return 1;
 }