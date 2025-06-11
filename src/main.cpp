#include <iostream>
#include "chess.hpp"

using namespace chess;

Move chosen;
int max_depth = 6;

bool is_checkmate(const Board& board) 
{
    if (!board.inCheck())
		return false;
    Movelist moves;
    movegen::legalmoves<>(moves, board);
    return moves.empty();
}

static constexpr int PIECE_VALUES[] = {
    /* PAWN   */ 1,
    /* KNIGHT */ 3,
    /* BISHOP */ 3,
    /* ROOK   */ 5,
    /* QUEEN  */ 9,
    /* KING   */ 0,
    /* NONE   */ 0
};

double evaluate(const Board& board)
{
	if (board.sideToMove() == Color::WHITE && is_checkmate(board))
		return -99999;  
	if (board.sideToMove() == Color::BLACK && is_checkmate(board))
		return 99999;  

	double score = 0.0;
	for (auto pt : { 	PieceType::underlying::PAWN,
						PieceType::underlying::KNIGHT,
						PieceType::underlying::BISHOP,
						PieceType::underlying::ROOK,
						PieceType::underlying::QUEEN,
						PieceType::underlying::KING }) 
	{
		Bitboard wb = board.pieces(PieceType(static_cast<PieceType::underlying>(pt)), Color::WHITE);
		Bitboard bb = board.pieces(PieceType(static_cast<PieceType::underlying>(pt)), Color::BLACK);

		int cntW = wb.count();
		int cntB = bb.count();
		int val  = PIECE_VALUES[static_cast<int>(pt)];

		score += val * (cntW - cntB);
	}

	return score;
}

double search(Board& board, int depth, Color player, double alpha, double beta)
{
	if(depth == 0)
		return evaluate(board);
	
	double score = 0.0;
	Movelist moves;
	movegen::legalmoves(moves, board);

	if(player == Color::WHITE) {
		double best_score = -99999;
		for(auto move : moves) {
			board.makeMove(move);
			double cur_eval = search(board, depth - 1, Color::BLACK, alpha, beta);
			board.unmakeMove(move);
			
			if(depth == max_depth && cur_eval > best_score)
				chosen = move;
			
			best_score = std::max(cur_eval, best_score);
			if(best_score > alpha)
				alpha = best_score;
			
			// if best_score is more than what black can guarantee - break
			if(best_score >= beta) 
				break;
		}
		score = best_score;
	}
	if(player == Color::BLACK) {
		double best_score = 99999;
		for(auto move : moves) {
			board.makeMove(move);
			double cur_eval = search(board, depth - 1, Color::WHITE, alpha, beta);
			board.unmakeMove(move);

			if(depth == max_depth && cur_eval < best_score)
				chosen = move;

			best_score = std::min(cur_eval, best_score);
			if(best_score < beta)
				beta = best_score;

			if(best_score <= alpha)
				break;
		}
		score = best_score;
	}

	return score;
}

int main()
{
    Board board = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	while(true) {
		search(board, max_depth, Color::WHITE, -99999, 99999);
		board.makeMove(chosen);

		std::cout << uci::moveToUci(chosen) << std::endl; 

		std::string player_move_uci;
		std::cin >> player_move_uci;
		Move player_move = uci::uciToMove(board, player_move_uci);
		board.makeMove(player_move); 
	}
	
	return 0;
}