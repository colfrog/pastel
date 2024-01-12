#include <QApplication>

#include "BoardView.h"

int main(int argc, char **argv) {
	QApplication app(argc, argv);
	BoardView view;
	view.show();
	return app.exec();
}
