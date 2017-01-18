/*=========================================================================

   Program: MeshTools
   Module:    copied and adapted from Paraview pqUndoStack.cxx

   Copyright (c) 2005-2008 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "pqUndoStack.h"

#include "vtkMeshToolsCore.h"

#include "vtkUndoStack.h"
#include "vtkUndoStackBuilder.h"
#include <vtkEventQtSlotConnect.h>
#include <QPointer>
#include <QtDebug>

//-----------------------------------------------------------------------------
class mqUndoStack::mqImplementation
{
public:
  mqImplementation() { this->NestedCount = 0; }
  vtkSmartPointer<vtkUndoStack> UndoStack;
  vtkSmartPointer<vtkUndoStackBuilder> UndoStackBuilder;
  vtkSmartPointer<vtkEventQtSlotConnect> VTKConnector;
   QList<bool> IgnoreAllChangesStack;
  int NestedCount;
};

//-----------------------------------------------------------------------------
pqUndoStack::pqUndoStack(vtkUndoStackBuilder* builder, QObject* _parent /*=null*/)
  : QObject(_parent)
{
  this->Implementation = new mqImplementation();
  this->Implementation->UndoStack = vtkSmartPointer<vtkUndoStack>::New();

  if (builder)
  {
    this->Implementation->UndoStackBuilder = builder;
  }
  else
  {
    // create default builder.
    builder = vtkUndoStackBuilder::New();
    this->Implementation->UndoStackBuilder = builder;
    builder->Delete();
  }

  builder->SetUndoStack(this->Implementation->UndoStack);

  this->Implementation->VTKConnector = vtkSmartPointer<vtkEventQtSlotConnect>::New();
  this->Implementation->VTKConnector->Connect(this->Implementation->UndoStack,
    vtkCommand::ModifiedEvent, this, SLOT(onStackChanged()), NULL, 1.0);
}

//-----------------------------------------------------------------------------
mqUndoStack::~mqUndoStack()
{
  delete this->Implementation;
}

//-----------------------------------------------------------------------------
vtkUndoStackBuilder* mqUndoStack::GetUndoStackBuilder()
{
  return this->Implementation->UndoStackBuilder;
}

//-----------------------------------------------------------------------------
bool mqUndoStack::canUndo()
{
  return this->Implementation->UndoStack->CanUndo();
}

//-----------------------------------------------------------------------------
bool mqUndoStack::canRedo()
{
  return this->Implementation->UndoStack->CanRedo();
}

//-----------------------------------------------------------------------------
const QString mqUndoStack::undoLabel()
{
  return this->Implementation->UndoStack->CanUndo()
    ? this->Implementation->UndoStack->GetUndoSetLabel(0)
    : QString();
}

//-----------------------------------------------------------------------------
const QString mqUndoStack::redoLabel()
{
  return this->Implementation->UndoStack->CanRedo()
    ? this->Implementation->UndoStack->GetRedoSetLabel(0)
    : QString();
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void mqUndoStack::onStackChanged()
{
  bool can_undo = false;
  bool can_redo = false;
  QString undo_label;
  QString redo_label;
  if (this->Implementation->UndoStack->CanUndo())
  {
    can_undo = true;
    undo_label = this->Implementation->UndoStack->GetUndoSetLabel(0);
  }
  if (this->Implementation->UndoStack->CanRedo())
  {
    can_redo = true;
    redo_label = this->Implementation->UndoStack->GetRedoSetLabel(0);
  }

  emit this->stackChanged(can_undo, undo_label, can_redo, redo_label);
  emit this->canUndoChanged(can_undo);
  emit this->canRedoChanged(can_redo);
  emit this->undoLabelChanged(undo_label);
  emit this->redoLabelChanged(redo_label);
}

//-----------------------------------------------------------------------------
void mqUndoStack::beginUndoSet(QString label)
{
  if (this->Implementation->NestedCount == 0)
  {
    this->Implementation->UndoStackBuilder->Begin(label.toLatin1().data());
  }

  this->Implementation->NestedCount++;
}

//-----------------------------------------------------------------------------
void mqUndoStack::endUndoSet()
{
  if (this->Implementation->NestedCount == 0)
  {
    qDebug() << "endUndoSet called without a beginUndoSet.";
    return;
  }

  this->Implementation->NestedCount--;
  if (this->Implementation->NestedCount == 0)
  {
    this->Implementation->UndoStackBuilder->EndAndPushToStack();
  }
}

//-----------------------------------------------------------------------------
void mqUndoStack::undo()
{
  this->Implementation->UndoStack->Undo();

//  vtkMeshToolsCore::instance()->render();

  emit this->undone();
}

//-----------------------------------------------------------------------------
void mqUndoStack::redo()
{
  this->Implementation->UndoStack->Redo();
 
  // vtkMeshToolsCore::instance()->render();

  emit this->redone();
}


//-----------------------------------------------------------------------------
void pqUndoStack::clear()
{
  this->Implementation->UndoStack->Clear();
  this->Implementation->UndoStackBuilder->Clear();
  this->Implementation->IgnoreAllChangesStack.clear();
}



//-----------------------------------------------------------------------------
bool mqUndoStack::ignoreAllChanges() const
{
	return this->Implementation->UndoStackBuilder->GetIgnoreAllChanges();
}