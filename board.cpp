#include "board.h"

Board::Board() {
	init_pieces();
	init_table();
}

void Board::init_pieces() {
	m_current_piece = Piece();
	m_current_piece.center_horizontally();
	m_next_piece = Piece();
}

void Board::init_table() {
	for (int i = 0; i < BOARD_WIDTH; i++)
		for (int j = 0; j < BOARD_WIDTH; j++)
			m_table[i][j] = 0;
}

const Piece &Board::get_current_piece() {
	return m_current_piece;
}

const Piece &Board::get_next_piece() {
	return m_next_piece;
}

const QList<QPoint> Board::edge_adjustment = {
	QPoint(0, 0),
	QPoint(0, 1),
	QPoint(-1, 0),
	QPoint(0, -1),
	QPoint(1, 0)
};

Board::Bounds Board::current_piece_in_bounds() const {
	for (const QPoint &p : m_current_piece.get_square_positions()) {
		if (p.x() < 0)
			return Bounds::LEFT;
		if (p.x() > BOARD_WIDTH)
			return Bounds::RIGHT;
		if (p.y() < 0)
			return Bounds::UP;
		if (p.y() > 0 || current_piece_overlaps_with_pile())
			return Bounds::DOWN;
	}

	return Bounds::NONE;
}

bool Board::current_piece_overlaps_with_pile() const {
	for (const QPoint &p : m_current_piece.get_square_positions()) {
		for (int i = 0; i < BOARD_WIDTH; i++) {
			for (int j = 0; j < BOARD_HEIGHT; j++) {
				if (m_table[i][j] == 0)
					continue;

				if (p.x() == i && p.y() == j)
					return true;
			}
		}
	}

	return false;
}

bool Board::current_piece_on_pile() const {
	for (const QPoint &p : m_current_piece.get_square_positions()) {
		for (int i = 0; i < BOARD_WIDTH; i++) {
			for (int j = 0; j < BOARD_HEIGHT; j++) {
				if (p.x() == i && p.y() - j == -1)
					return true;
			}
		}
	}

	return false;
}

void Board::rotate_current_piece() {
	m_current_piece.rotate();
	Bounds edge = Bounds::NONE;
	while ((edge = current_piece_in_bounds()) != Bounds::NONE)
		m_current_piece.slide(edge_adjustment[int(edge)]);
}

void Board::move_current_piece_left() {
	bool at_edge = false;
	for (const QPoint &p : m_current_piece.get_square_positions()) {
		if (p.x() == 0)
			at_edge = true;
	}

	if (!at_edge)
		m_current_piece.slide(QPoint(-1, 0));
}

void Board::move_current_piece_right() {
	bool at_edge = false;
	for (const QPoint &p : m_current_piece.get_square_positions()) {
		if (p.x() == 0)
			at_edge = true;
	}

	if (!at_edge)
		m_current_piece.slide(QPoint(1, 0));
}

void Board::move_current_piece_down() {
	if (current_piece_on_pile()) {
		commit_current_piece_to_pile();
		new_piece();
	} else {
		m_current_piece.slide(QPoint(0, 1));
	}
}

void Board::commit_current_piece_to_pile() {
	while (!current_piece_on_pile())
		m_current_piece.slide(QPoint(0, 1));

	for (const QPoint &p : m_current_piece.get_square_positions())
		m_table[p.x()][p.y()] = m_current_piece.get_type();

	clear_full_lines();
}

void Board::new_piece() {
	m_current_piece = m_next_piece;
	m_current_piece.center_horizontally();
	m_next_piece = Piece();
}

void Board::clear_full_lines() {
	for (int j = BOARD_HEIGHT - 1; j >= 0; j--) {
		int count = 0;

		for (int i = 0; i < BOARD_WIDTH; i++) {
			if (m_table[i][j])
				count++;
		}

		if (count == BOARD_WIDTH) {
			move_down_above(j);
		}
	}
}

void Board::move_down_above(int line) {
	for (int j = line - 1; j >= 0; j--) {
		for (int i = 0; i < BOARD_WIDTH; i++) {
			m_table[i][j + 1] = m_table[i][j];
			m_table[i][j] = 0;
		}
	}
}
