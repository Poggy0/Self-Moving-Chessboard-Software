#include "chessboard.h"

Board::Board()
{
    for(int i=0; i < _boardsize; i++)
    {
        for(int j=0; j < _boardsize; j++)
        {
            _board[i][j].type = EMPTY;
            _board[i][j].color = NO_PLAYER;
        }
    }

    //Black side
    for(int i=0; i < 2; i++)
    {
        for(int j=0; j < _boardsize; j++)
        {
            _board[i][j].color = BLACK;
        }
    }

    _board[0][0].type = ROOK;
    _board[0][1].type = KNIGHT;
    _board[0][2].type = BISHOP;
    _board[0][3].type = QUEEN;
    _board[0][4].type = KING;
    _board[0][5].type = BISHOP;
    _board[0][6].type = KNIGHT;
    _board[0][7].type = ROOK;

    for(int i=0; i < _boardsize; i++)
    {
        _board[1][i].type = PAWN;
    }

    //White side
    for(int i=_boardsize - 1; i > 5; i--)
    {
        for(int j=0; j < _boardsize; j++)
        {
            _board[i][j].color = WHITE;
        }
    }

    _board[7][0].type = ROOK;
    _board[7][1].type = KNIGHT;
    _board[7][2].type = BISHOP;
    _board[7][3].type = QUEEN;
    _board[7][4].type = KING;
    _board[7][5].type = BISHOP;
    _board[7][6].type = KNIGHT;
    _board[7][7].type = ROOK;

    for(int i=0; i < _boardsize; i++)
    {
        _board[6][i].type = PAWN;
    }

    _player = WHITE;
    _whiteKingSideCastle = true;
    _whiteQueenSideCastle = true;
    _blackKingSideCastle = true;
    _blackQueenSideCastle = true;
    _enPassantRow = -1;
    _enPassantCol = -1;
    _halfmoveClock = 0;
    _fullmoveNumber = 1;
}

PLAYER Board::getPlayer() const
{
    return _player;
}

PIECE Board::getPiece(int row, int col) const
{
    return _board[row][col];
}

int Board::getBoardSize() const
{
    return _boardsize;
}

std::string Board::toFEN() const
{
    std::string fen;

    for(int row=0; row < _boardsize; row++)
    {
        int emptySquares = 0;
        for(int col=0; col < _boardsize; col++)
        {
            PIECE piece = _board[row][col];
            if(piece.type == EMPTY)
            {
                emptySquares++;
                continue;
            }

            if(emptySquares > 0)
            {
                fen += static_cast<char>('0' + emptySquares);
                emptySquares = 0;
            }

            char symbol = ' ';
            switch(piece.type)
            {
                case PAWN:   symbol = 'p'; break;
                case KING:   symbol = 'k'; break;
                case QUEEN:  symbol = 'q'; break;
                case BISHOP: symbol = 'b'; break;
                case KNIGHT: symbol = 'n'; break;
                case ROOK:   symbol = 'r'; break;
                case EMPTY:  break;
            }

            if(piece.color == WHITE)
            {
                symbol = static_cast<char>(symbol - 'a' + 'A');
            }
            fen += symbol;
        }

        if(emptySquares > 0)
        {
            fen += static_cast<char>('0' + emptySquares);
        }
        if(row < _boardsize - 1)
        {
            fen += '/';
        }
    }

    fen += _player == WHITE ? " w " : " b ";

    bool hasCastlingRights = false;
    if(_whiteKingSideCastle)
    {
        fen += 'K';
        hasCastlingRights = true;
    }
    if(_whiteQueenSideCastle)
    {
        fen += 'Q';
        hasCastlingRights = true;
    }
    if(_blackKingSideCastle)
    {
        fen += 'k';
        hasCastlingRights = true;
    }
    if(_blackQueenSideCastle)
    {
        fen += 'q';
        hasCastlingRights = true;
    }
    if(!hasCastlingRights)
    {
        fen += '-';
    }

    fen += ' ';
    if(_enPassantRow < 0 || _enPassantCol < 0)
    {
        fen += '-';
    }
    else
    {
        fen += static_cast<char>('a' + _enPassantCol);
        fen += static_cast<char>('8' - _enPassantRow);
    }

    fen += ' ';
    fen += std::to_string(_halfmoveClock);
    fen += ' ';
    fen += std::to_string(_fullmoveNumber);
    return fen;
}

bool Board::isInBounds(int row, int col) const
{
    return row >= 0 && row < _boardsize && col >= 0 && col < _boardsize;
}

bool Board::isPathClear(int fromRow, int fromCol, int toRow, int toCol) const
{
    int rowStep = (toRow > fromRow) - (toRow < fromRow);
    int colStep = (toCol > fromCol) - (toCol < fromCol);
    int row = fromRow + rowStep;
    int col = fromCol + colStep;

    while(row != toRow || col != toCol)
    {
        if(_board[row][col].type != EMPTY)
        {
            return false;
        }

        row += rowStep;
        col += colStep;
    }

    return true;
}

bool Board::isSquareAttacked(int row, int col, PLAYER byPlayer) const
{
    for(int fromRow=0; fromRow < _boardsize; fromRow++)
    {
        for(int fromCol=0; fromCol < _boardsize; fromCol++)
        {
            PIECE piece = _board[fromRow][fromCol];
            if(piece.color != byPlayer)
            {
                continue;
            }

            int rowDiff = row - fromRow;
            int colDiff = col - fromCol;
            int absRowDiff = rowDiff < 0 ? -rowDiff : rowDiff;
            int absColDiff = colDiff < 0 ? -colDiff : colDiff;

            switch(piece.type)
            {
                case PAWN:
                    if(rowDiff == (byPlayer == WHITE ? -1 : 1) && absColDiff == 1)
                    {
                        return true;
                    }
                    break;

                case KNIGHT:
                    if((absRowDiff == 2 && absColDiff == 1)
                    || (absRowDiff == 1 && absColDiff == 2))
                    {
                        return true;
                    }
                    break;

                case KING:
                    if(absRowDiff <= 1 && absColDiff <= 1
                    && (absRowDiff != 0 || absColDiff != 0))
                    {
                        return true;
                    }
                    break;

                case ROOK:
                    if((rowDiff == 0 || colDiff == 0)
                    && isPathClear(fromRow, fromCol, row, col))
                    {
                        return true;
                    }
                    break;

                case BISHOP:
                    if(absRowDiff == absColDiff
                    && isPathClear(fromRow, fromCol, row, col))
                    {
                        return true;
                    }
                    break;

                case QUEEN:
                    if((rowDiff == 0 || colDiff == 0 || absRowDiff == absColDiff)
                    && isPathClear(fromRow, fromCol, row, col))
                    {
                        return true;
                    }
                    break;

                case EMPTY:
                    break;
            }
        }
    }

    return false;
}

bool Board::isInCheck(PLAYER player) const
{
    for(int row=0; row < _boardsize; row++)
    {
        for(int col=0; col < _boardsize; col++)
        {
            if(_board[row][col].type == KING && _board[row][col].color == player)
            {
                PLAYER opponent = player == WHITE ? BLACK : WHITE;
                return isSquareAttacked(row, col, opponent);
            }
        }
    }

    return true;
}

bool Board::checkIllegal(int fromRow, int fromCol, int toRow, int toCol) const
{
    if(!isInBounds(fromRow, fromCol) || !isInBounds(toRow, toCol)
    || (fromRow == toRow && fromCol == toCol))
    {
        return true;
    }

    PIECE piece = _board[fromRow][fromCol];
    PIECE target = _board[toRow][toCol];
    if(piece.color != _player || piece.type == EMPTY
    || (target.color == _player) || target.type == KING)
    {
        return true;
    }

    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;
    int absRowDiff = rowDiff < 0 ? -rowDiff : rowDiff;
    int absColDiff = colDiff < 0 ? -colDiff : colDiff;
    bool validMove = false;
    bool isCastle = false;

    switch(piece.type)
    {
        case PAWN:
        {
            int direction = piece.color == WHITE ? -1 : 1;
            int startRow = piece.color == WHITE ? 6 : 1;

            if(colDiff == 0 && rowDiff == direction && target.type == EMPTY)
            {
                validMove = true;
            }
            else if(colDiff == 0 && rowDiff == 2 * direction
            && fromRow == startRow && target.type == EMPTY
            && _board[fromRow + direction][fromCol].type == EMPTY)
            {
                validMove = true;
            }
            else if(absColDiff == 1 && rowDiff == direction
            && target.color != NO_PLAYER && target.color != piece.color)
            {
                validMove = true;
            }
            else if(absColDiff == 1 && rowDiff == direction
            && toRow == _enPassantRow && toCol == _enPassantCol
            && target.type == EMPTY
            && _board[fromRow][toCol].type == PAWN
            && _board[fromRow][toCol].color != piece.color)
            {
                validMove = true;
            }
            break;
        }

        case KNIGHT:
            validMove = (absRowDiff == 2 && absColDiff == 1)
                     || (absRowDiff == 1 && absColDiff == 2);
            break;

        case BISHOP:
            validMove = absRowDiff == absColDiff
                     && isPathClear(fromRow, fromCol, toRow, toCol);
            break;

        case ROOK:
            validMove = (rowDiff == 0 || colDiff == 0)
                     && isPathClear(fromRow, fromCol, toRow, toCol);
            break;

        case QUEEN:
            validMove = (rowDiff == 0 || colDiff == 0 || absRowDiff == absColDiff)
                     && isPathClear(fromRow, fromCol, toRow, toCol);
            break;

        case KING:
            if(absRowDiff <= 1 && absColDiff <= 1)
            {
                validMove = true;
            }
            else if(rowDiff == 0 && absColDiff == 2)
            {
                bool kingSide = colDiff > 0;
                bool castleRight = piece.color == WHITE
                    ? (kingSide ? _whiteKingSideCastle : _whiteQueenSideCastle)
                    : (kingSide ? _blackKingSideCastle : _blackQueenSideCastle);
                int rookCol = kingSide ? 7 : 0;
                int rookRow = piece.color == WHITE ? 7 : 0;
                int step = kingSide ? 1 : -1;
                PLAYER opponent = piece.color == WHITE ? BLACK : WHITE;

                if(fromRow == rookRow && fromCol == 4 && toCol == (kingSide ? 6 : 2)
                && castleRight && _board[rookRow][rookCol].type == ROOK
                && _board[rookRow][rookCol].color == piece.color
                && isPathClear(fromRow, fromCol, rookRow, rookCol)
                && !isInCheck(piece.color))
                {
                    Board transit = *this;
                    transit._board[fromRow][fromCol].type = EMPTY;
                    transit._board[fromRow][fromCol].color = NO_PLAYER;
                    transit._board[fromRow][fromCol + step] = piece;

                    if(!transit.isSquareAttacked(fromRow, fromCol + step, opponent))
                    {
                        validMove = true;
                        isCastle = true;
                    }
                }
            }
            break;

        case EMPTY:
            break;
    }

    if(!validMove)
    {
        return true;
    }

    Board positionAfterMove = *this;
    if(piece.type == PAWN && fromCol != toCol && target.type == EMPTY)
    {
        positionAfterMove._board[fromRow][toCol].type = EMPTY;
        positionAfterMove._board[fromRow][toCol].color = NO_PLAYER;
    }
    positionAfterMove._board[fromRow][fromCol].type = EMPTY;
    positionAfterMove._board[fromRow][fromCol].color = NO_PLAYER;
    positionAfterMove._board[toRow][toCol] = piece;

    if(isCastle)
    {
        int rookFromCol = toCol > fromCol ? 7 : 0;
        int rookToCol = toCol > fromCol ? toCol - 1 : toCol + 1;
        positionAfterMove._board[toRow][rookToCol] =
            positionAfterMove._board[toRow][rookFromCol];
        positionAfterMove._board[toRow][rookFromCol].type = EMPTY;
        positionAfterMove._board[toRow][rookFromCol].color = NO_PLAYER;
    }

    if(positionAfterMove.isInCheck(piece.color))
    {
        return true;
    }

    return false;
}

void Board::movePiece(int fromRow, int fromCol, int toRow, int toCol, PIECES promotion)
{
    if(checkIllegal(fromRow, fromCol, toRow, toCol))
    {
        return;
    }

    PIECE temp = _board[fromRow][fromCol];
    bool promotes = temp.type == PAWN
                 && (toRow == 0 || toRow == _boardsize - 1);
    if(promotes && promotion != QUEEN && promotion != ROOK
    && promotion != BISHOP && promotion != KNIGHT)
    {
        return;
    }

    PIECE captured = _board[toRow][toCol];
    bool isEnPassant = temp.type == PAWN && fromCol != toCol
                    && captured.type == EMPTY;
    bool isCapture = captured.type != EMPTY || isEnPassant;
    bool isCastle = temp.type == KING && fromRow == toRow
                 && (toCol - fromCol == 2 || fromCol - toCol == 2);

    if(isEnPassant)
    {
        _board[fromRow][toCol].type = EMPTY;
        _board[fromRow][toCol].color = NO_PLAYER;
    }

    _board[fromRow][fromCol].type = EMPTY;
    _board[fromRow][fromCol].color = NO_PLAYER;

    if(isCastle)
    {
        int rookFromCol = toCol > fromCol ? 7 : 0;
        int rookToCol = toCol > fromCol ? toCol - 1 : toCol + 1;
        _board[toRow][rookToCol] = _board[toRow][rookFromCol];
        _board[toRow][rookFromCol].type = EMPTY;
        _board[toRow][rookFromCol].color = NO_PLAYER;
    }

    _board[toRow][toCol] = temp;

    if(promotes)
    {
        _board[toRow][toCol].type = promotion;
    }

    if(temp.type == KING)
    {
        if(temp.color == WHITE)
        {
            _whiteKingSideCastle = false;
            _whiteQueenSideCastle = false;
        }
        else
        {
            _blackKingSideCastle = false;
            _blackQueenSideCastle = false;
        }
    }

    if(temp.type == ROOK)
    {
        if(fromRow == 7 && fromCol == 0) _whiteQueenSideCastle = false;
        if(fromRow == 7 && fromCol == 7) _whiteKingSideCastle = false;
        if(fromRow == 0 && fromCol == 0) _blackQueenSideCastle = false;
        if(fromRow == 0 && fromCol == 7) _blackKingSideCastle = false;
    }

    if(captured.type == ROOK)
    {
        if(toRow == 7 && toCol == 0) _whiteQueenSideCastle = false;
        if(toRow == 7 && toCol == 7) _whiteKingSideCastle = false;
        if(toRow == 0 && toCol == 0) _blackQueenSideCastle = false;
        if(toRow == 0 && toCol == 7) _blackKingSideCastle = false;
    }

    _enPassantRow = -1;
    _enPassantCol = -1;
    if(temp.type == PAWN && (toRow - fromRow == 2 || fromRow - toRow == 2))
    {
        _enPassantRow = (fromRow + toRow) / 2;
        _enPassantCol = fromCol;
    }

    if(temp.type == PAWN || isCapture)
    {
        _halfmoveClock = 0;
    }
    else
    {
        _halfmoveClock++;
    }

    if(_player == BLACK)
    {
        _fullmoveNumber++;
    }

    _player = _player == WHITE ? BLACK : WHITE;
}

bool Board::movePiece(const std::string& move)
{
    if(move.length() != 4 && move.length() != 5)
    {
        return false;
    }

    char fromFile = move[0];
    char fromRank = move[1];
    char toFile = move[2];
    char toRank = move[3];

    if(fromFile >= 'A' && fromFile <= 'H') fromFile += 'a' - 'A';
    if(toFile >= 'A' && toFile <= 'H') toFile += 'a' - 'A';

    if(fromFile < 'a' || fromFile > 'h'
    || toFile < 'a' || toFile > 'h'
    || fromRank < '1' || fromRank > '8'
    || toRank < '1' || toRank > '8')
    {
        return false;
    }

    PIECES promotion = QUEEN;
    if(move.length() == 5)
    {
        char promotionChar = move[4];
        if(promotionChar >= 'A' && promotionChar <= 'Z')
        {
            promotionChar += 'a' - 'A';
        }

        switch(promotionChar)
        {
            case 'q': promotion = QUEEN; break;
            case 'r': promotion = ROOK; break;
            case 'b': promotion = BISHOP; break;
            case 'n': promotion = KNIGHT; break;
            default: return false;
        }
    }

    int fromCol = fromFile - 'a';
    int fromRow = '8' - fromRank;
    int toCol = toFile - 'a';
    int toRow = '8' - toRank;
    if(checkIllegal(fromRow, fromCol, toRow, toCol))
    {
        return false;
    }

    PIECE piece = _board[fromRow][fromCol];
    bool promotes = piece.type == PAWN && (toRow == 0 || toRow == _boardsize - 1);
    if(move.length() == 5 && !promotes)
    {
        return false;
    }

    movePiece(fromRow, fromCol, toRow, toCol, promotion);
    return true;
}