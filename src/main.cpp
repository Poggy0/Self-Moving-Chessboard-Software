#include <stdio.h>
#include "chessboard.h"

void printBoard(Board board);
void printPiece(PIECE piece);

int main()
{
    Board chessboard;

    chessboard.movePiece("e2e4");
    chessboard.movePiece("e7e5");
    chessboard.movePiece("g1f3");
    chessboard.movePiece("e8e7");
    chessboard.movePiece("f3e5");
    

    printBoard(chessboard);
    printf("\n\n");
    printf("%s\n", chessboard.toFEN().c_str());

    return 0;
}

void printBoard(Board board)
{
    printf("White graveyard            Board                      Black graveyard\n");
    for(int row=0; row < board.getBoardSize(); row++)
    {
        if(row < board.getGraveyardRows())
        {
            for(int col=0; col < board.getBoardSize(); col++)
            {
                printPiece(board.getGraveyardPiece(WHITE, row, col));
            }
        }
        else
        {
            printf("                        ");
        }

        printf(" | ");

        for(int col=0; col < board.getBoardSize(); col++)
        {
            printPiece(board.getPiece(row, col));
        }

        printf(" | ");

        if(row < board.getGraveyardRows())
        {
            for(int col=0; col < board.getBoardSize(); col++)
            {
                printPiece(board.getGraveyardPiece(BLACK, row, col));
            }
        }
        printf("\n");
    }
}

void printPiece(PIECE piece)
{
    char color = piece.color == WHITE ? 'W' : 'B';
    char type = 'X';

    switch(piece.type)
    {
        case KING:   type = 'K'; break;
        case QUEEN:  type = 'Q'; break;
        case BISHOP: type = 'B'; break;
        case KNIGHT: type = 'H'; break;
        case ROOK:   type = 'R'; break;
        case PAWN:   type = 'P'; break;
        case EMPTY:  break;
    }

    if(piece.type == EMPTY)
    {
        printf("XX ");
    }
    else
    {
        printf("%c%c ", color, type);
    }
}