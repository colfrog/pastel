#include <list>
#include <map>
#include <random>

#include <QColor>
#include <QList>
#include <QPoint>
#include <QString>

class Piece {
public:
	Piece();
	Piece(quint8 type);

	enum class Orientation {
		UP, RIGHT, DOWN, LEFT
	};

	quint8 get_type() const;
	QColor get_color() const;
	static QColor get_type_color(quint8 type);
	const char *get_name() const;
	Orientation get_orientation() const;

	void slide(const QPoint &offset);
	void center_horizontally();
	void rotate();
	const QList<QPoint> &get_square_positions() const;

private:
	void rotate_line();
	void rotate_T();
	void rotate_Z();
	void rotate_inverted_Z();
	void rotate_L();
	void rotate_inverted_L();

        QVector<QPoint> m_squares;
	quint8 m_type;
	Orientation m_orientation;

	static std::random_device random_device;
	static std::mt19937 rng;
	static std::uniform_int_distribution<std::mt19937::result_type>
	random_type;

	static const std::map<quint8, QList<QPoint>> starting_positions;
	static constexpr Orientation starting_orientations[8] = {
		Orientation::UP,
		Orientation::RIGHT,
		Orientation::UP,
		Orientation::UP,
		Orientation::RIGHT,
		Orientation::RIGHT,
		Orientation::RIGHT,
		Orientation::LEFT
	};
	static const QColor colors[8];
	static const char *typenames[8];
	static constexpr const int center_x_offset[] = {
		0, 3, 4, 4, 4, 4, 4, 4
	};
};
