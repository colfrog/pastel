#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "board.h"

#define SIDEBAR_WIDTH 6
#define BV_WIDTH (BOARD_WIDTH + SIDEBAR_WIDTH)

class BoardView : public QWidget {
	Q_OBJECT

public:
	BoardView(QWidget *parent = nullptr);

	void set_square_size();

public slots:
	void step();

protected:
	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;

private:
	void draw_board(QPainter &painter);
	void draw_piece(QPainter &painter, const Piece &piece);
	void draw_next_piece(QPainter &painter);
	void draw_info(QPainter &painter);
	void draw_game_over(QPainter &painter);

	Board m_board;
	QTimer *m_timer;
	int m_time = 500;
	int m_square_size;
};
