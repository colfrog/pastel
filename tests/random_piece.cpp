#include <iostream>

#include "../piece.h"

int main() {
	Piece *p;
	for (int i = 0; i < 20; i++) {
		p = new Piece();
		std::cout << p->get_name() << std::endl;
		delete p;
	}
}
