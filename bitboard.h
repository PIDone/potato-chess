#ifndef BITBOARD_H
#define BITBOARD_H

#include <string>
#include <vector>

using ui = unsigned int;
using ull = unsigned long long;
namespace bitboard {
	struct Position {
		ull pieces;
		ull whitePieces[6];
		ull blackPieces[6];

		bool turn;
		int enPassant;
		ui castle;
		// 1111 (white king)(white queen)(black king)(black queen)

		int fiftyMoveClock;
		int fullMove;
	};
	
	extern Position board;
	
	extern std::string squares[];
	extern std::string pieces;

	bool query(ui* bitboard, int position);
	bool query(ull* bitboard, int position);

	void set(ui* bitboard, int position, int value);
	void set(ull* bitboard, int position, int value);

	int ctz(ull* board); // count trailing zeros in the binary representation of the ull
	int clz(ull* board); // count leading zeros in the binary representation of the ull

	std::vector<std::string> split(std::string str, char splitOn);

	void decode(std::string fen);
	std::string encode();

	void printBoard();
}

#endif