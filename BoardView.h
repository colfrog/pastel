#include <QWidget>
#include <QTimer>

#include "board.cpp"

#define SQUARE_SIZE 20

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

	int m_width = SQUARE_SIZE*(BOARD_WIDTH + 1);
	int m_height = SQUARE_SIZE*(BOARD_HEIGHT + 1);
};
