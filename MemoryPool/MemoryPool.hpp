

// -*- C++ -*-
//==========================================================
/**
*     Created_datetime : 12/13/2013 16:53
*     File_base        : MemoryPool
*     File_ext         : hpp
*     Author           : GNUnix <Kingbug2010@gmail.com>
*     Description      : 
*
*     <Change_list>
*/
//==========================================================
#ifndef _MemPool_hpp_
#define _MemPool_hpp_

#include "ScopedLock.hpp"

class MemoryBlock
{
public:
	MemoryBlock( int size ):
		m_pNext(nullptr),
		m_pData( new char[size] ),
		m_nBlockSize(size)
	{
		memset( m_pData,0,size );
	}

	~MemoryBlock()
	{
		if ( m_pData != nullptr )
		{
			delete[] m_pData;
			m_pData = nullptr;
		}
	}

	char *getData()
	{
		return m_pData;
	}

public:
	MemoryBlock	*m_pNext;

private:
	char	*m_pData;
	int		m_nBlockSize;
};

class MemoryPool
{
public:
	MemoryPool( int oneBlockSize, int count );
	~MemoryPool(void);

	MemoryBlock *getBlock();
	void setBlock( MemoryBlock *pBlock );

private:
	MemoryBlock	*m_Head;
	int m_nOneBlockSize;
	int	m_nBlockCount;
};


#endif