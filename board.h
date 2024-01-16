#include "piece.h"

#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10

class Board {
public:
	Board();

	const Piece &get_current_piece();
	const Piece &get_next_piece();
	quint8 at(int x, int y) const;
	bool game_over() const;
	quint64 score() const;
	quint64 level() const;

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
	void init_table();

	quint8 m_table[BOARD_WIDTH][BOARD_HEIGHT];
	Piece m_current_piece;
	Piece m_next_piece;
	bool m_game_over = false;
	quint64 m_score = 0;
	quint64 m_lines_cleared = 0;

	static const QList<QPoint> edge_adjustment;
};
