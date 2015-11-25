
//
// "$Id: SpecialRoller2.h,v 1.1.1.1 2005/08/02 15:26:40 renaud Exp $"
//
// Roller header file for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl.H>
#include <FL/Fl_Roller.H>
#include <FL/Fl_Group.H>
#include "MeshTools.h"
#ifndef _INC_SPROLLER2
#define _INC_SPROLLER2


class  SpecialRoller2 : public  Fl_Roller {

public:
SpecialRoller2(int x,int y,int w,int h,const char* label=0);
  int handle(int);
  
  int changevalue(float new_val);
	float min; float max; float val;
	float delta;
};

#endif

//

