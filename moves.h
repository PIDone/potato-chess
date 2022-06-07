#ifndef MOVES_H
#define MOVES_H

#include <string>

namespace moves {
    struct Move {
        int source; // Original square
        int dest; // Square where piece will be after move

        int capture; // iece this move captures (0 if none)
        int castle; // 0 = not a castling move, 1 = kingside, 2 = queenside
        int promote; // Piece this pawn promotes to (0 if none)

        int signal; // Casling rights. K disables Kingside, Q disables Queenside, X disables both

        bool isEp; // Is this move En Passant?
    };
    struct SimpleMove {
        int source;
        int dest;
    };

    void makeMove(Move move);

    void unmakeMove(Move move);

    void castle(int dir);

    void uncastle(int dir);

    SimpleMove convertUCI(std::string UCIMove);

    void printMove(moves::Move move, bool newLine);
}

#endif
