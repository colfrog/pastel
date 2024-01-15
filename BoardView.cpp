#include <QPainter>

#include "BoardView.h"

BoardView::BoardView(QWidget *parent) : QWidget(parent) {
	m_timer = new QTimer;
	connect(m_timer, &QTimer::timeout,
		this, &BoardView::step);
	resize(m_width, m_height);
	m_timer->start(500);
}

void BoardView::step() {
	m_board.move_current_piece_down();
	update();
}

void BoardView::paintEvent(QPaintEvent *event) {
	QPainter painter(this);

	for (int i = 0; i < BOARD_WIDTH; i++) {
		for (int j = 0; j < BOARD_HEIGHT; j++) {
			painter.setBrush(
				Piece::get_type_color(m_board.at(i, j))
			);
			painter.setPen("#aaa");
			painter.drawRect(SQUARE_SIZE*i, SQUARE_SIZE*j,
					 SQUARE_SIZE, SQUARE_SIZE);
		}
	}

	Piece piece = m_board.get_current_piece();
	for (const QPoint &point : piece.get_square_positions()) {
		painter.setBrush(piece.get_color());
		painter.setPen("#aaa");
		painter.drawRect(SQUARE_SIZE*point.x(), SQUARE_SIZE*point.y(),
				 SQUARE_SIZE, SQUARE_SIZE);
	}
}
