#ifndef PARSER_HPP_INCLUDED
#define PARSER_HPP_INCLUDED

#include "cards.hpp"

#include <string>

namespace parser {

// Returns vector of cards from string
cards::cards_vec parse_cards( const std::string &cards );

}    // namespace parser

#endif // !PARSER_HPP_INCLUDED
