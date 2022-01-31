#include <iostream>

using namespace std;

#include <vector>
#include <array>
#include <ctype.h>
#include <bits/stdc++.h>

vector<int> getKingMoves(const int pInt[], int index) {
    vector<int> moves;
    int positions[8] = {-9, -8, -7, -1, 1, 7, 8, 9};
    for (int position: positions) {
        int newIndex = index + position;
        // if the index is on the board and the new space is empty or an enemy piece
        if (newIndex >= 0 && newIndex < 64 && (pInt[newIndex] == 0 || pInt[newIndex] >> 3 != pInt[index] >> 3)) {
            // also prevent screen wrapping
            // make sure the max column difference is 1 or less
            if (abs(index % 8 - newIndex % 8) <= 1) {
                moves.push_back(newIndex);
            }
        }
    }
    return moves;
}

vector<int> getRookMoves(const int pInt[], int index) {
    vector<int> moves;
    int offsets[4] = {-1, -8, 1, 8};
    // max number of moves to the edge of the board
    // takes the form of left, up, right, down
    int maxOffsets[4] = {index % 8, index / 8, 7 - (index % 8), 7 - (index / 8)};
    // continue to add moves until the piece is blocked
    for (int i = 0; i < 4; i++) {
        int offset = offsets[i];
        int maxOffset = maxOffsets[i];
        for (int j = 1; j <= maxOffset; j++) {
            int newIndex = index + offset * j;
            // if the index is on the board and the new space is empty or an enemy piece
            if (newIndex >= 0 && newIndex < 64 && (pInt[newIndex] == 0 || pInt[newIndex] >> 3 != pInt[index] >> 3)) {
                moves.push_back(newIndex);
                // if the new space is an enemy piece, stop
                if (pInt[newIndex] != 0 && pInt[newIndex] >> 3 != pInt[index] >> 3) {
                    break;
                }
            } else {
                break;
            }
        }
    }

    return moves;
}

vector<int> getBishopMoves(const int pInt[], int index) {
    vector<int> moves;
    int offsets[4] = {-9, -7, 9, 7};
    // max number of moves to the edge of the board
    // takes the form of left up, right up, right down, left down
    int row = index / 8 + 1;
    int col = index % 8 + 1;
    int maxOffsets[4] = {min(row, col) - 1, min(row, 9 - col) - 1, 8 - max(row, col), 8 - max(row, 9 - col)};

    // continue to add moves until the piece is blocked
    for (int i = 0; i < 4; i++) {
        int offset = offsets[i];
        int maxOffset = maxOffsets[i];
        for (int j = 1; j <= maxOffset; j++) {
            int newIndex = index + offset * j;
            // if the index is on the board and the new space is empty or an enemy piece
            if (newIndex >= 0 && newIndex < 64 && (pInt[newIndex] == 0 || pInt[newIndex] >> 3 != pInt[index] >> 3)) {
                moves.push_back(newIndex);
                // if the new space is an enemy piece, stop
                if (pInt[newIndex] >> 3 != pInt[index] >> 3) {
                    break;
                }
            } else {
                break;
            }
        }
    }
    return moves;
}

vector<int> getQueenMoves(const int pInt[], int index) {
    vector<int> rookMoves = getRookMoves(pInt, index);
    vector<int> bishopMoves = getBishopMoves(pInt, index);
    rookMoves.insert(rookMoves.end(), bishopMoves.begin(), bishopMoves.end());
    return rookMoves;
}

vector<int> getKnightMoves(const int pInt[], int index) {
    vector<int> moves;
    int offsets[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
    for (int position: offsets) {
        int newIndex = index + position;
        // if the index is on the board and the new space is empty or an enemy piece
        if (newIndex >= 0 && newIndex < 64 && (pInt[newIndex] == 0 || pInt[newIndex] >> 3 != pInt[index] >> 3)) {
            moves.push_back(newIndex);
        }
    }
    return moves;
}

vector<int> getPawnMoves(const int pInt[], int index, int enPassant) {
    vector<int> moves;
    int direction = pInt[index] >> 3 == 1 ? 1 : -1;
    int possibleMoves[4] = {direction * 7, direction * 9, direction * 8, direction * 16};
    for (int i = 0; i < 2; i++) {
        int newIndex = index + possibleMoves[i];
        // if the index is on the board and the new space is an enemy piece
        if (newIndex >= 0 && newIndex < 64 && (pInt[newIndex] >> 3 != pInt[index] >> 3) || newIndex == enPassant) {
            moves.push_back(newIndex);
        }
    }
    // if there is nothing in front of the pawn, add the move
    if (pInt[index + direction * 8] == 0) {
        moves.push_back(index + direction * 8);
    }
    // if the pawn is on the first row and there is nothing in front of it, add the move two steps forward
    if (direction == 1 && index / 8 == 1 && pInt[index + direction * 8] == 0 && pInt[index + direction * 16] == 0) {
        moves.push_back(index + direction * 16);
    }
    if (direction == -1 && index / 8 == 6 && pInt[index + direction * 8] == 0 && pInt[index + direction * 16] == 0) {
        moves.push_back(index + direction * 16);
    }

    return moves;
}

vector<int> getMovesFromPieceValue(const int pInt[64], int index, int enPassant) {
    vector<int> moves;
    switch (pInt[index] % 8) {
        case 1:
            moves = getPawnMoves(pInt, index, enPassant);
            break;
        case 2:
            moves = getKnightMoves(pInt, index);
            break;
        case 3:
            moves = getBishopMoves(pInt, index);
            break;
        case 4:
            moves = getRookMoves(pInt, index);
            break;
        case 5:
            moves = getQueenMoves(pInt, index);
            break;
        case 6:
            moves = getKingMoves(pInt, index);
            break;
        default:
            break;
    }
    return moves;
}

void showBoard(const int board[]) {
    cout << "\x1B[2J\x1B[H\n\n";

    // piece type is found by using the following formula
    // (board[i] &= ~(1 << 3))
    // or one may use
    // (board[i] % 8)

    // piece color is found by using the following formula
    // (board[i] & (1 << 3))

    for (int i = 0; i < 64; i++) {
        switch (board[i] % 8) {
            case 1:
                cout << (char)('P' + ((board[i] >> 3) ? 32 : 0));
                break;
            case 2:
                cout << (char)('N' + ((board[i] >> 3) ? 32 : 0));
                break;
            case 3:
                cout << (char)('B' + ((board[i] >> 3) ? 32 : 0));
                break;
            case 4:
                cout << (char)('R' + ((board[i] >> 3) ? 32 : 0));
                break;
            case 5:
                cout << (char)('Q' + ((board[i] >> 3) ? 32 : 0));
                break;
            case 6:
                cout << (char)('K' + ((board[i] >> 3) ? 32 : 0));
                break;
            default:
                cout << " ";
                break;
        }

        cout << " | ";
        if (i % 8 == 7) {
            cout << endl;
        }
    }
}

int fenValue(char c) {
    switch (c) {
        case 'P':
            return 1;
        case 'N':
            return 2;
        case 'B':
            return 3;
        case 'R':
            return 4;
        case 'Q':
            return 5;
        case 'K':
            return 6;
        default:
            return 0;
    }
}

void setupBoard(int board[64], string &fen, int &enPassant, int &whiteToMove, int &castling) {
    int skip = 0;
    int index = 0;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (skip != 0) {
                skip--;
                board[y*8 + x] = 0;
                continue;
            }
            if (fen[index] == '/') {
                skip = 0;
                index++;
            }
            if (isdigit(fen[index])) {
                board[y*8+x] = 0;
                skip = fen[index] - '1';
                index++;
            } else {
                board[y*8+x] = (fenValue((char) toupper(fen[index])) + (isupper(fen[index]) ? 0 : 8));;
                index++;
            }
        }
        index++;
    }

    istringstream ss(fen);
    string token;
    int i = 0;
    while (ss >> token) {
        if (i == 1) {
            whiteToMove = (token == "w");
        } else if (i == 2) {
            enPassant = (token == "-" ? -1 : (token[1] - 'a') + (token[0] - '1') * 8);
        } else if (i == 3) {
            castling = 0;
            if (token.find('K') != string::npos) {
                castling |= 1;
            }
            if (token.find('Q') != string::npos) {
                castling |= 2;
            }
            if (token.find('k') != string::npos) {
                castling |= 4;
            }
            if (token.find('q') != string::npos) {
                castling |= 8;
            }
        }
        i++;
    }
}

bool isSpotInCheck(const int pInt[64], int index, bool white) {
    // create a list of all the tiles that can be attacked by the enemy
    vector<int> enemyIndexes;
    // find all the indexes of the enemy pieces
    for (int i = 0; i < 64; i++) {
        if (pInt[i] != 0 && pInt[i] >> 3 == white) {
            enemyIndexes.push_back(i);
        }
    }
    // for each enemy piece, check if it can attack the king
    for (int enemy : enemyIndexes) {
        // get the total possible moves for the piece
        vector<int> moves = getMovesFromPieceValue(pInt, enemy, -1);

        // remove pawn moves that cannot capture
        if (pInt[enemy] % 8 == 1) {
            for (int i = 0; i < moves.size(); i++) {
                if (abs(enemy - moves[i]) == 8 || abs(enemy - moves[i]) == 16) {
                    moves.erase(moves.begin() + i);
                    i--;
                }
            }
        }
        // check if this index is in the list of moves
        for (int move: moves) {
            if (move == index) {
                return true;
            }
        }
    }
    return false;
}

bool isKingInCheck(const int pInt[64], bool white) {
    int kingValue = white ? 6 : 14;
    // find the king
    int kingIndex = -1;
    for (int i = 0; i < 64; i++) {
        if (pInt[i] == kingValue) {
            kingIndex = i;
            break;
        }
    }

    return isSpotInCheck(pInt, kingIndex, white);
}

bool isKingInCheckmate(const int board[64], bool white, int enPassant) {
    if (!isKingInCheck(board, white)) {
        return false;
    }

    bitset<64> tilesUnderAttack = 0;
    for (int i = 0; i < 64; i++) {
        if (board[i] % 8 != 0 && (board[i] >> 3) == white) {
            for (int move : getMovesFromPieceValue(board, i, enPassant)) {
                tilesUnderAttack[move] = true;
            }
        }
    }

    int newBoard[64];
    for (int i = 0; i < 64; i++) {
        newBoard[i] = board[i];
    }

    // get all the moves that don't put the king in check
    for (int i = 0; i < 64; i++) {
        if (board[i] % 8 != 0 && (board[i] >> 3) != white) {
            for (int move : getMovesFromPieceValue(board, i, enPassant)) {
                newBoard[move] = board[i];
                newBoard[i] = 0;
                if (!isKingInCheck(newBoard, white)) {
                    return false;
                }
            }
        }
    }

    return true;
}

int main() {

    // stores the tile for en passant capture
    // this is -1 if there is no en passant capture
    // resets at the end of each turn unless there is a new en passant capture
    int enPassant = -1;

    // this int represents the rights to castle for each piece in the following order:
    // A8, E8, H8, A1, E1, H1
    // when the value is 0, the piece is not allowed to castle
    int castlingRights = 0b111111;

    // input values for the move of castling
    const int castlingMoves[4] = {5838, 5878, 5131, 5171};
    bool isWhitesTurn = true;

    int board[64];
    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
//    string fen = "1k6/8/Q7/8/8/8/8/2R3K1 w - - 0 1";
    setupBoard(board, fen, enPassant, reinterpret_cast<int &>(isWhitesTurn), castlingRights);

    showBoard(board);

    // main game loop
    while (true) {
        // get the input from the user
        string input;
        cin >> input;
        if (input == "q") {
            break;
        } else {
            // make sure it is at least 4 characters long
            if (input.length() < 4) {
                cout << "Invalid input" << endl;
                continue;
            }
            // cast string as a four digit integer where the first digit is the start column, the second is the start row, the third is the end column, and the fourth is the end row
            int startCol = stoi(input.substr(0, 1));
            int startRow = stoi(input.substr(1, 1));
            int endCol = stoi(input.substr(2, 1));
            int endRow = stoi(input.substr(3, 1));
            // if there is a fifth character, it is the promotion piece
            unsigned char promotionPiece = ' ';
            if (input.length() == 5) {
                promotionPiece = input.at(4);
            }
            switch (promotionPiece) {
                case 'Q':
                    promotionPiece = 5;
                    break;
                case 'R':
                    promotionPiece = 4;
                    break;
                case 'B':
                    promotionPiece = 3;
                    break;
                case 'N':
                    promotionPiece = 2;
                    break;
                default:
                    promotionPiece = 1;
                    break;
            }

            if (promotionPiece == 1 && input.length() == 5) {
                cout << "Invalid Promotion" << endl;
                continue;
            }

            // make sure the start and end are valid
            if (startCol < 1 || startCol > 8 || startRow < 1 || startRow > 8 || endCol < 1 || endCol > 8 ||
                endRow < 1 || endRow > 8) {
                cout << "Invalid input" << endl;
                continue;
            }
            // make sure the start and end are not the same
            if (startCol == endCol && startRow == endRow) {
                cout << "Invalid input" << endl;
                continue;
            }

            // convert the row and column to an index
            int startIndex = (8 - startRow) * 8 + (startCol - 1);
            int endIndex = (8 - endRow) * 8 + (endCol - 1);
            // get the piece at the start index
            int piece = board[startIndex];

            // if the piece is not the correct color, it is an invalid move
            if (piece >> 3 != !isWhitesTurn) {
                cout << "Invalid turn" << endl;
                continue;
            }

            // get the total possible moves for the piece
            vector<int> moves = getMovesFromPieceValue(board, startIndex, enPassant);

            // check if the move is within the possible moves
            bool isValid = false;
            for (int move: moves) {
                if (move == endIndex) {
                    isValid = true;
                    break;
                }
            }

            // check for castling
            if (castlingMoves[0] == stoi(input) && (castlingRights >> 5) & 1 &&
                (castlingRights >> 4) & 1 && !isSpotInCheck(board, 2, isWhitesTurn) && !isSpotInCheck(board, 3, isWhitesTurn) && !isKingInCheck(board, isWhitesTurn)) {
                isValid = true;
                board[0] = 0;
                board[3] = 12;
            }
            if (castlingMoves[1] == stoi(input) && (castlingRights >> 4) & 1 &&
                (castlingRights >> 3) & 1 && !isSpotInCheck(board, 5, isWhitesTurn) && !isSpotInCheck(board, 6, isWhitesTurn) && !isKingInCheck(board, isWhitesTurn)) {
                isValid = true;
                board[7] = 0;
                board[5] = 12;
            }
            if (castlingMoves[2] == stoi(input) && (castlingRights >> 2) & 1 &&
                (castlingRights >> 1) & 1 && !isSpotInCheck(board, 58, isWhitesTurn) && !isSpotInCheck(board, 59, isWhitesTurn) && !isKingInCheck(board, isWhitesTurn)) {
                isValid = true;
                board[56] = 0;
                board[59] = 4;
            }
            if (castlingMoves[3] == stoi(input) && (castlingRights >> 1) & 1 &&
                (castlingRights >> 0) & 1 && !isSpotInCheck(board, 61, isWhitesTurn) && !isSpotInCheck(board, 62, isWhitesTurn) && !isKingInCheck(board, isWhitesTurn)) {
                isValid = true;
                board[63] = 0;
                board[61] = 4;
            }

            if (!isValid) {
                cout << "Invalid move" << endl;
                continue;
            }

            std::array<int, 64> oldBoard = reinterpret_cast<array<int, 64> &&>(board);

            board[startIndex] = 0;
            board[endIndex] = piece;

            // check if this move puts the king in check
            if (isKingInCheck(board, isWhitesTurn)) {
                cout << "Invalid move" << endl;
                // reset the board
                copy(begin(oldBoard), end(oldBoard), begin(board));
                continue;
            }

            // check if this move puts the king in checkmate
            if (isKingInCheckmate(board, !isWhitesTurn, enPassant)) {
                cout << "Checkmate " << (isWhitesTurn ? "White" : "Black") << " Wins!" << endl;
                // end the game loop
                break;
            }

            // if the piece is a pawn, check if it can be promoted and check if it can be en passant
            // reset the en passant
            enPassant = -1;
            if (piece % 8 == 1) {
                if (endRow == 8 || endRow == 1) {
                    board[endIndex] = (int) promotionPiece + (isWhitesTurn ? 0 : 8);
                    cout << "Promotion to " << (int) promotionPiece << endl;
                }
                if (startIndex - endIndex == 16) {
                    enPassant = endIndex + 8;
                }
                if (startIndex - endIndex == -16) {
                    enPassant = endIndex - 8;
                }
            }


            if (piece % 8 == 4 || piece % 8 == 6) {
                switch (startIndex) {
                    case 0:
                        castlingRights = castlingRights & ~(1 << 5);
                        break;
                    case 4:
                        castlingRights = castlingRights & ~(1 << 4);
                        break;
                    case 7:
                        castlingRights = castlingRights & ~(1 << 3);
                        break;
                    case 56:
                        castlingRights = castlingRights & ~(1 << 2);
                        break;
                    case 60:
                        castlingRights = castlingRights & ~(1 << 1);
                        break;
                    case 63:
                        castlingRights = castlingRights & ~(1 << 0);
                        break;
                    default:
                        break;
                }
            }

            // change turns
            isWhitesTurn = !isWhitesTurn;
        }


        showBoard(board);
    }

    return 0;
}


