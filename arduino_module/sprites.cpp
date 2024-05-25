#include "sprites.hpp"

void show_service() {
	matrix.clear();
	matrix.setPixel(5, 0, true);
	matrix.setPixel(4, 1, true);
	matrix.setPixel(4, 2, true);
	matrix.setPixel(7, 2, true);
	matrix.setPixel(3, 3, true);
	matrix.setPixel(4, 3, true);
	matrix.setPixel(5, 3, true);
	matrix.setPixel(6, 3, true);
	matrix.setPixel(2, 4, true);
	matrix.setPixel(3, 4, true);
	matrix.setPixel(4, 4, true);
	matrix.setPixel(1, 5, true);
	matrix.setPixel(2, 5, true);
	matrix.setPixel(3, 5, true);
	matrix.setPixel(0, 6, true);
	matrix.setPixel(1, 6, true);
	matrix.setPixel(2, 6, true);
	matrix.setPixel(0, 7, true);
	matrix.setPixel(1, 7, true);
	matrix.display();
}

void show_stabilized() {
	matrix.clear();
	matrix.setPixel(2, 1, true);
	matrix.setPixel(3, 1, true);
	matrix.setPixel(4, 1, true);
	matrix.setPixel(5, 1, true);
	matrix.setPixel(1, 2, true);
	matrix.setPixel(6, 2, true);
	matrix.setPixel(1, 3, true);
	matrix.setPixel(2, 3, true);
	matrix.setPixel(2, 4, true);
	matrix.setPixel(3, 4, true);
	matrix.setPixel(4, 4, true);
	matrix.setPixel(5, 4, true);
	matrix.setPixel(5, 5, true);
	matrix.setPixel(6, 5, true);
	matrix.setPixel(1, 6, true);
	matrix.setPixel(6, 6, true);
	matrix.setPixel(2, 7, true);
	matrix.setPixel(3, 7, true);
	matrix.setPixel(4, 7, true);
	matrix.setPixel(5, 7, true);
	matrix.display();
}

void show_manual() {
	matrix.clear();
	matrix.setPixel(0, 1, true);
	matrix.setPixel(6, 1, true);
	matrix.setPixel(0, 2, true);
	matrix.setPixel(1, 2, true);
	matrix.setPixel(5, 2, true);
	matrix.setPixel(6, 2, true);
	matrix.setPixel(0, 3, true);
	matrix.setPixel(1, 3, true);
	matrix.setPixel(2, 3, true);
	matrix.setPixel(4, 3, true);
	matrix.setPixel(5, 3, true);
	matrix.setPixel(6, 3, true);
	matrix.setPixel(0, 4, true);
	matrix.setPixel(2, 4, true);
	matrix.setPixel(3, 4, true);
	matrix.setPixel(4, 4, true);
	matrix.setPixel(6, 4, true);
	matrix.setPixel(0, 5, true);
	matrix.setPixel(3, 5, true);
	matrix.setPixel(6, 5, true);
	matrix.setPixel(0, 6, true);
	matrix.setPixel(6, 6, true);
	matrix.setPixel(0, 7, true);
	matrix.setPixel(6, 7, true);
	matrix.display();
}

void show_acro() {
	matrix.clear();
	matrix.setPixel(2, 1, true);
	matrix.setPixel(3, 1, true);
	matrix.setPixel(4, 1, true);
	matrix.setPixel(5, 1, true);
	matrix.setPixel(1, 2, true);
	matrix.setPixel(2, 2, true);
	matrix.setPixel(5, 2, true);
	matrix.setPixel(6, 2, true);
	matrix.setPixel(1, 3, true);
	matrix.setPixel(6, 3, true);
	matrix.setPixel(1, 4, true);
	matrix.setPixel(6, 4, true);
	matrix.setPixel(1, 5, true);
	matrix.setPixel(2, 5, true);
	matrix.setPixel(3, 5, true);
	matrix.setPixel(4, 5, true);
	matrix.setPixel(5, 5, true);
	matrix.setPixel(6, 5, true);
	matrix.setPixel(1, 6, true);
	matrix.setPixel(6, 6, true);
	matrix.setPixel(1, 7, true);
	matrix.setPixel(6, 7, true);
	matrix.display();
}

void show_loading_0() {
	matrix.clear();
	matrix.setPixel(4, 2, true);
	matrix.setPixel(5, 3, true);
	matrix.setPixel(2, 4, true);
	matrix.setPixel(5, 4, true);
	matrix.setPixel(3, 5, true);
	matrix.setPixel(4, 5, true);
	matrix.display();
}

void show_loading_1() {
	matrix.clear();
	matrix.setPixel(3, 2, true);
	matrix.setPixel(4, 2, true);
	matrix.setPixel(5, 3, true);
	matrix.setPixel(5, 4, true);
	matrix.setPixel(3, 5, true);
	matrix.setPixel(4, 5, true);
	matrix.display();
}

void show_loading_2() {
	matrix.clear();
	matrix.setPixel(3, 2, true);
	matrix.setPixel(4, 2, true);
	matrix.setPixel(2, 3, true);
	matrix.setPixel(5, 3, true);
	matrix.setPixel(5, 4, true);
	matrix.setPixel(4, 5, true);
	matrix.display();
}

void show_loading_3() {
	matrix.clear();
	matrix.setPixel(3, 2, true);
	matrix.setPixel(4, 2, true);
	matrix.setPixel(2, 3, true);
	matrix.setPixel(5, 3, true);
	matrix.setPixel(2, 4, true);
	matrix.setPixel(5, 4, true);
	matrix.display();
}

void show_loading_4() {
	matrix.clear();
	matrix.setPixel(3, 2, true);
	matrix.setPixel(4, 2, true);
	matrix.setPixel(2, 3, true);
	matrix.setPixel(5, 3, true);
	matrix.setPixel(2, 4, true);
	matrix.setPixel(3, 5, true);
	matrix.display();
}

void show_loading_5() {
	matrix.clear();
	matrix.setPixel(3, 2, true);
	matrix.setPixel(4, 2, true);
	matrix.setPixel(2, 3, true);
	matrix.setPixel(2, 4, true);
	matrix.setPixel(3, 5, true);
	matrix.setPixel(4, 5, true);
	matrix.display();
}

void show_loading_6() {
	matrix.clear();
	matrix.setPixel(3, 2, true);
	matrix.setPixel(2, 3, true);
	matrix.setPixel(2, 4, true);
	matrix.setPixel(5, 4, true);
	matrix.setPixel(3, 5, true);
	matrix.setPixel(4, 5, true);
	matrix.display();
}

void show_loading_7() {
	matrix.clear();
	matrix.setPixel(2, 3, true);
	matrix.setPixel(5, 3, true);
	matrix.setPixel(2, 4, true);
	matrix.setPixel(5, 4, true);
	matrix.setPixel(3, 5, true);
	matrix.setPixel(4, 5, true);
	matrix.display();
}

void show_error() {
	matrix.clear();
	matrix.setPixel(2, 0, true);
	matrix.setPixel(3, 0, true);
	matrix.setPixel(4, 0, true);
	matrix.setPixel(5, 0, true);
	matrix.setPixel(1, 1, true);
	matrix.setPixel(6, 1, true);
	matrix.setPixel(0, 2, true);
	matrix.setPixel(3, 2, true);
	matrix.setPixel(4, 2, true);
	matrix.setPixel(7, 2, true);
	matrix.setPixel(0, 3, true);
	matrix.setPixel(3, 3, true);
	matrix.setPixel(4, 3, true);
	matrix.setPixel(7, 3, true);
	matrix.setPixel(0, 4, true);
	matrix.setPixel(7, 4, true);
	matrix.setPixel(0, 5, true);
	matrix.setPixel(3, 5, true);
	matrix.setPixel(4, 5, true);
	matrix.setPixel(7, 5, true);
	matrix.setPixel(1, 6, true);
	matrix.setPixel(6, 6, true);
	matrix.setPixel(2, 7, true);
	matrix.setPixel(3, 7, true);
	matrix.setPixel(4, 7, true);
	matrix.setPixel(5, 7, true);
	matrix.display();
}

void show_dead() {
	matrix.clear();
	matrix.setPixel(2, 0, true);
	matrix.setPixel(3, 0, true);
	matrix.setPixel(4, 0, true);
	matrix.setPixel(5, 0, true);
	matrix.setPixel(1, 1, true);
	matrix.setPixel(6, 1, true);
	matrix.setPixel(0, 2, true);
	matrix.setPixel(5, 2, true);
	matrix.setPixel(7, 2, true);
	matrix.setPixel(0, 3, true);
	matrix.setPixel(4, 3, true);
	matrix.setPixel(7, 3, true);
	matrix.setPixel(0, 4, true);
	matrix.setPixel(3, 4, true);
	matrix.setPixel(7, 4, true);
	matrix.setPixel(0, 5, true);
	matrix.setPixel(2, 5, true);
	matrix.setPixel(7, 5, true);
	matrix.setPixel(1, 6, true);
	matrix.setPixel(6, 6, true);
	matrix.setPixel(2, 7, true);
	matrix.setPixel(3, 7, true);
	matrix.setPixel(4, 7, true);
	matrix.setPixel(5, 7, true);
	matrix.display();
}

void show_rc_lost() {
	matrix.clear();
	matrix.setPixel(5, 0, true);
	matrix.setPixel(7, 0, true);
	matrix.setPixel(6, 1, true);
	matrix.setPixel(0, 2, true);
	matrix.setPixel(1, 2, true);
	matrix.setPixel(2, 2, true);
	matrix.setPixel(5, 2, true);
	matrix.setPixel(7, 2, true);
	matrix.setPixel(3, 3, true);
	matrix.setPixel(0, 4, true);
	matrix.setPixel(1, 4, true);
	matrix.setPixel(4, 4, true);
	matrix.setPixel(2, 5, true);
	matrix.setPixel(5, 5, true);
	matrix.setPixel(0, 6, true);
	matrix.setPixel(3, 6, true);
	matrix.setPixel(5, 6, true);
	matrix.setPixel(1, 7, true);
	matrix.setPixel(3, 7, true);
	matrix.setPixel(5, 7, true);
	matrix.display();
}

void show_smile() {
	matrix.clear();
	matrix.setPixel(2, 1, true);
	matrix.setPixel(5, 1, true);
	matrix.setPixel(2, 2, true);
	matrix.setPixel(5, 2, true);
	matrix.setPixel(2, 3, true);
	matrix.setPixel(5, 3, true);
	matrix.setPixel(0, 4, true);
	matrix.setPixel(7, 4, true);
	matrix.setPixel(1, 5, true);
	matrix.setPixel(6, 5, true);
	matrix.setPixel(2, 6, true);
	matrix.setPixel(3, 6, true);
	matrix.setPixel(4, 6, true);
	matrix.setPixel(5, 6, true);
	matrix.display();
}

