#include "cards.hpp"
#include "player.hpp"
#include "exceptions.hpp"

#include <sstream>
#include <algorithm>

namespace player {

player::player( cards::cards_vec &&c ) : cards_vec_( std::move( c ) ) {
    // Inserts cards as used
    cards::used_cards.insert( cards::used_cards.end(), cards_vec_.begin(), cards_vec_.end() );
}

// Output is - [mark] [cards] [category type]
std::ostream& operator<<(std::ostream&os,player&pl){
	os << pl.mark_ << ' ';
	for ( auto&[suit, rank] : pl.cards_vec_ )
		os << suit << rank;
	os << ' ' << pl.category_.first;
	return os;
}

void player::set_community( cards::cards_vec c ) {
	cards_vec_.insert( cards_vec_.end(), c.begin(), c.end() );
}

void player::find_category() {
	category_ = category::find_category( cards_vec_ );
	find_rank();
}

category::category_type player::get_category() const {
	return category_;
}

void player::winner() {
	mark_ = winner_;
}

player create_player( cards::cards_vec &&c ) {
	if ( c.size() != 2 )
		throw exceptions::hole_cards_error( "Wrong number of hole cards (should be 2)" );
	cards::check_duplicates( c );
	return player( std::move( c ) );
}

void find_winner( std::vector<player>& players ) {

    // Finds category with highest precedence from `players`
	auto max_category = ( std::max_element( players.begin(), players.end(),
											[]( player&x, player&y ) {
												return ( x.get_category().second < y.get_category().second );
											} )->get_category() ).second;

	// Iterators to all winner candidate players
	std::vector<std::vector<player>::iterator> max_players;

	for ( auto x = players.begin(); x != players.end(); ++x ) {
		if ( x->get_category().second == max_category )
			max_players.emplace_back( x );
	}


	int max_rank = 0;

	// Finds maximum ranks form winner candidate players
	std::for_each( max_players.begin(), max_players.end(), [&]( std::vector<player>::iterator& a ) {
		if ( a->get_rank() > max_rank )
            max_rank = a->get_rank();
				   } );

	// Sets all players with `max_rank` rank to winner
	for(auto&a:max_players ) {
		if ( a->get_rank() == max_rank )
			a->winner();
	}
}

void player::find_rank() {
	for ( auto&x : cards_vec_ ) {
		player_rank_ += category::find( x.second );
	}
}

int player::get_rank() const {
	return player_rank_;
}

} // namespace player
