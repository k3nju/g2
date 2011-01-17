#include "binarydata.h"
#include <cassert>

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	BinaryData::BinaryData( g2::Buffer &buf ):buffer_( &buf )
		{
		}
	
	//-----------------------------------------------------------------------------------------//
	BinaryData::~BinaryData()
		{
		}
	
	//-----------------------------------------------------------------------------------------//
	void BinaryData::SetBuffer( g2::Buffer &buf )
		{
		buffer_ = &buf;
		}

	//-----------------------------------------------------------------------------------------//
	void BinaryData::Pack()
		{
		assert( buffer_ != NULL && "buffer_ must not be NULL" );
		
		st::char_t pad = 0x00;
		buffer_->Write( &pad, sizeof( pad ) );
		}

	//-----------------------------------------------------------------------------------------//
	bool BinaryData::Unpack()
		{
		assert( buffer_ != NULL && "buffer_ must not be NULL" );
		
		if( buffer_->GetSize() < sizeof( st::char_t ) )
			{
			return false;
			}
		
		buffer_->AddReadCompletionSize( sizeof( st::char_t ) );

		return true;
		}
	}







































