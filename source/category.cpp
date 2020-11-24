#include "cards.hpp"
#include "category.hpp"

#include <string>
#include <algorithm>
#include <type_traits>

namespace category {

using cards::cards_vec;
using cards::card_t;

// Returns `suit`'s precedence
inline size_t find( const char &suit ) noexcept {
    return cards::suits.at( suit );
}

// Returns `rank`'s precedence
inline size_t find( std::string &rank ) noexcept {
    return cards::ranks.at( rank );
}

// Sorting cards from [ begin; end ) by ranks
inline void sort_by_ranks( cards_vec::iterator begin, cards_vec::iterator end ) noexcept {
    std::sort( begin, end, []( card_t &card1, card_t &card2 ) { return find( card1.second ) > find( card2.second ); } );
}

// Sorting cards from [begin; end) by suits
inline void sort_by_suits( cards_vec::iterator begin, cards_vec::iterator end ) noexcept {
    std::sort( begin, end, []( card_t &card1, card_t &card2 ) { return find( card1.first ) > find( card2.first ); } );
}

// Checks if cards from [begin; end) are sequential, by using rank's precedence
inline bool is_sequential( cards_vec::iterator begin, cards_vec::iterator end ) {
    return std::adjacent_find( begin, end, []( card_t &card1, card_t &card2 ) {
        return ( find( card1.second ) - 1 ) != find( card2.second );
    } ) == end;
}


category_type flushes( cards_vec &cards ) {

    // Searching 5 or more suits that are same, if any exist
    char suit = [&]() -> char {
        for ( auto &s : cards::suits ) {
            char c = s.first;
            int n = std::count_if( cards.begin(), cards.end(), [&]( card_t &card ) { return ( card.first == c ); } );
            if ( n >= 5 )
                return c;
        }
        return char{};
    }();

    if ( suit == char{} )
        return {{}, Categories::No_category};


    sort_by_suits( cards.begin(), cards.end());

    // `first` is first `suit` card in `cards`,
    // `second` is first non `suit` card in `cards` after `first`
    auto first = std::find_if( cards.begin(), cards.end(), [&]( card_t &card ) { return card.first == suit; } );
    auto last = std::find_if_not( first + 5, cards.end(), [&]( card_t &card ) { return card.first == suit; } );

    cards_vec flush_cards{first, last};
    sort_by_ranks( flush_cards.begin(), flush_cards.end());

    // `it` contains an iterator and count, if found at least 4 elements and if [iterator; iterator + count) are sequential
    // `it` points to (iterator - count) element,
    // if not found `it`'s iterator points to the end of `flush_cards`.
    // `count` is incrementing every time, when found 2 sequential cards,
    // so if `count` is 4, it means that found 5 sequential cards.
    auto it = [&]() -> std::pair< cards_vec::iterator, int > {

        int count = 0;
        auto it1 = flush_cards.begin();

        for ( ; it1 != flush_cards.end() - 1; ++it1 ) {
            if ( is_sequential( it1, it1 + 2 ))
                ++count;
            else if ( count < 3 )
                count = 0;
            else break;
        }

        if ( count > 2 )
            return {( it1 - count ), count};

        return {flush_cards.end(), 0};
    }();

    // If found more then 4 seq. cards ...
    if ( it.second > 3 ) {

        auto temp = cards_vec{it.first, it.first + 5};
        cards.swap( temp );

        // and if the first card's rank is `A`
        if ( cards.front().second == "A" ) {
            return {"Straight flush: Royal", Categories::Royal_straight_flush};
        }

        return {"Straight flush", Categories::Straight_flush};
    }

    // If found 4 seq. cards, whose first cards rank is`5`, i.e. ( 5 4 3 2 ),
    // and highest card`s rank from `flush_cards` is `A`
    if ( it.second == 3 &&
         flush_cards.front().second == "A" &&
         it.first->second == "5" ) {

        std::rotate( flush_cards.begin(), it.first, it.first + 4 );

        // Leaving first 5 cards in `flush_cards`
        while ( flush_cards.size() > 5 ) {
            flush_cards.pop_back();
        }

        cards.swap( flush_cards );

        return {"Straight flush: Steel wheel", Categories::Steel_wheel_straight_flush};
    }

    // Leaving first 5 cards in `flush_cards`
    while ( flush_cards.size() > 5 )
        flush_cards.pop_back();

    cards.swap( flush_cards );

    return {"Flush", Categories::Flush};
}

category_type four_of_a_kind( cards_vec &cards ) {

    sort_by_ranks( cards.begin(), cards.end());

    // Searching 4 cards with same rank,
    // if found returns iterator to the first card,
    // else returns iterator to `cards.end()`
    auto it = [&]() {
        for ( auto i = cards.begin(); i != cards.end() - 3; ++i ) {
            int n = std::count_if( i, cards.end(), [&]( card_t &x ) { return ( x.second == i->second ); } );
            if ( n == 4 )
                return i;
        }
        return cards.end();
    }();

    if ( it == cards.end())
        return {{}, Categories::No_category};

    if ( cards.begin() != it )
        std::rotate( cards.begin(), it, it + 4 );

    cards_vec vec( cards.begin(), cards.begin() + 5 );
    cards.swap( vec );

    sort_by_suits( cards.begin(), cards.end() - 1 );

    return {"Four of a kind", Categories::Four_of_a_kind};
}

category_type full_house( cards_vec &cards ) {

    sort_by_ranks( cards.begin(), cards.end());

    // `card` contains 2 string, first is rank of the card, which `cards` has 3 element,
    // and second is the rank - which `cards` has 2 elements
    auto card = [&]() {

        std::pair< std::string, std::string > p;

        for ( auto it = cards.begin(); it != cards.end() - 1 && it != cards.end(); ++it ) {
            int n = std::count_if( it, cards.end(), [&]( card_t &a ) { return a.second == it->second; } );
            if ( n == 3 ) {
                // If already found 3 cards with same rank, then
                // if new 3 card's rank has higher precedence than the previous
                if ( p.first.empty() || ( find( it->second ) > find( p.first ))) {
                    p.first = it->second;
                    it += 2;
                }
            }
            else if ( n == 2 ) {
                if ( p.second.empty() || ( find( it->second ) > find( p.second ))) {
                    p.second = it->second;
                }
            }
        }
        return p;
    }();

    // If any of the card's tuple was not found
    if ( card.first.empty() || card.second.empty())
        return {{}, Categories::No_category};

    // `it_3` is an iterator to the first element, whose rank repeats in `cards` 3 times
    auto it_3 = std::find_if( cards.begin(), cards.end(), [&]( card_t &a ) {
        return a.second == card.first;
    } );

    std::rotate( cards.begin(), it_3, it_3 + 3 );

    // `it_2` is an iterator to the first element, whose rank repeats in `cards` 2 times
    auto it_2 = std::find_if( cards.begin(), cards.end(), [&]( card_t &a ) {
        return a.second == card.second;
    } );

    std::rotate( cards.begin() + 3, it_2, it_2 + 2 );

    cards_vec vec{cards.begin(), cards.begin() + 5};
    cards.swap( vec );

    sort_by_suits( cards.begin(), cards.begin() + 3 );
    sort_by_suits( cards.begin() + 3, cards.end());

    return {"Full house", Categories::Full_house};
}

category_type straight( cards_vec &cards ) {

    cards_vec cards_copy = cards;

    sort_by_ranks( cards_copy.begin(), cards_copy.end());

    // Erasing cards that has duplicate ranks in `cards_copy`
    cards_copy.erase(
            std::unique( cards_copy.begin(), cards_copy.end(), []( card_t &a, card_t &b ) {
                return a.second == b.second;
            } ),
            cards_copy.end());

    // `it` first argument points to the first iterator of sequential part of `cards_copy`
    // and the second argument contains number of seq. elements.
    // `count` is same as in `flushes` function
    auto it = [&]() -> std::pair< cards_vec::iterator, int > {

        int count = 0;

        auto it1 = cards_copy.begin();

        for ( ; it1 != cards_copy.end() - 1; ++it1 ) {
            if ( is_sequential( it1, it1 + 2 ))
                ++count;
                // if found less than 4 seq. cards
            else if ( count < 3 )
                count = 0;
            else break;
        }

        if ( count > 2 )
            return {( it1 - count ), count};

        return {cards_copy.end(), 0};
    }();

    // If doesn't found at least 4 seq. cards
    if ( it.second < 3 )
        return {{}, Categories::No_category};

    // If found more than 4 seq. cards
    if ( it.second > 3 ) {

        auto vec = cards_vec{it.first, it.first + 5};
        cards.swap( vec );

        if ( cards.front().second == "A" ) {
            return {"Straight: Broadway", Categories::Broadway_straight};
        }

        return {"Straight", Categories::Straight};
    }


    // If found 4 seq. cards,
    // if first card's rank in `cards_copy` is `A`, and first card's rank in seq. cards is `5`
    if ( cards_copy.front().second == "A" && it.first->second == "5" ) {

        std::rotate( cards_copy.begin(), it.first, it.first + it.second + 1 );
        auto temp = cards_vec{cards_copy.begin(), cards_copy.begin() + 5};
        cards.swap( temp );

        return {"Straight: Baby", Categories::Baby_straight};
    }

    return {{}, Categories::No_category};
}

category_type three_of_a_kind( cards_vec &cards ) {

    sort_by_ranks( cards.begin(), cards.end());

    // Iterator to the first card, whose rank repeats 3 times
    auto it = cards.begin();

    for ( ; it != cards.end() - 2; ++it ) {
        if ( it->second == ( it + 1 )->second && it->second == ( it + 2 )->second )
            break;
    }

    if ( std::distance( it, cards.end()) == 2 )
        return {{}, Categories::No_category};

    std::rotate( cards.begin(), it, it + 3 );

    while ( cards.size() > 5 )
        cards.pop_back();

    sort_by_suits( cards.begin(), cards.begin() + 3 );

    return {"Three of a kind", Categories::Three_of_a_kind};
}

category_type two_pair( cards_vec &cards ) {

    sort_by_ranks( cards.begin(), cards.end());

    auto find_pair = []( auto begin, auto end ) {
        return std::adjacent_find( begin, end, []( auto &a, auto &b ) {
            return a.second == b.second;
        } );
    };

    auto first = find_pair( cards.begin(), cards.end());

    auto second = cards.end();

    // Checks if pair exists, and if so searching second pair after first one.
    if ( first != cards.end())
        second = find_pair( first + 2, cards.end());

    if ( second == cards.end())
        return {{}, Categories::No_category};

    std::rotate( cards.begin(), first, first + 2 );
    std::rotate( cards.begin() + 2, second, second + 2 );

    sort_by_suits( cards.begin(), cards.begin() + 2 );
    sort_by_suits( cards.begin() + 2, cards.begin() + 4 );

    while ( cards.size() > 5 )
        cards.pop_back();

    return {"Two pair", Categories::Two_pair};
}

category_type pair( cards_vec &cards ) {

    sort_by_ranks( cards.begin(), cards.end());

    auto find_pair = []( auto begin, auto end ) {
        return std::adjacent_find( begin, end, []( auto &a, auto &b ) {
            return a.second == b.second;
        } );
    };

    auto first = find_pair( cards.begin(), cards.end());

    if ( first == cards.end())
        return {{}, Categories::No_category};

    std::rotate( cards.begin(), first, first + 2 );

    while ( cards.size() > 5 )
        cards.pop_back();

    return {"Pair", Categories::One_pair};
}

category_type high_card( cards_vec &cards ) {

    sort_by_ranks( cards.begin(), cards.end());

    if ( cards.size() > 5 ) {
        auto temp = cards_vec{cards.begin(), cards.begin() + 5};
        cards.swap( temp );
    }

    return {"High card", Categories::High_card};
}


template < typename... Funcs >
category_type search( cards_vec &cards, Funcs ... func ) {

    using common_func_type = category_type( * )( cards_vec & );
    static_assert( std::is_same_v< typename std::common_type< Funcs... >::type, common_func_type > );

    category_type category;
    //	Using short-circuit evaluation
    (( category = func( cards ), category.second != Categories::No_category ) || ... );

    return category;
}

category_type find_category( cards_vec &cards ) {

    // Calls `search` function with poker hand's category functions from high rank to low.
    return search( cards,
                   flushes,
                   four_of_a_kind,
                   full_house,
                   straight,
                   three_of_a_kind,
                   two_pair,
                   pair,
                   high_card );
}

}   // namespace category

