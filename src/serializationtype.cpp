#include "serializationtype.h"

namespace g2
	{
	namespace serialization_type
		{
#define PRIMITIVE_TYPE_DEFINE( type, id ) const id_t type_pair< type >::ID = id;
/*
#define STRUCT_TYPE_DEFINE( id ) template < class T > const id_t type_pair< T >::ID = id;
#define VECTOR_TYPE_DEFINE( id ) template < class T > const id_t type_pair< std::vector< T > >::ID = id;
#define MAP_TYPE_DEFINE( id ) template < class K, class V > const id_t type_pair< std::map< K, V > >::ID = id;
*/
		PRIMITIVE_TYPE_DEFINE( byte_t, ID_BYTE )
		PRIMITIVE_TYPE_DEFINE( word_t, ID_WORD )
		PRIMITIVE_TYPE_DEFINE( dword_t, ID_DWORD )
		PRIMITIVE_TYPE_DEFINE( qword_t, ID_QWORD )
		/*		STRUCT_TYPE_DEFINE( ID_STRUCT )
		VECTOR_TYPE_DEFINE( ID_VECTOR )
		MAP_TYPE_DEFINE( ID_MAP )
		*/
		PRIMITIVE_TYPE_DEFINE( std::string, ID_STRING )
		PRIMITIVE_TYPE_DEFINE( g2::Buffer, ID_G2_BUFFER )
#undef PRIMITIVE_TYPE_DEFINE
/*
#undef STRUCT_TYPE_DEFINE
#undef VECTOR_TYPE_DEFINE
#undef MAP_TYPE_DEFINE
*/
		
		//-----------------------------------------------------------------------------------------//
		TypeMismatchException::TypeMismatchException( string message, int number, int code )
			:Exception( message, number, code )
			{
			}

		//-----------------------------------------------------------------------------------------//
		TypeMismatchException::TypeMismatchException( const TypeMismatchException &rhs )
			:Exception( rhs )
			{
			}

		//-----------------------------------------------------------------------------------------//
		TypeMismatchException& TypeMismatchException::operator = ( const TypeMismatchException &rhs )
			{
			Exception::operator = ( rhs );
			return *this;
			}

		//-----------------------------------------------------------------------------------------//
		TypeMismatchException::~TypeMismatchException()
			{
			}

		//-----------------------------------------------------------------------------------------//
		DebinarizeException::DebinarizeException( string message, int number, int code )
			:Exception( message, number, code )
			{
			}

		//-----------------------------------------------------------------------------------------//
		DebinarizeException::DebinarizeException( const DebinarizeException &rhs )
			:Exception( rhs )
			{
			}

		//-----------------------------------------------------------------------------------------//
		DebinarizeException& DebinarizeException::operator = ( const DebinarizeException &rhs )
			{
			Exception::operator = ( rhs );
			return *this;
			}

		//-----------------------------------------------------------------------------------------//
		DebinarizeException::~DebinarizeException()
			{
			}		
		}
	}
