#include "cards.hpp"
#include "parser.hpp"
#include "exceptions.hpp"

#include <string>
#include <sstream>

namespace parser {

bool is_valid( const char &suit ) noexcept {
	try {
		[[maybe_unused]] auto x = cards::suits.at( suit );
		return true;
	}
	catch ( std::out_of_range & ) {
		return false;
	}
}

bool is_valid( const std::string &rank ) noexcept {
	try {
		[[maybe_unused]] auto x = cards::ranks.at( rank );
		return true;
	}
	catch ( std::out_of_range & ) {
		return false;
	}
}

// Checks if `suit` and `rank` are valid, if so creates card and returns
// throws `exceptions::invalid_card_suit`
cards::card_t add( const char &suit, const std::string &rank ) {
	using namespace std::string_literals;
	if ( !is_valid( suit ) )
		throw exceptions::invalid_card_suit( "Invalid card suit "s + suit );
	if ( !is_valid( rank ) )
		throw exceptions::invalid_card_rank( "Invalid card rank "s + rank );

	return { suit, rank };
}

// Parsing string to cards, returning vector of cards
cards::cards_vec parse_cards( const std::string &cards_string ) {

	cards::cards_vec cards;
	std::istringstream ss( cards_string );

	char character;

	while ( ss >> character ) {

		char suit = character;

		std::string rank{};
		if ( ss >> character )
			rank += character;

		// If `character` was digit...
		if ( std::isdigit( character ) ) {
			// continue reading all digits.
			while ( ss >> character && std::isdigit( character ) ) {
				rank += character;
			}
			// If `character` was not digit, putting back in stream.
			ss.putback( character );
		}

		cards.push_back( add( suit, rank ) );
	}

	return cards;
}

}   // namespace parser

