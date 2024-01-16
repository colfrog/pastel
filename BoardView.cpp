#include <algorithm>
#include <math>

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
	m_square_size = std::min((int) (width() / BV_WIDTH),
				 (int) (height() / BOARD_HEIGHT));
	update();
}

void BoardView::step() {
	if (m_board.game_over()) {
		m_timer->stop();
		update();
		return;
	}

	if (m_board.current_piece_on_pile()) {
		m_timer->setInterval(500);
	} else {
		m_board.move_current_piece_down();
		m_time = 500 - sqrt(1600*(m_board.level() - 1));
		m_timer->setInterval(m_time);
	}
	update();
}

void BoardView::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.setBrush(QColor("#A0F"));
	painter.setPen("#A0F");
	painter.drawRect(0, 0, width(), height());
	painter.translate((width() - m_square_size*BV_WIDTH)/2, 0);
	draw_board(painter);
	if (!m_board.game_over())
		draw_piece(painter, m_board.get_current_piece());
	painter.translate(BOARD_WIDTH*m_square_size, 0);
	draw_next_piece(painter);
	painter.translate(0, 4*m_square_size);
	draw_info(painter);

	painter.resetTransform();
	if (m_board.game_over())
		draw_game_over(painter);
}

void BoardView::resizeEvent(QResizeEvent *event) {
	set_square_size();
}

void BoardView::keyPressEvent(QKeyEvent *event) {
	if (m_board.game_over())
		return;

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
	painter.setBrush(QColor("#000"));
	painter.setPen("#aaa");
	painter.drawRect(0, 0, SIDEBAR_WIDTH*m_square_size,
			 4*m_square_size);

	painter.setPen("#dcdccc");
	QFont font = painter.font();
	font.setPixelSize(m_square_size/2);
	painter.setFont(font);
	painter.drawText(0, 0, 6*m_square_size, m_square_size,
			 Qt::AlignHCenter | Qt::AlignVCenter, "Next piece");

        painter.translate(m_square_size, m_square_size);
	draw_piece(painter, m_board.get_next_piece());
	painter.translate(-m_square_size, -m_square_size);
}

void BoardView::draw_info(QPainter &painter) {
	painter.setBrush(QColor("#000"));
	painter.drawRect(0, 0, SIDEBAR_WIDTH*m_square_size, 16*m_square_size);

	painter.setPen("#dcdccc");
	QFont font = painter.font();
	font.setPixelSize(m_square_size);
	painter.setFont(font);
	QString score = QString("Score: ") + QString::number(m_board.score());
	QString level = QString("Level: ") + QString::number(m_board.level());
	painter.drawText(0, m_square_size,
			 6*m_square_size, m_square_size,
			 Qt::AlignHCenter | Qt::AlignVCenter, score);
	painter.drawText(0, 2*m_square_size,
			 6*m_square_size, m_square_size,
			 Qt::AlignHCenter | Qt::AlignVCenter, level);
}

void BoardView::draw_game_over(QPainter &painter) {
	QFont font = painter.font();
	font.setPixelSize(2*m_square_size);
	painter.setFont(font);
	painter.setPen("#f00");
	painter.drawText(0, 0, width(), height(),
			 Qt::AlignHCenter | Qt::AlignVCenter,
			 "GAME OVER");
	painter.translate(0, -m_square_size*(BOARD_HEIGHT/2 - 1));
}
