#include "networkcontainer.h"
#include "serializationtype.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	OutputNetworkContainer::OutputNetworkContainer()
		:opts_( st::network_container_options::NONE ),
		 buf_( 0x100 ),
		 ser_( buf_ )
		{
		Initialize();
		}

	//-----------------------------------------------------------------------------------------//
	OutputNetworkContainer::~OutputNetworkContainer()
		{
		}

	//-----------------------------------------------------------------------------------------//
	const g2::Buffer& OutputNetworkContainer::GetBuffer() const
		{
		return buf_;
		}

	//-----------------------------------------------------------------------------------------//
	void OutputNetworkContainer::Initialize()
		{
		st::byte_t pad[ st::NET_CONT_HEADER_SIZE ];
		memset( pad, 0, sizeof( pad ) );
		buf_.Write( pad, sizeof( pad ) );
		}

	//-----------------------------------------------------------------------------------------//
	void OutputNetworkContainer::SetOption( st::nc_opt_t opts )
		{
		opts_ = opts;
		}

	//-----------------------------------------------------------------------------------------//
	void OutputNetworkContainer::Clear()
		{
		buf_.Clear();
		Initialize();
		}
	
	//-----------------------------------------------------------------------------------------//
	void OutputNetworkContainer::BuildUp()
		{
		assert( buf_.GetSize() >= st::NET_CONT_HEADER_SIZE );
		
		st::byte_t *begin = (st::byte_t*)buf_.GetBegin();
		*(st::id_t*)begin = st::ID_NET_CONT;
		begin += sizeof( st::id_t );
		*(st::size_t*)begin = (st::size_t)( buf_.GetSize() - st::NET_CONT_HEADER_SIZE );
		begin += sizeof( st::size_t );
		*(st::nc_opt_t*)begin = opts_;

		namespace nc_opts = st::network_container_options;
		if( opts_ & nc_opts::CHECK_SUM )
			{
			unsigned long crc = g2::Utility::GetCRC32( (unsigned char*)buf_.GetBegin(), buf_.GetSize() );
			Binarize( (st::dword_t)crc );
			}
		}
	
	//-----------------------------------------------------------------------------------------//
	void OutputNetworkContainer::Binarize( std::string &value )
		{
		ser_.Write( value );
		}
	
	//-----------------------------------------------------------------------------------------//
	void OutputNetworkContainer::Binarize( g2::Buffer &value )
		{
		ser_.Write( value );
		}	

	//-----------------------------------------------------------------------------------------//
	void OutputNetworkContainer::StartScopedOperation( st::nc_opt_t opts )
		{
		SetOption( opts );
		}

	//-----------------------------------------------------------------------------------------//
	void OutputNetworkContainer::EndScopedOperation()
		{
		BuildUp();
		}

	//-----------------------------------------------------------------------------------------//
	InputNetworkContainer::InputNetworkContainer( g2::Buffer &buf )
		:opts_( st::network_container_options::NONE ),
		 deser_( buf ),
		 contSize_( 0 ),
		 initialSize_( 0 )
		{
		}

	//-----------------------------------------------------------------------------------------//
	InputNetworkContainer::InputNetworkContainer()
		:deser_(),
		 contSize_( 0 ),
		 initialSize_( 0 )
		{
		}

	//-----------------------------------------------------------------------------------------//
	InputNetworkContainer::~InputNetworkContainer()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void InputNetworkContainer::SetBuffer( g2::Buffer &buf )
		{
		deser_.SetBuffer( buf );
		}

	//-----------------------------------------------------------------------------------------//
	g2::Buffer& InputNetworkContainer::GetBuffer()
		{
		return deser_.GetBuffer();
		}

	//-----------------------------------------------------------------------------------------//
	void InputNetworkContainer::Debinarize( std::string &value )
		{
		if( deser_.Read( value ) == false )
			{
			throw st::DebinarizeException( "Debinarize() failed" );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void InputNetworkContainer::Debinarize( g2::Buffer &value )
		{
		if( deser_.Read( value ) == false )
			{
			throw st::DebinarizeException( "Debinarize() failed" );
			}
		}

	//-----------------------------------------------------------------------------------------//
	bool InputNetworkContainer::Validate()
		{
		g2::Buffer &tmp = deser_.GetBuffer();
		if( tmp.GetSize() < st::NET_CONT_HEADER_SIZE )
			{
			return false;
			}

		st::byte_t *begin = (st::byte_t*)tmp.GetBegin();
		if( *(st::id_t*)begin != st::ID_NET_CONT )
			{
			return false;
			}
		begin += sizeof( st::id_t );
		
		contSize_ = *(st::dword_t*)begin;
		if( ( st::NET_CONT_HEADER_SIZE + contSize_ ) > tmp.GetSize() )
			{
			contSize_ = 0;
			
			return false;
			}
		begin += sizeof( st::dword_t );

		namespace nc_opts = st::network_container_options;
		opts_ = *(st::nc_opt_t*)begin;
		
		if( opts_ & nc_opts::CHECK_SUM )
			{
			st::size_t requireSize =
				st::NET_CONT_HEADER_SIZE +
				contSize_ +
				st::NET_CONT_CHECK_SUM_SIZE;
			
			if( tmp.GetSize() < requireSize )
				{
				return false;
				}

			st::dword_t there = 0;
			Buffer checkSumBuf( tmp.GetBegin() + st::NET_CONT_HEADER_SIZE + contSize_,
								st::PRIMITIVE_HEADER_SIZE + sizeof( there ) );

			Deserialization tmpDeser( checkSumBuf );
			if( tmpDeser.Read( there ) == false )
				{
				return false;
				}
			
			st::dword_t here = (st::dword_t)g2::Utility::GetCRC32( (unsigned char*)tmp.GetBegin(), st::NET_CONT_HEADER_SIZE + contSize_ );
			if( here != there )
				{
				throw st::DebinarizeException( "Check sum is incorrect" );
				}
			}
		
		tmp.AddReadCompletionSize( st::NET_CONT_HEADER_SIZE );
		initialSize_ = tmp.GetSize();

		return true;
		}

	//-----------------------------------------------------------------------------------------//
	st::size_t InputNetworkContainer::GetSize() const
		{
		assert( initialSize_ > 0 );
		return contSize_;
		}

	//-----------------------------------------------------------------------------------------//
	void InputNetworkContainer::CleanUp()
		{
		assert( contSize_ != 0 && initialSize_ != 0 );
		
		g2::Buffer &tmp = deser_.GetBuffer();
		if( tmp.GetSize() > initialSize_ )
			{
			return;
			}
		
		::size_t consumedSize = initialSize_ - tmp.GetSize();
		if( contSize_ > consumedSize )
			{
			tmp.AddReadCompletionSize( contSize_ - consumedSize );
			}

		namespace nc_opts = st::network_container_options;
		if( opts_ & nc_opts::CHECK_SUM )
			{
			tmp.AddReadCompletionSize( st::NET_CONT_CHECK_SUM_SIZE );
			}
		}
	}
