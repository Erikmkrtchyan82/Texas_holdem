#ifndef EXCEPTIONS_HPP_INCLUDED
#define EXCEPTIONS_HPP_INCLUDED

#include <stdexcept>

namespace exceptions {

struct invalid_card_suit : std::invalid_argument {
	using std::invalid_argument::invalid_argument;
};

struct invalid_card_rank :std::invalid_argument {
	using std::invalid_argument::invalid_argument;
};

struct community_cards_error :std::range_error {
	using std::range_error::range_error;
};

struct hole_cards_error :std::range_error {
	using std::range_error::range_error;
};

struct duplicate_card_error :std::logic_error {
	using std::logic_error::logic_error;
};

}	// namespace exceptions 

#endif // !EXCEPTIONS_HPP_INCLUDED
