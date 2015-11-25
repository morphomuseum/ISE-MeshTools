#include "SpecialRoller3.h"


/*#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Slider.H>
#include <stdio.h>
#include <string.h>*/


SpecialRoller3::SpecialRoller3(int x,int y,int w,int h,const char* label)
: Fl_Value_Roller(x, y, w, h, label) {
 //end(); 
}



int SpecialRoller3::handle(int event) 
{
  int handled = 1;
  
  switch(event) {
  case FL_RELEASE:
	this->delta =  this->value() - this->val;
	this->maximum(this->value()/2);
	this->minimum(this->value() + this->value()/2);
	this->step((this->maximum()- this->minimum())/200);
	
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

int SpecialRoller3::changevalue(float newval)
 {
	
	 this->delta =  newval - this->value();
	 if (newval<0){newval = -newval;}
 	 this->value(newval);
 
	 this->minimum(5*this->value()/2);
		
	
	this->maximum(0.5*this->value()/2);
	
	//this->minimum(this->value() + this->value()/2);
	this->step((this->maximum()- this->minimum())/100);

	
	this->redraw();
	return 1;
 }