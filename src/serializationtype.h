#pragma once

/*-------------------
TODO:
neet to refactoring serialization classes!
---------------------*/

#include <stdint.h>
#include <vector>
#include <map>
#include <string>
#include "buffer.h"
#include "exception.h"

namespace g2
	{
	namespace serialization_type
		{
		typedef uint8_t byte_t;
		typedef uint16_t word_t;
		typedef uint32_t dword_t;
		typedef uint64_t qword_t;
		typedef byte_t id_t;
		typedef dword_t size_t;
		typedef byte_t nc_opt_t;

		enum TYPE
			{
			UNKNOWN     = 0x00,
			PRIMITIVE   = 0x40, // primitive type. not array, hash, map, string and so on			
			POD         = 0x80, // plain old data
			
			BYTE        = 0x01 | POD | PRIMITIVE,
			WORD        = 0x02 | POD | PRIMITIVE,
			DWORD       = 0x03 | POD | PRIMITIVE,
			QWORD       = 0x04 | POD | PRIMITIVE,
			STRUCT      = 0x05 | POD | PRIMITIVE,
			FIXED_ARRAY = 0x06,
			ARRAY       = 0x07,
			VECTOR      = 0x08,
			MAP         = 0x09,
			STRING      = 0x10,
			G2_BUFFER   = 0x11,
			NET_CONT    = 0x12
			};

		const id_t ID_UNKNOWN     = UNKNOWN;
		const id_t ID_POD         = POD;
		const id_t ID_PRIMITIVE   = PRIMITIVE;
		const id_t ID_BYTE        = BYTE;
		const id_t ID_WORD        = WORD;
		const id_t ID_DWORD       = DWORD;
		const id_t ID_QWORD       = QWORD;
		const id_t ID_STRUCT      = STRUCT;
		const id_t ID_FIXED_ARRAY = FIXED_ARRAY;
		const id_t ID_ARRAY       = ARRAY;
		const id_t ID_VECTOR      = VECTOR;
		const id_t ID_MAP         = MAP;
		const id_t ID_STRING      = STRING;
		const id_t ID_G2_BUFFER   = G2_BUFFER;
		const id_t ID_NET_CONT    = NET_CONT;

		// type id
		const size_t PRIMITIVE_HEADER_SIZE = sizeof( id_t );
		// type id + struct size
		const size_t STRUCT_HEADER_SIZE = sizeof( id_t ) + sizeof( size_t );
		// type id + size
		const size_t STRING_HEADER_SIZE = sizeof( id_t ) + sizeof( size_t );
		// type id + size
		const size_t G2_BUFFER_HEADER_SIZE = sizeof( id_t ) + sizeof( size_t );
		// type id + size + options
		const size_t NET_CONT_HEADER_SIZE = sizeof( id_t ) + sizeof( size_t ) + sizeof( nc_opt_t );
		// type id + 32bit crc value
		const size_t NET_CONT_CHECK_SUM_SIZE = sizeof( id_t ) + sizeof( dword_t );

		// ID definitons
		template < class Type >	struct type_pair;
#define PRIMITIVE_TYPE_PAIR( type ) template <> struct type_pair< type > { static const id_t ID; };
//#define ARRAY_TYPE_PAIR template < class T, dword_t size > struct type_pair< T[size] >{ static const id_t ID = ID_FIXED_ARRAY; };
#define STRUCT_TYPE_PAIR template < class T > struct type_pair{ static const id_t ID = ID_STRUCT; };
//#define VECTOR_TYPE_PAIR template < class T > struct type_pair< std::vector< T > >{ static const id_t ID = ID_VECTOR; };
//#define MAP_TYPE_PAIR template < class K, class V > struct type_pair< std::map< K, V > >{ static const id_t ID = ID_MAP; };
		PRIMITIVE_TYPE_PAIR( byte_t )
		PRIMITIVE_TYPE_PAIR( word_t )
		PRIMITIVE_TYPE_PAIR( dword_t )
		PRIMITIVE_TYPE_PAIR( qword_t )
		STRUCT_TYPE_PAIR
		/*
		ARRAY_TYPE_PAIR
		VECTOR_TYPE_PAIR
		MAP_TYPE_PAIR
		*/
		PRIMITIVE_TYPE_PAIR( std::string )
		PRIMITIVE_TYPE_PAIR( g2::Buffer )
#undef STRUCT_TYPE_PAIR
#undef PRIMITIVE_TYPE_PAIR
//#undef VECTOR_TYPE_PAIR
//#undef MAP_TYPE_PAIR

		//-----------------------------------------------------------------------------------------//
		class TypeMismatchException :public g2::Exception
			{
			public:
				explicit TypeMismatchException( string message = "", int number = -1, int code = -1 );
				TypeMismatchException( const TypeMismatchException& );
				TypeMismatchException& operator = ( const TypeMismatchException& );
				virtual ~TypeMismatchException();
			};

		//-----------------------------------------------------------------------------------------//
		class DebinarizeException :public g2::Exception
			{
			public:
				explicit DebinarizeException( string message = "", int number = -1, int code = -1 );
				DebinarizeException( const DebinarizeException& );
				DebinarizeException& operator = ( const DebinarizeException& );
				virtual ~DebinarizeException();
			};

		//-----------------------------------------------------------------------------------------//
		namespace network_container_options
			{
			//const nc_opt_t INVALID    = 0x80; //0b10000000;
			const nc_opt_t NONE       = 0x00; //0b00000000;
			const nc_opt_t CHECK_SUM  = 0x01; //0b00000001;
			const nc_opt_t COMPRESSED = 0x02; //0b00000010;
			}
		}
	}
