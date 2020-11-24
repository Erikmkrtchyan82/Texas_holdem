#include "cards.hpp"
#include "exceptions.hpp"

#include <string>
#include <algorithm>

namespace cards {

void check_duplicates( const cards_vec &hand ) {
	using namespace std::string_literals;

	// Checks duplicate cards in `hand`
	for ( auto it = hand.begin(); it != hand.end() - 1; ++it ) {
		if ( std::find( it + 1, hand.end(), *it ) != hand.end() ) {
			throw exceptions::duplicate_card_error( "Duplicate card dealt: "s + it->first + it->second );
		}
	}

	// Checks, if any card from `hand` was used before
	for ( auto &x : hand ) {
		if ( std::find( used_cards.begin(), used_cards.end(), x ) != used_cards.end() )
			throw exceptions::duplicate_card_error( "Duplicate card dealt: "s + x.first + x.second );
	}
	
}

}   // namespace category