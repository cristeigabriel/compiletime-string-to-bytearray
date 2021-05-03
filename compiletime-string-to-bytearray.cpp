#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <array>

template <char token, typename T, size_t N = sizeof( T )>
constexpr size_t _count_token( T &&str )
{
	size_t count = 1u;
	for( const auto &character : str )
	{
		if( character == token )
		{
			++count;
		}
	}

	return count;
}

template <size_t CT, typename T, size_t N = sizeof( T )>
constexpr auto _make_array( T &&sig )
{
	//	@sean :)
	constexpr auto char_to_int = []( char input ) -> int
	{
		if( input >= '0' && input <= '9' )
			return input - '0';

		if( input >= 'A' && input <= 'F' )
			return input - 'A' + 10;

		return input - 'a' + 10;
	};

	std::array<int, CT> ret{};
	std::array<int, CT> skips{};

	size_t skip_count = 0u;
	size_t character_count = 0u;
	for( int i = 0; i < N; ++i )
	{
		if( sig[i] == ' ' )
		{
			skips[skip_count] = character_count;
			++skip_count;
		}
		++character_count;
	}

	size_t traversed_character_count = 0u;
	size_t make_count = 1u;

	for( int i = 0; i < N; ++i )
	{
		if( i == 0 )
		{
			//	construct a base16 hex and emplace it at make_count
			//	change 16 to 256 if u want the result to be when:
			//	sig[0] == 0xA && sig[1] == 0xB = 0xA0B
			//	or leave as is for the scenario to return 0xAB
			ret[0] = 16 * char_to_int( sig[0] ) + char_to_int( sig[1] );
			continue;
		}

		for( const auto &skip : skips )
		{
			if( ( skip == i /*&& !computed_skips[skip]*/ ) && skip < N - 1 )
			{
				//	construct a base16 hex and emplace it at make_count
				//	change 16 to 256 if u want the result to be when:
				//	sig[0] == 0xA && sig[1] == 0xB = 0xA0B
				//	or leave as is for the scenario to return 0xAB
				ret[make_count] = 16 * char_to_int( sig[i + 1] ) + char_to_int( sig[i + 2] );
				++make_count;
			}
		}
	}

	return ret;
}

#define BUILD_ARRAY(a) _make_array<_count_token<' '>( std::move( a ) )>( std::move( a ) )

int main()
{
	constexpr auto shit = BUILD_ARRAY( "AA BB CC DD 01 02 31 32" );
	for( const auto &hex : shit )
	{
		printf( "%x", hex );
	}
}