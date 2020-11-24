#ifndef TEXAS_HOLDEM_CATEGORY_HPP
#define TEXAS_HOLDEM_CATEGORY_HPP

#include "cards.hpp"

#include <string>

namespace category {

// All possible categories in poker game
enum class Categories {
    No_category,
    High_card,
    One_pair,
    Two_pair,
    Three_of_a_kind,
    Baby_straight,
    Straight,
    Broadway_straight,
    Flush,
    Full_house,
    Four_of_a_kind,
    Steel_wheel_straight_flush,
    Straight_flush,
    Royal_straight_flush
};

// Type alias, to store player's category and it's string representation
using category_type = std::pair<std::string, Categories>;

category_type find_category(cards::cards_vec& cards );

// Returns `rank`'s precedence
inline size_t find( std::string& rank ) noexcept;

}   // namespace category

#endif //TEXAS_HOLDEM_CATEGORY_HPP
