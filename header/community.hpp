#ifndef TEXAS_HOLDEM_COMMUNITY_HPP
#define TEXAS_HOLDEM_COMMUNITY_HPP

#include "cards.hpp"

#include <string>

namespace community {

class community {
public:
    friend community create_community( cards::cards_vec &&c );

    operator cards::cards_vec() const;

private:
    // Private constructor
    // Creating community with friend function `create_community` to avoid throwing exception from constructor
    explicit community( cards::cards_vec &&c );

    cards::cards_vec cards_;
};

// Throws `exceptions::community_cards_error`, and `exceptions::duplicate_card_error` from `cards::check_duplicates` function
community create_community( cards::cards_vec &&c );

}   // namespace community

#endif //TEXAS_HOLDEM_COMMUNITY_HPP
