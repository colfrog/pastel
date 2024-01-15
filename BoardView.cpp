#include <algorithm>

#include <QKeyEvent>

#include "BoardView.h"

BoardView::BoardView(QWidget *parent) : QWidget(parent) {
	set_square_size();
	m_timer = new QTimer;
	connect(m_timer, &QTimer::timeout,
		this, &BoardView::step);
	m_timer->start(m_time);
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
	draw_board(painter);
	draw_piece(painter, m_board.get_current_piece());
	draw_next_piece(painter);
}

void BoardView::resizeEvent(QResizeEvent *event) {
	set_square_size();
}

void BoardView::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
	case Qt::Key_Escape:
		if (m_timer->isActive())
			m_timer->stop();
		else
			m_timer->start();
		break;
	case Qt::Key_Up:
	case Qt::Key_W:
	case Qt::Key_J:
		m_board.rotate_current_piece();
		break;
	case Qt::Key_Left:
	case Qt::Key_A:
	case Qt::Key_H:
		m_board.move_current_piece_left();
		break;
	case Qt::Key_Right:
	case Qt::Key_D:
	case Qt::Key_L:
		m_board.move_current_piece_right();
		break;
	case Qt::Key_Down:
	case Qt::Key_S:
	case Qt::Key_K:
		m_board.move_current_piece_down();
		break;
	case Qt::Key_Space:
		m_board.commit_current_piece_to_pile();
		break;
	default:
		break;
	}

	update();
}

void BoardView::draw_board(QPainter &painter) {
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
}

void BoardView::draw_piece(QPainter &painter, const Piece &piece) {
	for (const QPoint &point : piece.get_square_positions()) {
		painter.setBrush(piece.get_color());
		painter.setPen("#aaa");
		painter.drawRect(m_square_size*point.x(),
				 m_square_size*point.y(),
				 m_square_size, m_square_size);
	}
}

void BoardView::draw_next_piece(QPainter &painter) {
	painter.translate(BOARD_WIDTH*m_square_size, 0);
	painter.setBrush(QColor("#000"));
	painter.drawRect(0, 0, 6*m_square_size, 4*m_square_size);

	painter.setPen("#dcdccc");
	QFont font = painter.font();
	font.setPixelSize(m_square_size/2);
	painter.setFont(font);
	painter.drawText(m_square_size/5, 0, 6*m_square_size, m_square_size,
			 Qt::AlignHCenter | Qt::AlignVCenter, "Next piece");

        painter.translate(m_square_size, m_square_size);
	draw_piece(painter, m_board.get_next_piece());
}
