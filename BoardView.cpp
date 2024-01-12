#include <QPainter>

#include "BoardView.h"

BoardView::BoardView(QWidget *parent) : QWidget(parent) {
	m_timer = new QTimer;
	connect(m_timer, &QTimer::timeout,
		this, &BoardView::step);
	resize(m_width, m_height);
}

void BoardView::step() {
	m_board.move_current_piece_down();
	update();
}

void BoardView::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
}
