#include <sstream>

#include "cards.hpp"
#include "community.hpp"
#include "exceptions.hpp"

namespace community {

community::community( cards::cards_vec &&c ) : cards_( std::move( c )) {
    // Inserts cards as used
    cards::used_cards.insert( cards::used_cards.end(), cards_.begin(), cards_.end());
}

community::operator cards::cards_vec() const {
    return cards_;
}

community create_community( cards::cards_vec &&c ) {
    if ( c.size() < 3 || c.size() > 5 )
        throw exceptions::community_cards_error( "Wrong number of community cards (should be from 3 to 5)" );
    cards::check_duplicates( c );
    return community( std::move( c ));
}

}   // namespace community
