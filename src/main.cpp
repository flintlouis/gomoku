#include "Board.hpp"
#include "GUI.hpp"
#include "PrimitiveGui.hpp"
#include "gomoku.hpp"
#include "heuristic.hpp"
#include "TranspositionTable.hpp"
#include "misc.hpp"
#include "algorithm.hpp"
#include "argument_parser.hpp"

class NegamaxAi : public IAi
{
public:
	int	calculate(Board &board)
    {
        if (board.is_empty())
            return misc::calc_index(9,9);

        int move = this->iterative_deepening_negamax(board, board.get_current_player());
        return move;
    }
private:
    int iterative_deepening_negamax(Board board, int player)
    {
        TIMEOUT_REACHED = false;
        int best_move = -1;
        Timer timer;
        timer.start();
        int last_best_move;
        int depth = 1;
        int max_depth = 100;
        
        for (; depth <= max_depth && !TIMEOUT_REACHED; depth++)
        {
            
            TranspositionTable h_table;
            TranspositionTable t_table;
            try
            {
                last_best_move = negamax(board, depth, depth, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), player, t_table, h_table, true, timer);
            }
            catch(const char* e)
            {   
                // PRINT("depth: " << depth - 1);
                // board.print_principal_variation(player, depth - 1, t_table);
                // PRINT("timeout reached during depth: " << depth << ".\nusing previous depth search results:");
                board.place(best_move);
                TableEntry tt_entry;
                t_table.retrieve(board, tt_entry);
                // PRINT("heuristic: " << tt_entry.value);
                return best_move;
            }
            best_move = last_best_move;
        }
        return best_move;
    }
};

int                     main(int argc, char **argv)
{
    po::options_description options = argument_parser::get_options();
    po::variables_map vm = argument_parser::get_args(argc, argv, options);

    if (vm.count("help"))
    {
        std::cout << options << '\n';
        exit(0);
    }

    NegamaxAi nai;
    GUI gui = argument_parser::get_gui(vm, nai);
    if (vm.count("replay"))
    {
        try
        {
            gui.replay(argument_parser::get_file(vm));
        }
        catch(const char *e)
        {
            std::cerr << e << '\n';
            exit(1);
        }
    }
    else
        gui.play(Board());
}
