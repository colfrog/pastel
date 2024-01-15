#include <algorithm>

#include <QPainter>

#include "BoardView.h"

BoardView::BoardView(QWidget *parent) : QWidget(parent) {
	set_square_size();
	m_timer = new QTimer;
	connect(m_timer, &QTimer::timeout,
		this, &BoardView::step);
	m_timer->start(500);
}

void BoardView::set_square_size() {
	m_square_size = std::min((int) (width() / BOARD_WIDTH),
				 (int) (height() / BOARD_HEIGHT));
	update();
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
			painter.drawRect(m_square_size*i, m_square_size*j,
					 m_square_size, m_square_size);
		}
	}

	Piece piece = m_board.get_current_piece();
	for (const QPoint &point : piece.get_square_positions()) {
		painter.setBrush(piece.get_color());
		painter.setPen("#aaa");
		painter.drawRect(m_square_size*point.x(),
				 m_square_size*point.y(),
				 m_square_size, m_square_size);
	}
}

void BoardView::resizeEvent(QResizeEvent *event) {
	set_square_size();
}
