//
// "$Id: Fl_Value_Roller.cxx,v 1.5.2.4.2.5 2004/04/11 04:38:58 easysw Exp $"
//
// Value roller widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//


#include "Fl_Value_Roller.h";
#include <stdio.h>
#include <iostream>
//#include "SpecialRoller2.h";
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <math.h>


Fl_Value_Roller::Fl_Value_Roller(int X, int Y, int W, int H, const char*l)
: Fl_Roller(X,Y,W,H,l) {
  step(1,100);
  textfont_ = FL_HELVETICA;
  textsize_ = 10;
  textcolor_ = FL_BLACK;
}

void Fl_Value_Roller::draw() {
  int sxx = x(), syy = y(), sww = w(), shh = h();
  int bxx = x(), byy = y(), bww = w(), bhh = h();
  if (horizontal()) {
    bww = 35; sxx += 35; sww -= 35;
  } else {
    syy += 25; bhh = 25; shh -= 25;
  }
  if (damage()&FL_DAMAGE_ALL) draw_box(box(),sxx,syy,sww,shh,color());
  Fl_Roller::draw();
  draw_box(box(),bxx,byy,bww,bhh,color());
  char buf[128];
  format(buf);
  fl_font(textfont(), textsize());
  fl_color(active_r() ? textcolor() : fl_inactive(textcolor()));
  fl_draw(buf, bxx, byy, bww, bhh, FL_ALIGN_CLIP);
}

int Fl_Value_Roller::handle(int event) {
  //std::cout<<"Fl_value_roller event"<<event<<std::endl;
  int sxx = x(), syy = y(), sww = w(), shh = h();
  if (horizontal()) {
    sxx += 35; sww -= 35;
  } else {
    syy += 25; shh -= 25;
  }
  return Fl_Roller::handle(event);
}

//
// End of "$Id: Fl_Value_Roller.cxx,v 1.5.2.4.2.5 2004/04/11 04:38:58 easysw Exp $".
//
