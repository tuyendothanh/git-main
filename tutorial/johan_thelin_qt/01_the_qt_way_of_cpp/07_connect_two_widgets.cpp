/*
  $ qmake-qt5 -project -nopwd $PWD/06_MyClass.h \
                              $PWD/07_connect_two_widgets.cpp \
                              -o project.pro
  $ qmake-qt5
  $ make
  $ ./project
  $ git clean -f .
 */


#include "06_MyClass.h"
#include <QtWidgets/QtWidgets>



int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	QWidget widget;
	QLineEdit* lineEdit = new QLineEdit();
	QLabel* label = new QLabel();

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(lineEdit);
	layout->addWidget(label);

	widget.setLayout(layout);

	MyClass* bridge = new MyClass("", &app);

	QObject::connect(
		lineEdit, SIGNAL(textChanged(const QString&)),
		bridge, SLOT(setText(const QString&)) );

	QObject::connect(
		bridge, SIGNAL(textChanged(const QString&)),
		label, SLOT(setText(const QString&)) );

	widget.show();

	return app.exec();
}
