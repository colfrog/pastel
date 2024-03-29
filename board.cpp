#include <algorithm>

#include "board.h"

Board::Board() {
	m_current_piece.center_horizontally();
	init_table();
}

void Board::init_table() {
	for (int i = 0; i < BOARD_WIDTH; i++)
		for (int j = 0; j < BOARD_HEIGHT; j++)
			m_table[i][j] = 0;
}

const Piece &Board::get_current_piece() {
	return m_current_piece;
}

const Piece &Board::get_next_piece() {
	return m_next_piece;
}

quint8 Board::at(int x, int y) const {
	return m_table[x][y];
}

bool Board::game_over() const {
	return m_game_over;
}

quint64 Board::score() const {
	return m_score;
}

quint64 Board::level() const {
	return std::min(((int) m_lines_cleared/10) + 1, 99);
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
		if (p.y() > BOARD_HEIGHT || current_piece_overlaps_with_pile())
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
				if (p.y() == BOARD_HEIGHT - 1 ||
				    (m_table[i][j] != 0 &&
				     p.x() == i && p.y() - j == -1)) {
					return true;
				}
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
		if (p.x() >= 1 && m_table[p.x() - 1][p.y()])
			at_edge = true;
	}

	if (!at_edge)
		m_current_piece.slide(QPoint(-1, 0));
}

void Board::move_current_piece_right() {
	bool at_edge = false;
	for (const QPoint &p : m_current_piece.get_square_positions()) {
		if (p.x() == BOARD_WIDTH - 1)
			at_edge = true;
		if (p.x() <= BOARD_WIDTH - 2 && m_table[p.x() + 1][p.y()])
			at_edge = true;
	}

	if (!at_edge)
		m_current_piece.slide(QPoint(1, 0));
}

void Board::move_current_piece_down() {
	if (current_piece_on_pile()) {
		commit_current_piece_to_pile();
	} else {
		m_current_piece.slide(QPoint(0, 1));
	}
}

void Board::commit_current_piece_to_pile() {
	while (!current_piece_on_pile())
		m_current_piece.slide(QPoint(0, 1));

	for (const QPoint &p : m_current_piece.get_square_positions())
		m_table[p.x()][p.y()] = m_current_piece.get_type();

	m_score += 10;
	new_piece();
	clear_full_lines();
}

void Board::new_piece() {
	m_current_piece = m_next_piece;
	m_current_piece.center_horizontally();
	m_next_piece = Piece();

	if (current_piece_overlaps_with_pile())
		m_game_over = true;
}

void Board::clear_full_lines() {
	int lines_cleared = 0;
	for (int j = BOARD_HEIGHT - 1; j >= 0; j--) {
		int count = 0;

		for (int i = 0; i < BOARD_WIDTH; i++) {
			if (m_table[i][j])
				count++;
		}

		if (count == BOARD_WIDTH) {
			move_down_above(j);
			j++;
			lines_cleared++;
		}
	}

	m_score += lines_cleared*lines_cleared*100;
	m_lines_cleared += lines_cleared;
}

void Board::move_down_above(int line) {
	for (int j = line - 1; j >= 0; j--) {
		for (int i = 0; i < BOARD_WIDTH; i++) {
			m_table[i][j + 1] = m_table[i][j];
			m_table[i][j] = 0;
		}
	}
}
