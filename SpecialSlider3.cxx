#include "SpecialSlider3.h"
/*#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Slider.H>
#include <stdio.h>
#include <string.h>*/


SpecialSlider3::SpecialSlider3(int x, int y, int w, int h, const char *label)
: Fl_Slider(x, y, w, h, label) {
 //end(); 
}

int SpecialSlider3::handle(int event) 
{
  int handled = 1;
  
  switch(event) {
  case FL_RELEASE:
	this->delta =  (float)this->value() - this->val;
	this->maximum(this->value()/2);
	this->minimum(this->value() + this->value()/2);
	
	break;
  case FL_PUSH:
    this->max = (float)this->maximum();
	this->min = (float)this->minimum();
	this->val = (float)this->value();
  default:
    // pass other events to the base class...
	  handled= Fl_Slider::handle(event);
  }
 
  
  this->redraw();
  return (handled);  // Returning one acknowledges that we handled this event
}

int SpecialSlider3::changevalue(float newval)
 {
	 this->delta =  newval - (float)this->value();
	this->maximum(this->value()/2);
	this->minimum(this->value() + this->value()/2);
	this->value(newval);
	this->redraw();
	return 1;
 }