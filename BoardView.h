#include <QWidget>
#include <QTimer>

#include "board.h"

#define SQUARE_SIZE 25

class BoardView : public QWidget {
	Q_OBJECT

public:
	BoardView(QWidget *parent = nullptr);

public slots:
	void step();

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	Board m_board;
	QTimer *m_timer;

	int m_width = SQUARE_SIZE*BOARD_WIDTH;
	int m_height = SQUARE_SIZE*BOARD_HEIGHT;
};
