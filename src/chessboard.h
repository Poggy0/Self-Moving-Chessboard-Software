#ifndef CHESSBOARD
#define CHESSBOARD

#include <string>

typedef enum
{
    EMPTY,

    PAWN,
    KING,
    QUEEN,
    BISHOP,
    KNIGHT,
    ROOK

} PIECES;

typedef enum
{
    NO_PLAYER,

    WHITE,
    BLACK

} PLAYER;

typedef struct
{
    
    PIECES type;
    PLAYER color;

} PIECE;


class Board
{
    private:

        const static int _boardsize = 8;
        PIECE _board[_boardsize][_boardsize];
        PLAYER _player;
        bool _whiteKingSideCastle;
        bool _whiteQueenSideCastle;
        bool _blackKingSideCastle;
        bool _blackQueenSideCastle;
        int _enPassantRow;
        int _enPassantCol;
        int _halfmoveClock;
        int _fullmoveNumber;

        bool isInBounds(int row, int col) const;
        bool isPathClear(int fromRow, int fromCol, int toRow, int toCol) const;
        bool isSquareAttacked(int row, int col, PLAYER byPlayer) const;
        bool isInCheck(PLAYER player) const;
    
    public:

        Board();

        PLAYER getPlayer() const;
        PIECE getPiece(int row, int col) const;
        int getBoardSize() const;
        std::string toFEN() const;

        // Returns true when the move is illegal.
        bool checkIllegal(int fromRow, int fromCol, int toRow, int toCol) const;

        void movePiece(int fromRow, int fromCol, int toRow, int toCol,
                       PIECES promotion = QUEEN);
        bool movePiece(const std::string& move);
};

#endif