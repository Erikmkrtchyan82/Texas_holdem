#ifndef CARDS_HPP_INCLUDED
#define CARDS_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>

namespace cards {

// Type alias for every `card`, first argument is for suit, second is for rank
using card_t = std::pair< char, std::string >;

using cards_vec = std::vector< card_t >;

// This stores every card that is used in game
static cards_vec used_cards;

// All suits with their precedences
const std::map< const char, size_t > suits{
        {'S', 1},
        {'H', 2},
        {'D', 3},
        {'C', 4}
};

// All ranks with their precedences
const std::map< std::string, size_t > ranks{
        {"2",  1},
        {"3",  2},
        {"4",  3},
        {"5",  4},
        {"6",  5},
        {"7",  6},
        {"8",  7},
        {"9",  8},
        {"10", 9},
        {"J",  10},
        {"Q",  11},
        {"K",  12},
        {"A",  13}
};

// Checks, if all cards are from same deck, if not throws `exceptions::duplicate_card_error`
void check_duplicates( const cards_vec &hand );

}    // namespace category

#endif // !CARDS_HPP_INCLUDED
