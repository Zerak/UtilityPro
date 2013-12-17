

// -*- C++ -*-
//==========================================================
/**
 *     Created_datetime : 12/16/2013 9:44
 *     File_base        : GNUMemoryPool
 *     File_ext         : hpp
 *     Author           : GNUnix <Kingbug2010@gmail.com>
 *     Description      : 
 *
 *     <Change_list>
 */
//==========================================================
#ifndef _MemoryPoolTemple_hpp_
#define _MemoryPoolTemple_hpp_

#include "ScopedLock.hpp"
#include "MemoryPool.hpp"

template<class T >
class GNUMemoryPool : ScopedLock
{
public:
	GNUMemoryPool():
		m_nPoolSize(0),
		m_nBlockSize(0),
		m_pPool(nullptr)
	{
	}

	~GNUMemoryPool()
	{
		if ( m_pPool != nullptr )
		{
			delete m_pPool;
			m_pPool = nullptr;
		}
	}

	void init( int poolSize )
	{
		m_nPoolSize = poolSize;
		m_nBlockSize = sizeof(T) + sizeof( MemoryBlock* );
		m_pPool = new MemoryPool( m_nBlockSize, m_nPoolSize );
	}

	T * get()
	{
		lock();
		MemoryBlock *pBlock = m_pPool->getBlock();
		unlock();

		if ( pBlock == nullptr )
		{
			return nullptr;
		}

		MemoryBlock **ppBlock = (MemoryBlock**) (pBlock->getData());
		*ppBlock = pBlock;

		ppBlock ++;
		return (T *) (ppBlock);
	}

	void set( T *t )
	{
		if ( t == nullptr )
		{
			return;
		}

		MemoryBlock **ppBlock = (MemoryBlock **) t;
		ppBlock --;

		MemoryBlock *pBlock = *ppBlock;

		lock();
		m_pPool->setBlock( pBlock );
		unlock();
	}

private:
	int	m_nPoolSize;
	int	m_nBlockSize;
	MemoryPool	*m_pPool;
};

#endif