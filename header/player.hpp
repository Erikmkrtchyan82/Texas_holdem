#ifndef TEXAS_HOLDEM_PLAYER_HPP
#define TEXAS_HOLDEM_PLAYER_HPP


#include "cards.hpp"
#include "category.hpp"

#include <string>
#include <vector>

namespace player {

class player {
public:
    friend player create_player( cards::cards_vec && );

    friend std::ostream &operator<<( std::ostream &, player & );

    void find_category();

    void set_community( cards::cards_vec );

    category::category_type get_category() const;

    // Sets player's `mark_` to `winner_`
    void winner();

    int get_rank() const;

private:
    // Private constructor
    // Creating player with friend function `create_player` to avoid throwing exception from constructor
    explicit player( cards::cards_vec &&c );

    // Finds sum of player's all cards rank
    void find_rank();

    char mark_{' '};
    int player_rank_ = 0;
    const char winner_{'W'};
    cards::cards_vec cards_vec_{};
    category::category_type category_{};
};

// Throws `exceptions::hole_cards_error`, and `exceptions::duplicate_card_error` from `cards::check_duplicates` function
player create_player( cards::cards_vec && );

std::ostream &operator<<( std::ostream &, player & );

// Finds category with highest precedence from `players`, then finds highest rank of all players with same category,
//  and sets that players as winner
void find_winner( std::vector< player > &players );

}   // namespace player

#endif //TEXAS_HOLDEM_PLAYER_HPP
