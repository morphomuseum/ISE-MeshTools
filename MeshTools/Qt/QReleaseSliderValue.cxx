#include <QtWidgets>

#include "QReleaseSliderValue.h"

QReleaseSliderValue::QReleaseSliderValue(Qt::Orientation orientation, const QString &title,
	QWidget *parent)
	: QGroupBox(title, parent)
{
	this->setFlat(true);
	this->spinbox = new QSpinBox;
	this->spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->spinbox->setDisabled(true);
	this->spinbox->setFixedWidth(20);
	//this->slider = new QReleaseSlider(orientation);
	this->slider = new QSlider(orientation);
	//this->slider->setFocusPolicy(Qt::StrongFocus);
	//this->slider->setTickPosition(QSlider::TicksBothSides);
	//this->slider->setTickInterval(10);
	this->slider->setSingleStep(1);

	
	
	connect(slider, SIGNAL(valueChanged(int)), spinbox, SLOT(setValue(int)));
	connect(slider, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));

	QBoxLayout::Direction direction;

	if (orientation == Qt::Vertical)
	{
		direction = QBoxLayout::TopToBottom;
	}
	else
	{
		direction = QBoxLayout::LeftToRight;
	}

	QBoxLayout *slidersLayout = new QBoxLayout(direction);
	
	slidersLayout->addWidget(slider);
	slidersLayout->addWidget(spinbox);
	setLayout(slidersLayout);
	this->setContentsMargins(0, 0, 0, 0);
	//this->setFixedWidth(30);
}

void QReleaseSliderValue::setValue(int value)
{
	slider->setValue(value);
}

void QReleaseSliderValue::setMinimum(int value)
{
	slider->setMinimum(value);
	spinbox->setMinimum(value);
}

void QReleaseSliderValue::setMaximum(int value)
{
	slider->setMaximum(value);
	
	spinbox->setMaximum(value);
}

void QReleaseSliderValue::invertAppearance(bool invert)
{
	slider->setInvertedAppearance(invert);
	//spinbox->setInvertedAppearance(invert);
	
}

void QReleaseSliderValue::invertKeyBindings(bool invert)
{
	slider->setInvertedControls(invert);
	//spinbox->setInvertedControls(invert);
	
}