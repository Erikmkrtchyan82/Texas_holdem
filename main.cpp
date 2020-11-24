#include <iostream>
#include <stdexcept>

#include "parser.hpp"
#include "player.hpp"
#include "community.hpp"

int main( int argc, char *argv[] ) {

    if ( argc < 4 ) {
        std::cerr << "Usage: " << argv[ 0 ] <<
                  " <community-category> <player1-hole-category> <player2-hole-category> [<playerN-hole-category>]...\n";
        return -1;
    }

    try {
        auto community = community::create_community( parser::parse_cards( argv[ 1 ] ));

        std::vector< player::player > players;

        for ( int i = 2; i < argc; ++i ) {
            std::string s = argv[ i ];
            player::player p = player::create_player( parser::parse_cards( argv[ i ] ));
            p.set_community( community );
            players.emplace_back( p );
        }

        for ( auto &pl:players ) {
            pl.find_category();
        }

        player::find_winner( players );

        for ( auto &pl:players ) {
            std::cout << pl << std::endl;
        }

    }
    catch ( std::exception &e ) {
        std::cerr << e.what() << std::endl;
        return -2;
    }
    catch ( ... ) {
        std::cerr << "Unexpected error" << std::endl;
        return -3;
    }

    return 0;

}
