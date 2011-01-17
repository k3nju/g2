#pragma once

#include <tr1/type_traits>
#include <boost/mpl/if.hpp>
#include <typeinfo>
#include "buffer.h"
#include "uncopyable.h"
#include "serialization.h"
#include "deserialization.h"
#include "scopedoperation.h"

namespace g2
	{
	class InputNetworkContainer;
	class OutputNetworkContainer;
	
	namespace detail
		{
		// dispatch tags
		struct tag_pod {};
		struct tag_sequence {};
		struct tag_map {};
		struct tag_push_back {};
		struct tag_insert {};
		struct tag_g2_serializable {};
		
		// typedefs
		typedef tag_pod& tag_pod_t;
		typedef tag_sequence& tag_sequence_t;
		typedef tag_map& tag_map_t;
		typedef tag_push_back& tag_push_back_t;
		typedef tag_insert& tag_insert_t;
		typedef tag_g2_serializable& tag_g2_serializable_t;
		typedef char true_t;
		typedef short false_t;

		// map trait
		template < class T >
		struct is_map
			{
			private:
				template < class C >
				static true_t Test( typename C::mapped_type* );
				template < class C >
				static false_t Test( ... );

			public:
				static const bool value = sizeof( Test< T >( NULL ) ) == sizeof( true_t );
			};

		// push_back trait
		// [*] currently not used
		/*
		  template < class T >
		  struct has_push_back
		  {
		  private:
		  template < class C >
		  static true_t Test( C*, void (C::*dummy)( const typename C::value_type& ) = &C::push_back );
		  template < class C >
		  static false_t Test( C*, void (C::*dummy)( const typename C::value_type& ) = &C::insert );

		  public:
		  static const bool value = sizeof( Test< T >( NULL ) ) == sizeof( true_t );
		  };
		*/

		// serialize trait
		template < class T,
				   void (T::*)( g2::OutputNetworkContainer& ),
				   bool (T::*)( g2::InputNetworkContainer& ) >
		struct is_g2_serializable_impl_helper
			{
				typedef void type;
			};
		
		template < class T, class U = void >
		struct is_g2_serializable_impl
			{
				static const bool value = false;
			};

		template < class T >
		struct is_g2_serializable_impl < T,
										 typename
										 is_g2_serializable_impl_helper< T,
																		 &T::Serialize,
																		 &T::Deserialize >::type >
			{
				static const bool value = true;
			};
		
		template < class T >
		struct is_g2_serializable : is_g2_serializable_impl< T >
			{
				/*
			private:
				template < class C >
				static true_t Test( typename C::gensin_serializable_t* );
				template < class C >
				static false_t Test( ... );

			public:
				static const bool value = sizeof( Test< T >( NULL ) ) == sizeof( true_t );
				*/
			};

		// container selecter
		template < class T >
		struct SelectContainer
			{
			public:
/*				typedef typename boost::mpl::if_c<
				// is T pod or not
				( ( tr1::is_pod< T >::value & tr1::is_object< T >::value ) == true ),
				tag_pod_t,
				typename boost::mpl::if_c<
					// does T have g2 ser/deser member functions or not
					( is_g2_serializable< T >::value == true ),
					tag_g2_serializable_t,
					typename boost::mpl::if_c<
						// is T map or sequence?
						( is_map< T >::value == true ),
						tag_map_t,
						tag_sequence_t >::type
					>::type
				>::type type;
*/

				typedef typename boost::mpl::if_c<
				( is_g2_serializable< T >::value == true ),
				tag_g2_serializable_t,
				typename boost::mpl::if_c<
					( tr1::is_pod< T >::value == true ),
					tag_pod_t,
					typename boost::mpl::if_c<
						( is_map< T >::value == true ),
						tag_map_t,
						tag_sequence_t >::type
					>::type
				>::type type;
			};
		}
	//-----------------------------------------------------------------------------------------//
	namespace st = g2::serialization_type;
	
// mark a class g2_serializable.
// member functions named "Serialize" and "Deserialize" in marked class will be called on serialization and deserialization.
// marked class must have these member functions
// void Serialize( g2::OutputNetworkContainer &onc );
// bool Deserialize( g2::InputNetworkContainer &inc );
// * 10/3/15 this mark is currently unused.
#define GENSIN__SERIALIZABLE typedef char gensin_serializable_t;

	class OutputNetworkContainer :private g2::Uncopyable
		{
			friend class g2::ScopedOperation< OutputNetworkContainer > ;
			
		public:
			OutputNetworkContainer();
			~OutputNetworkContainer();

			const g2::Buffer& GetBuffer() const;
			
			void SetOption( st::nc_opt_t options );
			void Clear();
			void BuildUp();

			template < class T >
			void Binarize( const T *value, st::word_t size );
			template < class T >
			void Binarize( const T &value );
			void Binarize( std::string &value );
			void Binarize( g2::Buffer &value );
			
		private:
			void Initialize();
			template < class T >
			void Load( const T &value, detail::tag_pod_t );
			template < class T >
			void Load( const T &value, detail::tag_g2_serializable_t );
			template < class T >
			void Load( const T &value, detail::tag_sequence_t );
			template < class T >
			void Load( const T &value, detail::tag_map_t );

			// for RAII (g2::ScopedOperation)
			void StartScopedOperation( st::nc_opt_t options );
			void EndScopedOperation();

			st::nc_opt_t opts_;
			g2::Buffer buf_;
			g2::Serialization ser_;
		};


	//-----------------------------------------------------------------------------------------//
	class InputNetworkContainer :private g2::Uncopyable
		{
		public:
			explicit InputNetworkContainer( g2::Buffer &buf );
			InputNetworkContainer();
			~InputNetworkContainer();

			void SetBuffer( g2::Buffer &buf );
			g2::Buffer& GetBuffer();

			bool Validate();
			void CleanUp();
			st::size_t GetSize() const;
			
			template < class T >
			void Debinarize( T *value, st::word_t size );
			template < class T >
			void Debinarize( T &value );
			void Debinarize( std::string &value );
			void Debinarize( g2::Buffer &value );
			
		private:
			template < class T >
			void Unload( T &value, detail::tag_pod_t );
			template < class T >
			void Unload( T &value, detail::tag_g2_serializable_t );
			template < class T >
			void Unload( T &value, detail::tag_sequence_t );
			template < class T >
			void Unload( T &value, detail::tag_map_t );

			st::nc_opt_t opts_;
			g2::Deserialization deser_;
			// container data size.
			// contSize_ represents container unit size.
			::size_t contSize_;
			// initial buffer size.
			// initialSize_ represents total buffer size when IsBinarizable() returned true.
			::size_t initialSize_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void OutputNetworkContainer::Binarize( const T *value, st::word_t size )
		{
		ser_.Write( value, size );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void OutputNetworkContainer::Binarize( const T &value )
		{
		Load( value, typename detail::SelectContainer< T >::type() );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void OutputNetworkContainer::Load( const T &value, detail::tag_pod_t )
		{
		ser_.Write( value );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void OutputNetworkContainer::Load( const T &value, detail::tag_g2_serializable_t )
		{
		const_cast< T& >( value ).Serialize( *this );
		}
	
	//-----------------------------------------------------------------------------------------//
	template < class T >
	void OutputNetworkContainer::Load( const T &value, detail::tag_sequence_t )
		{
		ser_.Write( st::ID_ARRAY );
		st::dword_t size = value.size();
		ser_.Write( size );

		for( typename T::const_iterator begin( value.begin() ), end( value.end() );
			 begin != end;
			 ++begin )
			{
			Load( *begin, typename detail::SelectContainer< typename T::value_type >::type() );
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void OutputNetworkContainer::Load( const T &value, detail::tag_map_t )
		{
		ser_.Write( st::ID_MAP );
		st::dword_t size = value.size();
		ser_.Write( size );
		
		for( typename T::const_iterator begin( value.begin() ), end( value.end() );
			 begin != end;
			 ++begin )
			{
			Load( begin->first, typename detail::SelectContainer< typename T::key_type >::type() );
			Load( begin->second, typename detail::SelectContainer< typename T::mapped_type >::type() );
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void InputNetworkContainer::Debinarize( T *value, st::word_t size )
		{
		if( deser_.Read( value, size ) == false )
			{
			// TODO: need more detail informations for failed
			throw st::DebinarizeException( "Debinarize failed" );
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void InputNetworkContainer::Debinarize( T &value )
		{
		Unload( value, typename detail::SelectContainer< T >::type() );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void InputNetworkContainer::Unload( T &value, detail::tag_pod_t )
		{
		if( deser_.Read( value ) == false )
			{
			// TODO: need more detail informations for failed
			throw st::DebinarizeException( "Debinarize failed" );
			}
		}
	
	//-----------------------------------------------------------------------------------------//
	template < class T >
	void InputNetworkContainer::Unload( T &value, detail::tag_g2_serializable_t )
		{
		if( value.Deserialize( *this ) == false )
			{
			// TODO: need more detail informations for failed
			throw st::DebinarizeException( "Debinarize failed" );
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void InputNetworkContainer::Unload( T &value, detail::tag_sequence_t )
		{
		st::id_t id = st::ID_UNKNOWN;
		st::dword_t size = 0;

		if( deser_.Read( id ) == false )
			{
			// TODO: need more detail informations for failed
			throw st::DebinarizeException( "Debinarize failed" );
			}
		
		if( id != st::ID_ARRAY )
			{
			throw st::TypeMismatchException( "ID is not ID_ARRAY" );
			}
		
		if( deser_.Read( size ) == false )
			{
			// TODO: need more detail informations for failed
			throw st::DebinarizeException( "Debinarize failed" );
			}
		
		for( st::word_t i = 0; i < size; ++i )
			{
			typename T::value_type tmp;
			Unload( tmp, typename detail::SelectContainer< typename T::value_type >::type() );
			value.insert( value.end(), tmp );
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void InputNetworkContainer::Unload( T &value, detail::tag_map_t )
		{
		st::id_t id = st::ID_UNKNOWN;
		st::dword_t size = 0;

		if( deser_.Read( id ) == false )
			{
			// TODO: need more detail informations for failed
			throw st::DebinarizeException( "Debinarize failed" );
			}
		
		if( id != st::ID_MAP )
			{
			throw st::TypeMismatchException( "ID is not ID_MAP" );
			}

		if( deser_.Read( size ) == false )
			{
			// TODO: need more detail informations for failed
			throw st::DebinarizeException( "Debinarize failed" );
			}
		
		for( st::word_t i = 0; i < size; ++i )
			{
			std::pair< typename T::key_type, typename T::mapped_type > elem;
			Unload( elem.first, typename detail::SelectContainer< typename T::key_type >::type() );
			Unload( elem.second, typename detail::SelectContainer< typename T::mapped_type >::type() );
			value.insert( value.end(), elem );
			}
		}
	}


#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__NetContTest

/*
<Makefile.am>
here=/home/kizi/devel/gensinx/g2/src
guts_SOURCES=\
$(here)/networkcontainer.h\
$(here)/networkcontainer.cpp
</Makefile.am>
*/

#include "buffer.h"
#include "debugutility.h"
#include <deque>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <string>
#include <tr1/unordered_map>
#include <cassert>
#include <typeinfo>
#include <iostream>
#include "serializationtype.h"


using namespace g2;
using namespace std;
using namespace serialization_type;

typedef deque<int> int_deq_t;
typedef list<int> int_list_t;
typedef set<int> int_set_t;
typedef vector<int> int_vec_t;
typedef map<int,int> ii_map_t;
typedef tr1::unordered_map<int,int> ii_umap_t;

struct POD
	{
		int i;
		char c;
	};

class G2Class
	{
	public:
		dword_t n;
		GENSIN__SERIALIZABLE;

		G2Class():n(0){};
		G2Class( dword_t i ):n(i){};
		
		void Serialize( g2::OutputNetworkContainer &onc )
			{
			onc.Binarize( n );
			}

		bool Deserialize( g2::InputNetworkContainer &inc )
			{
			n = 0;
			inc.Debinarize( n );

			return true;
			}

		bool operator == ( const G2Class &other )
			{
			return other.n == n;
			}
	};

template < class T >
void Name( const T& )
	{
	puts( typeid( T ).name() );
	}

void Dump( g2::Buffer &t )
	{
	g2::DebugUtil::Dump( t.GetBegin(), t.GetSize() );
	}

template < class T >
void Dump( T &t )
	{
	for( typename T::const_iterator b( t.begin() ), e( t.end() );
		 b != e;
		 ++b )
		{
		cout << *b << endl;
		}
	}

g2::Buffer OutTest()
	{
	g2::OutputNetworkContainer onc;

	{
	g2::ScopedOperation< g2::OutputNetworkContainer > scopedOp( onc, network_container_options::CHECK_SUM );
	
	byte_t b = 1;
	word_t w = 2;
	dword_t d = 3;
	qword_t q = 4;
	::POD p = { 5, 6 };
	G2Class g( 7 );
	int_deq_t ideq;
	ideq.push_back( 8 );

	onc.Binarize( b );
	onc.Binarize( w );
	onc.Binarize( d );
	onc.Binarize( q );
	onc.Binarize( p );
	onc.Binarize( g );
	onc.Binarize( ideq );
	}

	//g2::DebugUtil::Dump( onc.GetBuffer() );

	return onc.GetBuffer();
	}

void InTest( g2::Buffer &buf )
	{
	g2::InputNetworkContainer inc( buf );
	
	byte_t b = 0;
	word_t w = 0;
	dword_t d = 0;
	qword_t q = 0;
	::POD p = { 0, 0 };
	G2Class g( 0 );
	int_deq_t ideq;

	assert( inc.Validate() == true );
	inc.Debinarize( b );
	inc.Debinarize( w );
	inc.Debinarize( d );
	inc.Debinarize( q );
	inc.Debinarize( p );
	inc.Debinarize( g );
	//inc.Debinarize( ideq );

	assert( b == 1 &&
			w == 2 &&
			d == 3 &&
			q == 4 &&
			p.i == 5 &&
			p.c == 6 &&
			g.n == 7 );
		/*&&
		  ideq.front() == 8 );*/
	}

#define INIT_OBJECTS buf.Clear(); onc.Clear();
#define BUILDUP onc.BuildUp(); buf = onc.GetBuffer();

int GutsEntryNetContTest(int,char**)
	{
	{
	// trivial tests
	g2::Buffer tmp = OutTest();
	InTest( tmp );
	}
	
	InputNetworkContainer inc;
	OutputNetworkContainer onc;
	g2::Buffer buf;
	inc.SetBuffer( buf );


	// g2 object test
	if(true)
		{
		G2Class c;
		c.n = 0x11223344;
		onc.Binarize( c );
		BUILDUP;
		assert( inc.Validate() == true );
		c.n = 0;
		inc.Debinarize( c );
		assert( c.n == 0x11223344 );

		deque< G2Class > d;
		for( dword_t i = 0; i < 10; ++i )
			{
			G2Class g;
			g.n = i;
			d.push_back( g );
			}
		
		INIT_OBJECTS;
		onc.Binarize( d );
		BUILDUP;
		
		deque< G2Class > d2;
		assert( inc.Validate() == true );
		inc.Debinarize( d2 );

		assert( d == d2 );
		}

	// general test
	if( true )
		{
		INIT_OBJECTS;

		byte_t b = 0x01;
		onc.Binarize( b );
		BUILDUP;

		assert( inc.Validate() == true );
		b = 0;
		inc.Debinarize( b );
		assert( b == 0x01 );
		}
	if( true )
		{
		INIT_OBJECTS;

		word_t b = 0x1111;
		onc.Binarize( b );
		BUILDUP;

		assert( inc.Validate() == true );
		b = 0;
		inc.Debinarize( b );
		assert( b == 0x1111 );
		}
	if( true )
		{
		INIT_OBJECTS;

		dword_t b = 0x11223344;
		onc.Binarize( b );
		BUILDUP;

		assert( inc.Validate() == true );
		b = 0;
		inc.Debinarize( b );
		assert( b == 0x11223344 );
		}
	if( true )
		{
		INIT_OBJECTS;

		qword_t b = 0x1122334455667788LL;
		onc.Binarize( b );
		BUILDUP;

		assert( inc.Validate() == true );
		b = 0;
		inc.Debinarize( b );
		assert( b == 0x1122334455667788LL );
		}
	if( true )
		{
		INIT_OBJECTS;
		int_deq_t d;
		for(int i = 0; i < 10; ++i ) d.push_back( i );
		G2Class c;
		c.n = 0xffffffff;

		onc.Binarize( d );
		onc.Binarize( c );
		BUILDUP;

		assert( inc.Validate() == true );
		int_deq_t d2;
		G2Class c2;
		inc.Debinarize( d2 );
		inc.Debinarize( c2 );

		assert( d2 == d );
		assert( c2 == c );
		}


	// unsatisfied buffer test
	if( true )
		{
		INIT_OBJECTS;

		dword_t b = 0x11223344;
		onc.Binarize( b );
		BUILDUP;

		for( ::size_t i = 0; i < buf.GetSize(); ++i )
			{
			Buffer tmp;
			tmp.Write( buf.GetBegin(), i );
			inc.SetBuffer( tmp );
			assert( inc.Validate() == false );
			}


		inc.SetBuffer( buf );
		assert( inc.Validate() == true );
		}

	// clean up test
	if( true )
		{
		INIT_OBJECTS;

		byte_t a = 0x11;
		byte_t b = 0x22;
		onc.Binarize( a );
		onc.Binarize( b );
		BUILDUP;

		assert( inc.Validate() == true );
		inc.CleanUp();
		assert( buf.GetSize() == 0 );
		
		INIT_OBJECTS;

		a = 0x11;
		b = 0x22;
		onc.Binarize( a );
		onc.Binarize( b );
		BUILDUP;
		
		a = 0;
		assert( inc.Validate() == true );
		inc.Debinarize( a );
		assert( a == 0x11 );
		inc.CleanUp();
		assert( buf.GetSize() == 0 );

		{
		INIT_OBJECTS;
		int_deq_t d;
		for( int i = 0; i < 10; ++i ) d.push_back( i );
		G2Class c;
		c.n = 0xffffffff;
		
		onc.Binarize( d );
		onc.Binarize( c );
		BUILDUP;
		assert( inc.Validate() == true );
		int_deq_t dd;
		inc.Debinarize( d );
		for( int i = 0; i < 10; ++i )
			{
			assert( d.front() == i );
			d.pop_front();
			}
		inc.CleanUp();
		assert( buf.GetSize() == 0 );
		}

		{
		INIT_OBJECTS;
		int_deq_t d;
		for( int i = 0; i < 10; ++i ) d.push_back( i );
		G2Class c;
		c.n = 0xffffffff;

		onc.Binarize( c );
		onc.Binarize( d );
		BUILDUP;
		assert( inc.Validate() == true );
		G2Class cc;
		inc.Debinarize( cc );
		assert( cc.n == c.n );
		inc.CleanUp();
		assert( buf.GetSize() == 0 );
		}
		
		}


	if( true )
		{
		typedef std::deque< ii_map_t > a_t;
		a_t a;
		for( int i = 0; i < 10; ++i )
			{
			ii_map_t m;
			for( int n = 0; n < 10; ++n )
				{
				m.insert( std::make_pair( n, n ) );
				}
			a.push_back( m );
			}
		
		INIT_OBJECTS;
		onc.Binarize( a );
		BUILDUP;

		assert( inc.Validate() == true );
		a_t a2;
		inc.Debinarize( a2 );

		assert( a2 == a );
		}

	if( true )
		{
		typedef std::deque< ii_map_t > a_t;
		typedef std::map< std::string, a_t > b_t;
		b_t b;
		for( int k = 0; k < 10; ++k )
			{
			a_t a;
			for( int i = 0; i < 10; ++i )
				{
				ii_map_t m;
				for( int n = 0; n < 10; ++n )
					{
					m.insert( std::make_pair( n, n ) );
					}
				a.push_back( m );
				}

			char tmp[2] = { k + 0x41, 0x00 };
			b.insert( std::make_pair( tmp, k ) );
			}
		
		INIT_OBJECTS;
		onc.Binarize( b );
		BUILDUP;
		assert( inc.Validate() == true );

		b_t b2;
		inc.Debinarize( b2 );
		assert( b2 == b );
		}

	// big object test
	if( true )
		{
		typedef std::deque< ii_map_t > a_t;
		typedef std::map< std::string, a_t > b_t;
		INIT_OBJECTS;

		b_t barr[10];
		{
		g2::ScopedOperation<g2::OutputNetworkContainer> so( onc, network_container_options::NONE );

		
		for( int k = 0; k < 10; ++k )
			{
			for( int i = 0; i < 30; ++i )
				{
				a_t a;
				for( int n = 0; n < 50; ++n )
					{
					ii_map_t iimap;
					for( int j = 0; j < 70; ++j )
						{
						iimap.insert( std::make_pair( j, j ) );
						}
					a.push_back( iimap );
					}

				char tmp[3] = { k + 0x41, i + 0x41, 0x00 };
				barr[k].insert( std::make_pair( tmp, a ) );
				}
			onc.Binarize( barr[k] );
			}
		}
		
		buf = onc.GetBuffer();
		
		assert( inc.Validate() == true );
		b_t barr2[10];
		for( int i = 0; i < 10; ++i )
			{
			inc.Debinarize( barr2[i] );
			}

		for( int i = 0; i < 10; ++i )
			{
			assert( barr[i] == barr2[i] );
			}
		}

	// big object with checksum
	if( true )
		{
		typedef std::deque< ii_map_t > a_t;
		typedef std::map< std::string, a_t > b_t;
		INIT_OBJECTS;

		b_t barr[10];
		{
		g2::ScopedOperation<g2::OutputNetworkContainer> so( onc, network_container_options::CHECK_SUM );

		
		for( int k = 0; k < 10; ++k )
			{
			for( int i = 0; i < 30; ++i )
				{
				a_t a;
				for( int n = 0; n < 50; ++n )
					{
					ii_map_t iimap;
					for( int j = 0; j < 70; ++j )
						{
						iimap.insert( std::make_pair( j, j ) );
						}
					a.push_back( iimap );
					}

				char tmp[3] = { k + 0x41, i + 0x41, 0x00 };
				barr[k].insert( std::make_pair( tmp, a ) );
				}
			onc.Binarize( barr[k] );
			}
		}
		
		buf = onc.GetBuffer();
		
		assert( inc.Validate() == true );
		b_t barr2[10];
		for( int i = 0; i < 10; ++i )
			{
			inc.Debinarize( barr2[i] );
			}

		for( int i = 0; i < 10; ++i )
			{
			assert( barr[i] == barr2[i] );
			}
		}

	// big object with checksum
	if( true )
		{
		typedef std::deque< ii_map_t > a_t;
		typedef std::map< std::string, a_t > b_t;
		INIT_OBJECTS;

		b_t barr[10];
		{
		g2::ScopedOperation<g2::OutputNetworkContainer> so( onc, network_container_options::CHECK_SUM );

		
		for( int k = 0; k < 10; ++k )
			{
			for( int i = 0; i < 30; ++i )
				{
				a_t a;
				for( int n = 0; n < 50; ++n )
					{
					ii_map_t iimap;
					for( int j = 0; j < 70; ++j )
						{
						iimap.insert( std::make_pair( j, j ) );
						}
					a.push_back( iimap );
					}

				char tmp[3] = { k + 0x41, i + 0x41, 0x00 };
				barr[k].insert( std::make_pair( tmp, a ) );
				}
			onc.Binarize( barr[k] );
			}
		}
		
		buf = onc.GetBuffer();
		*(buf.GetBegin() + 89) = 0xff;

		try
			{
			inc.Validate();
			}
		catch( DebinarizeException &E )
			{
			E.Output();
			}
		}

	return 0;
	}

#endif
#endif


  
