#ifndef TRANSPOSITION_TABLE_HPP
# define TRANSPOSITION_TABLE_HPP

# include "Board.hpp"
# include <unordered_map>

class TableEntry
{
    public:
        int value;
        int depth;
        int flag;
        bool game_finished;
};

class TranspositionTable
{
    private:
        std::unordered_map<std::bitset<MASKSIZE>, TableEntry> t_table;

    public:
        bool lookup(Board &node, TableEntry &tt_entry);
        void insert(Board &node, TableEntry &tt_entry);
        void update(Board &node, int value);
        size_t size();
};

#endif
