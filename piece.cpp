#include "piece.h"

const std::map<quint8, QList<QPoint>> Piece::starting_positions = {
	// Line
	{1, {QPoint(0, 0), QPoint(1, 0), QPoint(2, 0), QPoint(3, 0)}},
	// Square
	{2, {QPoint(0, 0), QPoint(0, 1), QPoint(1, 0), QPoint(1, 1)}},
	// T
	{3, {QPoint(1, 0), QPoint(0, 1), QPoint(1, 1), QPoint(2, 1)}},
	// Z
	{4, {QPoint(0, 0), QPoint(1, 0), QPoint(1, 1), QPoint(2, 1)}},
	// Inverted Z
	{5, {QPoint(2, 0), QPoint(1, 0), QPoint(1, 1), QPoint(0, 1)}},
	// L
	{6, {QPoint(2, 0), QPoint(0, 1), QPoint(1, 1), QPoint(2, 1)}},
	// Inverted L
	{7, {QPoint(0, 0), QPoint(0, 1), QPoint(1, 1), QPoint(2, 1)}}
};

const QColor Piece::colors[8] = {
	QColor("#000"),
	QColor("#7BD3EA"),
	QColor("#FDFFAB"),
	QColor("#AC87C5"),
	QColor("#FFB996"),
	QColor("#A1EEBD"),
	QColor("#FFCF81"),
	QColor("#80BCBD")
};

std::random_device Piece::random_device;
std::mt19937 Piece::rng{random_device()};
std::uniform_int_distribution<std::mt19937::result_type>
Piece::random_type{1, 7};

Piece::Piece() : Piece(random_type(rng)) {}

Piece::Piece(quint8 type) {
	m_type = type;
	m_orientation = starting_orientations[type];
	const QList<QPoint> *squares = &starting_positions.at(type);
	for (QPoint square : *squares)
		m_squares.append(square);
}

QColor Piece::get_color() const {
	return get_type_color(m_type);
}

QColor Piece::get_type_color(quint8 type) {
	return colors[type];
}

quint8 Piece::get_type() const {
	return m_type;
}

const char *Piece::typenames[8] = {
	"None",
	"Line",
	"Square",
	"T",
	"Z",
	"Inverted Z",
	"L",
	"Inverted L"
};
const char *Piece::get_name() const {
	return typenames[m_type];
}

Piece::Orientation Piece::get_orientation() const {
	return m_orientation;
}

void Piece::slide(const QPoint &offset) {
	for (QPoint &point : m_squares)
		point += offset;
}

void Piece::center_horizontally() {
	slide(QPoint(center_x_offset[m_type], 0));
}

void Piece::rotate_line() {
	int i = 0;
	for (QPoint &square : m_squares) {
		if (m_orientation == Orientation::UP ||
		    m_orientation == Orientation::DOWN) {
			square.rx() -= 2 - i;
			square.ry() += 2 - i;
		} else {
			square.rx() += 2 - i;
			square.ry() -= 2 - i;
		}

		i++;
	}
}

void Piece::rotate_T() {
	QList<QPoint> &s = m_squares;
	s[1] = s[0];
	s[0] = s[3];
	switch (m_orientation) {
	case Orientation::UP:
		s[3] += QPoint(-1, 1);
		break;
	case Orientation::RIGHT:
		s[3] += QPoint(-1, -1);
		break;
	case Orientation::DOWN:
		s[3] += QPoint(1, -1);
		break;
	case Orientation::LEFT:
		s[3] += QPoint(1, 1);
		break;
	}
}

void Piece::rotate_Z() {
	QList<QPoint> &s = m_squares;
	
	switch (m_orientation) {
	case Orientation::UP:
		s[3] += QPoint(2, 0);
		s[2] += QPoint(1, 1);
		s[0] += QPoint(-1, 1);
		break;
	case Orientation::RIGHT:
		s[0] += QPoint(2, 0);
		s[1] += QPoint(1, 1);
		s[3] += QPoint(-1, 1);
		break;
	case Orientation::DOWN:
		s[0] += QPoint(-2, 0);
		s[1] += QPoint(-1, -1);
		s[3] += QPoint(1, -1);
		break;
	case Orientation::LEFT:
		s[3] += QPoint(-2, 0);
		s[2] += QPoint(-1, -1);
		s[0] += QPoint(1, -1);
		break;
	}
}

void Piece::rotate_inverted_Z() {
	QList<QPoint> &s = m_squares;
	
	switch (m_orientation) {
	case Orientation::UP:
		s[3] += QPoint(-2, 0);
		s[2] += QPoint(-1, 1);
		s[0] += QPoint(1, 1);
		break;
	case Orientation::RIGHT:
		s[0] += QPoint(-2, 0);
		s[1] += QPoint(-1, 1);
		s[3] += QPoint(1, 1);
		break;
	case Orientation::DOWN:
		s[0] += QPoint(2, 0);
		s[1] += QPoint(1, -1);
		s[3] += QPoint(-1, -1);
		break;
	case Orientation::LEFT:
		s[3] += QPoint(2, 0);
		s[2] += QPoint(1, -1);
		s[0] += QPoint(-1, -1);
		break;
	}
}

void Piece::rotate_L() {
	QList<QPoint> &s = m_squares;
	
	switch (m_orientation) {
	case Orientation::UP:
		s[0] += QPoint(2, 0);
		s[3] += QPoint(1, 1);
		s[1] += QPoint(-1, -1);
		break;
	case Orientation::RIGHT:
		s[0] += QPoint(0, 2);
		s[3] += QPoint(-1, 1);
		s[1] += QPoint(1, -1);
		break;
	case Orientation::DOWN:
		s[0] += QPoint(-2, 0);
		s[3] += QPoint(-1, -1);
		s[1] += QPoint(1, 1);
		break;
	case Orientation::LEFT:
		s[0] += QPoint(0, -2);
		s[3] += QPoint(1, -1);
		s[1] += QPoint(-1, 1);
		break;
	}
}

void Piece::rotate_inverted_L() {
	QList<QPoint> &s = m_squares;
	
	switch (m_orientation) {
	case Orientation::UP:
		s[0] += QPoint(0, 2);
		s[1] += QPoint(1, 1);
		s[3] += QPoint(-1, -1);
		break;
	case Orientation::RIGHT:
		s[0] += QPoint(-2, 0);
		s[1] += QPoint(-1, 1);
		s[3] += QPoint(1, -1);
		break;
	case Orientation::DOWN:
		s[0] += QPoint(0, -2);
		s[1] += QPoint(-1, -1);
		s[3] += QPoint(1, 1);
		break;
	case Orientation::LEFT:
		s[0] += QPoint(2, 0);
		s[1] += QPoint(1, -1);
		s[3] += QPoint(-1, 1);
		break;
	}
}

void Piece::rotate() {
	switch (m_type) {
	case 1: // Line
		rotate_line();
		break;
	case 2: // Square
		// noop
		break;
	case 3: // T
		rotate_T();
		break;
	case 4: // Z
		rotate_Z();
		break;
	case 5: // Inverted Z
		rotate_inverted_Z();
		break;
	case 6: // L
		rotate_L();
		break;
	case 7: // Inverted L
		rotate_inverted_L();
		break;
	default:
		break;
	}

	m_orientation = Orientation(((int) m_orientation + 1) % 4);
}

const QList<QPoint> &Piece::get_square_positions() const {
	return m_squares;
}
