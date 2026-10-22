#include <stdio.h>
#include "chessboard.h"

void printBoard(Board board);

int main()
{
    Board chessboard;

    chessboard.movePiece("e2e4");
    chessboard.movePiece("e7e5");
    chessboard.movePiece("g1f3");
    

    printBoard(chessboard);
    printf("\n\n");
    printf("%s\n", chessboard.toFEN().c_str());

    return 0;
}

void printBoard(Board board)
{
    PIECE temp;

    for(int i=0; i < board.getBoardSize(); i++)
    {
        for(int j=0; j < board.getBoardSize(); j++)
        {
            temp = board.getPiece(i, j);

            if(temp.color == WHITE)
            {
                switch(temp.type)
                {
                    case KING:
                        printf("WK ");
                        break;

                    case QUEEN:
                        printf("WQ ");
                        break;

                    case BISHOP:
                        printf("WB ");
                        break;

                    case KNIGHT:
                        printf("WH ");
                        break;

                    case ROOK:
                        printf("WR ");
                        break;

                    case PAWN:
                        printf("WP ");
                        break;
                }
            }
            else if(temp.color == BLACK)
            {
                switch(temp.type)
                {
                    case KING:
                        printf("BK ");
                        break;

                    case QUEEN:
                        printf("BQ ");
                        break;

                    case BISHOP:
                        printf("BB ");
                        break;

                    case KNIGHT:
                        printf("BH ");
                        break;

                    case ROOK:
                        printf("BR ");
                        break;

                    case PAWN:
                        printf("BP ");
                        break;
                }
            }
            else
            {
                printf("XX ");
            }
        }
        printf("\n");
    }
}