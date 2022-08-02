#include <vector>

#include "bitboard.h"
#include "moveGen.h"

void moveGenWhite(std::vector<ui>* moves) {
	ull checks = 0;
	int kingPos = bitboard::ctz(&bitboard::board.whitePieces[5]);
	
	
}
void moveGenBlack(std::vector<ui>* moves) { }
	
void moveGenmoveGen(std::vector<unsigned int>* moves) {
	if (bitboard::board.turn)
		moveGenBlack(moves);
	else
		moveGenWhite(moves);
}