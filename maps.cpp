#include <bitset>
#include <iostream>

#include "bitboard.h"
#include "maps.h"

ull maps::rook[64];
ull maps::bishop[64];
	
void maps::init() {
	for (int i = 0; i < 64; i++) {
		for (int x = i % 8; x < 8; x++)
			bitboard::set(&maps::rook[i], x + (i / 8) * 8, 1);
		for (int x = i % 8; x >= 0; x--)
			bitboard::set(&maps::rook[i], x + (i / 8) * 8, 1);
		for (int y = i / 8; y < 8; y++)
			bitboard::set(&maps::rook[i], y * 8 + (i % 8), 1);
		for (int y = i / 8; y >= 0; y--)
			bitboard::set(&maps::rook[i], y * 8 + (i % 8), 1);
		bitboard::set(&maps::rook[i], i, 0);

		for (int x = i % 8, y = i / 8; x < 8 && y < 8; x++, y++)
			bitboard::set(&maps::bishop[i], y * 8 + x, 1);
		for (int x = i % 8, y = i / 8; x < 8 && y >= 0; x++, y--)
			bitboard::set(&maps::bishop[i], y * 8 + x, 1);
		for (int x = i % 8, y = i / 8; x >= 0 && y < 8; x--, y++)
			bitboard::set(&maps::bishop[i], y * 8 + x, 1);
		for (int x = i % 8, y = i / 8; x >= 0 && y >= 0; x--, y--)
			bitboard::set(&maps::bishop[i], y * 8 + x, 1);
		bitboard::set(&maps::bishop[i], i, 0);
	}
}