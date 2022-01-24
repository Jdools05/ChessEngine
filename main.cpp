#include <iostream>

using namespace std;

#include <vector>
#include <array>

vector<int> getPawnMoves(const int pInt[64], int index, int enPassant);

vector<int> getKnightMoves(const int pInt[64], int index);

vector<int> getBishopMoves(const int pInt[64], int index);

vector<int> getRookMoves(const int pInt[64], int index);

vector<int> getQueenMoves(const int pInt[64], int index);

vector<int> getKingMoves(const int pInt[64], int index);

bool isInCheck(const int pInt[64], bool white);

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
                cout << "P";
                break;
            case 2:
                cout << "N";
                break;
            case 3:
                cout << "B";
                break;
            case 4:
                cout << "R";
                break;
            case 5:
                cout << "Q";
                break;
            case 6:
                cout << "K";
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

int main() {

//    int emptyBoard[64] = {0, 0, 0, 0, 0, 0, 0, 0,
//                          0, 0, 0, 0, 0, 0, 0, 0,
//                          0, 0, 0, 0, 0, 0, 0, 0,
//                          0, 0, 0, 0, 0, 0, 0, 0,
//                          0, 0, 0, 0, 0, 0, 0, 0,
//                          0, 0, 0, 0, 0, 0, 0, 0,
//                          0, 0, 0, 0, 0, 0, 0, 0,
//                          0, 0, 0, 0, 0, 0, 0, 0};
//
//    vector<int> testMoves = getKnightMoves(emptyBoard, 28);
//    for (int move : testMoves) {
//        cout << move << endl;
//    }

    int enPassant = -1;

    // this int represents the rights to castle for each piece in the following order:
    // A8, E8, H8, A1, E1, H1
    // when the value is 0, the piece is not allowed to castle
    int castlingRights = 0b111111;

    // input values for the move of castling
    const int castlingMoves[4] = {5838, 5878, 5131, 5171};

    // initialize the chess board with the pieces
    int board[64] = {12, 10, 11, 13, 14, 11, 10, 12,
                     9, 9, 9, 9, 9, 9, 9, 9,
                     0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0,
                     1, 1, 1, 1, 1, 1, 1, 1,
                     4, 2, 3, 5, 6, 3, 2, 4};

//    int board[64] = {14, 0, 0, 0, 0, 0, 0, 0,
//                     0, 0, 0, 0, 0, 0, 0, 0,
//                     12, 0, 0, 0, 0, 0, 0, 0,
//                     0, 0, 0, 0, 0, 0, 0, 0,
//                     0, 0, 0, 0, 0, 0, 0, 0,
//                     0, 0, 0, 0, 0, 0, 0, 0,
//                     0, 0, 0, 0, 0, 0, 0, 0,
//                     5, 0, 0, 0, 0, 0, 0, 6};

    bool isWhitesTurn = true;

    showBoard(board);

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
            vector<int> moves;

            // get the moves for the piece
            switch (piece % 8) {
                case 1:
                    moves = getPawnMoves(board, startIndex, enPassant);
                    break;
                case 2:
                    moves = getKnightMoves(board, startIndex);
                    break;
                case 3:
                    moves = getBishopMoves(board, startIndex);
                    break;
                case 4:
                    moves = getRookMoves(board, startIndex);
                    break;
                case 5:
                    moves = getQueenMoves(board, startIndex);
                    break;
                case 6:
                    moves = getKingMoves(board, startIndex);
                    break;
                default:
                    break;
            }

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
                (castlingRights >> 4) & 1) {
                isValid = true;
                board[0] = 0;
                board[3] = 12;
            }
            if (castlingMoves[1] == stoi(input) && (castlingRights >> 4) & 1 &&
                (castlingRights >> 3) & 1) {
                isValid = true;
                board[7] = 0;
                board[5] = 12;
            }
            if (castlingMoves[2] == stoi(input) && (castlingRights >> 2) & 1 &&
                (castlingRights >> 1) & 1) {
                isValid = true;
                board[56] = 0;
                board[59] = 4;
            }
            if (castlingMoves[3] == stoi(input) && (castlingRights >> 1) & 1 &&
                (castlingRights >> 0) & 1) {
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

            // check if this move puts the king in check
            if (isInCheck(board, !isWhitesTurn)) {
                cout << "Invalid move" << endl;
                // reset the board
                copy(begin(oldBoard), end(oldBoard), begin(board));
                continue;
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

bool isInCheck(const int pInt[64], bool white) {
    int kingValue = white ? 4 : 12;
    // find the king
    int kingIndex = -1;
    for (int i = 0; i < 64; i++) {
        if (pInt[i] == kingValue) {
            kingIndex = i;
            break;
        }
    }

    // check if the king is in check
    // create a bitboard of all the tiles that can be attacked by the enemy
    vector<int> enemyIndexes;
    // find all the indexes of the enemy pieces
    for (int i = 0; i < 64; i++) {
        if (pInt[i] != 0 && pInt[i] >> 3 != white) {
            enemyIndexes.push_back(i);
        }
    }
    // for each enemy piece, check if it can attack the king
    for (int enemy : enemyIndexes) {
        // get the total possible moves for the piece
        vector<int> moves;

        // get the moves for the piece
        switch (enemy % 8) {
            case 1:
                moves = getPawnMoves(pInt, enemy, -1);
                break;
            case 2:
                moves = getKnightMoves(pInt, enemy);
                break;
            case 3:
                moves = getBishopMoves(pInt, enemy);
                break;
            case 4:
                moves = getRookMoves(pInt, enemy);
                break;
            case 5:
                moves = getQueenMoves(pInt, enemy);
                break;
            case 6:
                moves = getKingMoves(pInt, enemy);
                break;
            default:
                break;
        }

        // remove pawn moves that cannot capture
        if (pInt[enemy] % 8 == 1) {
            for (int i = 0; i < moves.size(); i++) {
                if (abs(enemy - moves[i])== 8 || abs(enemy - moves[i]) == 16) {
                    moves.erase(moves.begin() + i);
                    i--;
                }
            }
        }

        // check if the king is in any of the moves
        for (int move : moves) {
            if (move == kingIndex) {
                return true;
            }
        }
    }

    return false;
}

vector<int> getKingMoves(const int pInt[], int index) {
    vector<int> moves;
    int positions[8] = {-9, -8, -7, -1, 1, 7, 8, 9};
    for (int position: positions) {
        int newIndex = index + position;
        // if the index is on the board and the new space is empty or an enemy piece
        if (newIndex >= 0 && newIndex < 64 && (pInt[newIndex] == 0 || pInt[newIndex] >> 3 != pInt[index] >> 3)) {
            moves.push_back(newIndex);
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


