#include "SpecialSlider4.h"
/*#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Slider.H>
#include <stdio.h>
#include <string.h>*/


SpecialSlider4::SpecialSlider4(int x, int y, int w, int h, const char *label)
: Fl_Value_Slider(x, y, w, h, label) {
 //end(); 
}

int SpecialSlider4::handle(int event) 
{
  int handled = 1;
  
  switch(event) {
  case FL_RELEASE:
	this->value(0);
	
	break;
  case FL_PUSH:
    this->max = this->maximum();
	this->min = this->minimum();
	this->val = this->value();
  default:
    // pass other events to the base class...
	  handled= Fl_Value_Slider::handle(event);
  }
 
  
  this->redraw();
  return (handled);  // Returning one acknowledges that we handled this event
}

int SpecialSlider4::changevalue(float newval)
 {
	
		 this->delta =  newval - this->value();
	this->maximum(this->maximum() + this->delta);
	this->minimum(this->minimum() + this->delta);
	this->value(newval);
	this->redraw();
	
	
	return 1;
 }