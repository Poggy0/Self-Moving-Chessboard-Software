#ifndef CHESSBOARD
#define CHESSBOARD

#include <map>
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
    BLACK,
    DRAW

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
        const static int _graveyardRows = 2;
        PIECE _board[_boardsize][_boardsize];
        PIECE _whiteGraveyard[_graveyardRows][_boardsize];
        PIECE _blackGraveyard[_graveyardRows][_boardsize];
        PLAYER _player;
        bool _whiteKingSideCastle;
        bool _whiteQueenSideCastle;
        bool _blackKingSideCastle;
        bool _blackQueenSideCastle;
        int _enPassantRow;
        int _enPassantCol;
        int _halfmoveClock;
        int _fullmoveNumber;
        PLAYER _winner;
        std::map<std::string, int> _positionOccurrences;

        bool isInBounds(int row, int col) const;
        bool isPathClear(int fromRow, int fromCol, int toRow, int toCol) const;
        bool isSquareAttacked(int row, int col, PLAYER byPlayer) const;
        bool isInCheck(PLAYER player) const;
        std::string getPositionKey() const;
    
    public:

        Board();

        void reset_game();
        PLAYER getPlayer() const;
        PLAYER getWinner() const;
        PIECE getPiece(int row, int col) const;
        PIECE getGraveyardPiece(PLAYER color, int row, int col) const;
        int getGraveyardRows() const;
        int getBoardSize() const;
        std::string toFEN() const;

        // Sets the winner (or DRAW) when the current position is terminal.
        bool checkGameOver();

        // Returns true when the move is illegal.
        bool checkIllegal(int fromRow, int fromCol, int toRow, int toCol) const;

        void movePiece(int fromRow, int fromCol, int toRow, int toCol,
                       PIECES promotion = QUEEN);
        bool movePiece(const std::string& move);
};

#endif