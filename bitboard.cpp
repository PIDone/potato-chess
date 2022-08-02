#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "bitboard.h"

std::string bitboard::squares[64] = {
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
};
std::string bitboard::pieces = "PNBRQKpnbrqk";
bitboard::Position bitboard::board = { 0, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, false, 0, 0, 0, 0 };

bool bitboard::query(ui* bitboard, int position) {
	return *bitboard >> position & 1;
}
bool bitboard::query(ull* bitboard, int position) {
	return *bitboard >> position & 1;
}

void bitboard::set(ui* bitboard, int position, int value) {
	if (value == 0)
		*bitboard ^= 1ull << position;
	else if (value == 1)
		*bitboard |= 1ull << position;
}
void bitboard::set(ull* bitboard, int position, int value) {
	if (value == 0)
		*bitboard ^= 1ull << position;
	else if (value == 1)
		*bitboard |= 1ull << position;
}

// __builtin_ctz and __builtin_clz are only available on gcc and clang (and not MSVC)
// but emscripten (web assembly compiler) uses clang so I don't care
int bitboard::ctz(ull* board) {
	return __builtin_ctz(*board);
}
int bitboard::clz(ull* board) {
	return __builtin_clz(*board);
}

std::vector<std::string> bitboard::split(std::string str, char splitOn) {
	std::vector<std::string> result;

	int left = 0;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == splitOn) {
			result.push_back(str.substr(left, i - left));
			left = i + 1;
		}
	}

	result.push_back(str.substr(left, str.length() - left));
	return result;
}

void bitboard::decode(std::string fen) {
	std::vector<std::string> splitted = bitboard::split(fen, ' ');

	if (!splitted[1].compare("w"))
		board.turn = false;
	else
		board.turn = true;

	board.fiftyMoveClock = stoi(splitted[4]);

	std::string castle = splitted[2];
	if (castle.find('K') != std::string::npos)
		bitboard::set(&board.castle, 3, 1);
	if (castle.find('Q') != std::string::npos)
		bitboard::set(&board.castle, 2, 1);
	if (castle.find('k') != std::string::npos)
		bitboard::set(&board.castle, 1, 1);
	if (castle.find('q') != std::string::npos)
		bitboard::set(&board.castle, 0, 1);

	std::string ep = splitted[3];
	if (ep.compare("-"))
		board.enPassant = (ep[1] - '0' - 1) * 8 + ep[0] - 'a';
	else
		board.enPassant = -1;

	std::vector<std::string> line = split(splitted[0], '/');

	for (int i = 0; i < 8; i++) {
		int cur = 0;
		std::string curRank = line[i];
		for (int j = 0; j < curRank.size(); j++) {
			if (isdigit(curRank[j])) {
				cur += curRank[j] - '0';
				continue;
			}

			bitboard::set(&board.pieces, i * 8 + cur, 1);

			int piece = bitboard::pieces.find(curRank[j]);
			if (piece >= 6)
				bitboard::set(&board.blackPieces[piece - 6], i * 8 + cur, 1);
			else
				bitboard::set(&board.whitePieces[piece], i * 8 + cur, 1);

			cur++;
		}
	}
}
std::string bitboard::encode() {
	std::string fen = "";
	for (int i = 0; i < 8; i++) {
		int empty = 0;
		for (int j = 0; j < 8; j++) {
			int index = i * 8 + j;
			if (bitboard::query(&board.pieces, index)) {
				if (empty != 0)
					fen += empty + '0';
				empty = 0;for (int k = 0; k < 6; k++) {
					if (bitboard::query(&board.whitePieces[k], index)) {
						fen += bitboard::pieces[k];
						break;
					}
				}
				for (int k = 0; k < 6; k++) {
					if (bitboard::query(&board.blackPieces[k], index)) {
						fen += bitboard::pieces[k + 6];
						break;
					}
				}
			}
			else
				empty++;
		}

		if (empty != 0)
			fen += empty + '0';

		if (i < 7)
			fen += "/";
	}
	fen += " ";

	fen += board.turn ? 'b' : 'w';
	fen += " ";

	std::string castleRights = "";
	if (bitboard::query(&board.castle, 3))
		castleRights += "K";
	if (bitboard::query(&board.castle, 2))
		castleRights += "Q";
	if (bitboard::query(&board.castle, 1))
		castleRights += "k";
	if (bitboard::query(&board.castle, 0))
		castleRights += "q";
	if (castleRights == "")
		castleRights = "-";

	fen += castleRights + " ";

	if (board.enPassant == -1)
		fen += "-";
	else
		fen += bitboard::squares[board.enPassant];
		
	fen += " " + std::to_string(board.fullMove);
	fen += " " + std::to_string(board.fiftyMoveClock + 1);

	return fen;
}

void bitboard::printBoard() {
	std::cout << "╭───┬───┬───┬───┬───┬───┬───┬───╮" << "\n";

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int index = i * 8 + j;
			char print = ' ';

			if (bitboard::query(&board.pieces, index)) {
				for (int k = 0; k < 6; k++) {
					if (bitboard::query(&board.whitePieces[k], index)) {
						std::cout << "│ " << bitboard::pieces[k] << " ";
						break;
					}
				}
				for (int k = 0; k < 6; k++) {
					if (bitboard::query(&board.blackPieces[k], index)) {
						std::cout << "│ " << bitboard::pieces[k + 6] << " ";
						break;
					}
				}
			}
			else
				std::cout << "│   ";
		}

		std::cout << "│";

		if (i < 7)
			std::cout << "\n├───┼───┼───┼───┼───┼───┼───┼───┤\n";
		else
			std::cout << "\n╰───┴───┴───┴───┴───┴───┴───┴───╯\n";
	}

	std::cout << "\nFEN: " << encode();
	std::cout << "\nWhite Kingside: " << bitboard::query(&board.castle, 3);
	std::cout << "    White Queenside: " << bitboard::query(&board.castle, 2);
	std::cout << "\nBlack Kingside: " << bitboard::query(&board.castle, 1);
	std::cout << "    Black Queenside: " << bitboard::query(&board.castle, 0);

	std::cout << "\nEn Passant Square: " << bitboard::squares[board.enPassant];
	std::cout << "\nTurn: " << (board.turn ? "Black" : "White");

	std::cout << "\n";
}