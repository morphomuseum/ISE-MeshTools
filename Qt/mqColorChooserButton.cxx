/*=========================================================================

   Program: ParaView
   Module:  mqColorChooserButton.cxx

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

// self includes
#include "mqColorChooserButton.h"

// Qt includes
#include <QColorDialog>
#include <QPainter>
#include <QResizeEvent>

//-----------------------------------------------------------------------------
mqColorChooserButton::mqColorChooserButton(QWidget* p)
  : QToolButton(p)
  , ShowAlphaChannel(false)
{
  this->Color[0] = 0.0;
  this->Color[1] = 0.0;
  this->Color[2] = 0.0;
  this->Color[3] = 1.0;
  this->IconRadiusHeightRatio = 0.75;
  this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  this->connect(this, SIGNAL(clicked()), SLOT(chooseColor()));
}

//-----------------------------------------------------------------------------
QColor mqColorChooserButton::chosenColor() const
{
  QColor color;
  color.setRgbF(this->Color[0], this->Color[1], this->Color[2], this->Color[3]);

  return color;
}

//-----------------------------------------------------------------------------
QVariantList mqColorChooserButton::chosenColorRgbF() const
{
  QVariantList val;
  val << this->Color[0] << this->Color[1] << this->Color[2];
  return val;
}

//-----------------------------------------------------------------------------
QVariantList mqColorChooserButton::chosenColorRgbaF() const
{
  QVariantList val;
  val << this->Color[0] << this->Color[1] << this->Color[2] << this->Color[3];
  return val;
}

//-----------------------------------------------------------------------------
void mqColorChooserButton::setChosenColor(const QColor& color)
{
  if (color.isValid())
  {
    QVariantList val;
    val << color.redF() << color.greenF() << color.blueF() << color.alphaF();
    this->setChosenColorRgbaF(val);
  }
}

//-----------------------------------------------------------------------------
void mqColorChooserButton::setChosenColorRgbF(const QVariantList& val)
{
  Q_ASSERT(val.size() == 3);
  QColor color;
  color.setRgbF(val[0].toDouble(), val[1].toDouble(), val[2].toDouble());

  if (color.isValid())
  {
    if (val[0].toDouble() != this->Color[0] || val[1].toDouble() != this->Color[1] ||
      val[2].toDouble() != this->Color[2])
    {
      this->Color[0] = val[0].toDouble();
      this->Color[1] = val[1].toDouble();
      this->Color[2] = val[2].toDouble();

      this->setIcon(this->renderColorSwatch(color));
      emit this->chosenColorChanged(color);
    }

    emit this->validColorChosen(color);
  }
}

//-----------------------------------------------------------------------------
void mqColorChooserButton::setChosenColorRgbaF(const QVariantList& val)
{
  Q_ASSERT(val.size() == 4);
  QColor color;
  color.setRgbF(val[0].toDouble(), val[1].toDouble(), val[2].toDouble(), val[3].toDouble());

  if (color.isValid())
  {
    if (val[0].toDouble() != this->Color[0] || val[1].toDouble() != this->Color[1] ||
      val[2].toDouble() != this->Color[2] || val[3].toDouble() != this->Color[3])
    {
      this->Color[0] = val[0].toDouble();
      this->Color[1] = val[1].toDouble();
      this->Color[2] = val[2].toDouble();
      this->Color[3] = val[3].toDouble();

      this->setIcon(this->renderColorSwatch(color));
      emit this->chosenColorChanged(color);
    }
    emit this->validColorChosen(color);
  }
}

//-----------------------------------------------------------------------------
QIcon mqColorChooserButton::renderColorSwatch(const QColor& color)
{
  int radius = qRound(this->height() * this->IconRadiusHeightRatio);
  if (radius <= 10)
  {
    radius = 10;
  }

  QPixmap pix(radius, radius);
  pix.fill(QColor(0, 0, 0, 0));

  QPainter painter(&pix);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setBrush(QBrush(color));
  painter.drawEllipse(1, 1, radius - 2, radius - 2);
  painter.end();
  return QIcon(pix);
}

//-----------------------------------------------------------------------------
void mqColorChooserButton::chooseColor()
{
  QColorDialog::ColorDialogOptions opts;
  if (this->ShowAlphaChannel)
  {
    opts |= QColorDialog::ShowAlphaChannel;
  }

  this->setChosenColor(QColorDialog::getColor(this->chosenColor(), this, "Select Color", opts));
}

//-----------------------------------------------------------------------------
void mqColorChooserButton::resizeEvent(QResizeEvent* rEvent)
{
  (void)rEvent;

  QColor color = this->chosenColor();
  this->setIcon(this->renderColorSwatch(color));
}
