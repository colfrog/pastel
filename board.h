#pragma once

#include "piece.h"

#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10

class Board {
public:
	Board();

	const Piece &get_current_piece();
	const Piece &get_next_piece();

	enum class Bounds {
		NONE, UP, RIGHT, DOWN, LEFT
	};

	Bounds current_piece_in_bounds() const;
	bool current_piece_overlaps_with_pile() const;
	bool current_piece_on_pile() const;

	void rotate_current_piece();
	void move_current_piece_left();
	void move_current_piece_right();
	void move_current_piece_down();
	void commit_current_piece_to_pile();
	void new_piece();
	void clear_full_lines();
	void move_down_above(int line);

private:
	void init_pieces();
	void init_table();

	quint8 m_table[BOARD_WIDTH][BOARD_HEIGHT];
	Piece m_current_piece;
	Piece m_next_piece;

	static const QList<QPoint> edge_adjustment;
};
