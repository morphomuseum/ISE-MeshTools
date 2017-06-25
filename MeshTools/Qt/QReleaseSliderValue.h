#ifndef QRELEASESLIDERVALUE_H
#define QRELEASESLIDERVALUE_H

#include <QGroupBox>

class QDial;
class QScrollBar;
//class QReleaseSlider;
class QSlider;
class QSpinBox;

class QReleaseSliderValue : public QGroupBox
{
	Q_OBJECT

public:
	QReleaseSliderValue(Qt::Orientation orientation, const QString &title,
		QWidget *parent = 0);

signals:
	void valueChanged(int value);

	public slots:
	void setValue(int value);
	void setMinimum(int value);
	void setMaximum(int value);
	void invertAppearance(bool invert);
	void invertKeyBindings(bool invert);

private:
	//QReleaseSlider *slider;
	QSlider *slider;
	QSpinBox *spinbox;
	QDial *dial;
};

#endif