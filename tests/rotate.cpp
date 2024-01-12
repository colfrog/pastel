#include <iostream>

#include "../piece.h"

const char *orientations[] = {
	"Up", "Right", "Down", "Left"
};

void print_piece(Piece &p) {
	for (int i = -2; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			bool printed = false;
			for (const QPoint &point : p.get_square_positions()) {
				if (point.x() == j && point.y() == i) {
					std::cout << "#";
					printed = true;
				}
			}

			if (!printed)
				std::cout << " ";
		}
		std::cout << std::endl;
	}
}

void print_points(Piece &p) {
	std::cout << orientations[(int) p.get_orientation()] << "\t";
	for (const QPoint point : p.get_square_positions())
		std::cout << point.x() << ", " << point.y() << "; ";
	std::cout << std::endl;
	print_piece(p);
}

void test_rotation(quint8 type) {
	Piece p = Piece(type);
	std::cout << p.get_name() << std::endl;
	print_points(p);
	p.rotate();
	print_points(p);
	p.rotate();
	print_points(p);
	p.rotate();
	print_points(p);
	p.rotate();
	print_points(p);
}

int main() {
	for (quint8 i = 1; i <= 7; i++) {
		test_rotation(i);
		std::cout << std::endl;
	}
}
