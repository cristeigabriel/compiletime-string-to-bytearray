#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <array>

#define DELIMITING_WILDCARD ' '

//	@sean :)
constexpr int _char_to_int( char ch )
{
	if( ch >= '0' && ch <= '9' )
		return ch - '0';

	if( ch >= 'A' && ch <= 'F' )
		return ch - 'A' + 10;

	return ch - 'a' + 10;
};

template <char wildcard, typename T, size_t N = sizeof( T )>
constexpr size_t _count_wildcard( T &&str )
{
	size_t count = 1u;
	for( const auto &character : str )
	{
		if( character == wildcard )
		{
			++count;
		}
	}

	return count;
}

//	construct a base16 hex and emplace it at make_count
//	change 16 to 256 if u want the result to be when:
//	sig[0] == 0xA && sig[1] == 0xB = 0xA0B
//	or leave as is for the scenario to return 0xAB
#define CONCATE_HEX_FACTOR 16
#define CONCATE_HEX(a, b) ( CONCATE_HEX_FACTOR * ( a ) + ( b ) )

template
<   char skip_wildcard,
	//	How many occurances of a delimiting wildcard do we find in sig
	size_t delimiter_count,
	typename T, size_t N = sizeof( T )>
	constexpr auto _make_array( T &&sig )
{
	static_assert( delimiter_count > 0, "this is a logical error, delimiter count can't be of size 0" );
	static_assert( N > 1, "sig length must be bigger than 1" );

	//	Resulting byte array, for delimiter_count skips we should have delimiter_count integers
	std::array<int, delimiter_count> ret{};

	//	List of skips that point to the position of the delimiter wildcard in skip
	std::array<size_t, delimiter_count> skips{};

	//	Current skip
	size_t skip_count = 0u;

	//	Character count, traversed for skip
	size_t skip_traversed_character_count = 0u;
	for( size_t i = 0u; i < N; ++i )
	{
		if( sig[i] == DELIMITING_WILDCARD )
		{
			skips[skip_count] = skip_traversed_character_count;
			++skip_count;
		}

		++skip_traversed_character_count;
	}

	//	Finally traversed character count
	size_t traversed_character_count = 0u;

	//	Make count (we will supposedly have at least an instance in our return array)
	size_t make_count = 1u;

	//	Traverse signature
	for( size_t i = 0u; i < N; ++i )
	{
		//	Read before
		if( i == 0u )
		{
			//	We don't care about this, and we don't want to use 0
			if( sig[0u] == skip_wildcard )
			{
				ret[0u] = -1;
				continue;
			}

			ret[0u] = CONCATE_HEX( _char_to_int( sig[0u] ), _char_to_int( sig[1u] ) );
			continue;
		}

		//	Make result by skip data
		for( const auto &skip : skips )
		{
			if( ( skip == i ) && skip < N - 1u )
			{
				//	We don't care about this, and we don't want to use 0
				if( sig[i + 1u] == skip_wildcard )
				{
					ret[make_count] = -1;
					++make_count;
					continue;
				}

				ret[make_count] = CONCATE_HEX( _char_to_int( sig[i + 1u] ), _char_to_int( sig[i + 2u] ) );
				++make_count;
			}
		}
	}

	return ret;
}

#define SKIP_WILDCARD '?'
#define BUILD_ARRAY(a) _make_array<SKIP_WILDCARD, _count_wildcard<DELIMITING_WILDCARD>( a )>( a )
#define BUILD_ARRAY_MV(a) _make_array<SKIP_WILDCARD, _count_wildcard<DELIMITING_WILDCARD>( std::move( a ) )>( std::move( a ) )

//	-----
//	usage
//	-----
template <int n>
constexpr int combine_two()
{
	constexpr auto numbers = BUILD_ARRAY( "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12 ?" );
	constexpr int number = numbers[0];
	constexpr int number_now = n + number;
	return number_now;
}

int main()
{
	constexpr auto shit = BUILD_ARRAY( "?? AA BB CC DD ? ? ? 02 31 32" );
	for( const auto &hex : shit )
	{
		printf( "%x ", hex );
	}

	combine_two<3>();
	constexpr auto saaahhah = combine_two<3>();
	static_assert( combine_two<3>() == 88 );
	static_assert( combine_two<3>() == saaahhah );
	printf( "\n%d", saaahhah );
}
