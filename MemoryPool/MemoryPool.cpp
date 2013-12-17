#include "MemoryPool.hpp"
#include <assert.h>

MemoryPool::MemoryPool( int oneBlockSize, int count ):
	m_Head(nullptr),
	m_nOneBlockSize(oneBlockSize),
	m_nBlockCount(count)
{
	MemoryBlock **ppBlock = &m_Head;

	for ( int i = 0; i < m_nBlockCount; i ++ )
	{
		MemoryBlock *pBlock = new MemoryBlock(m_nOneBlockSize);
		assert( pBlock != nullptr ) ;

		*ppBlock = pBlock;
		ppBlock = &(pBlock->m_pNext);
	}
	*ppBlock = nullptr;
}

MemoryPool::~MemoryPool(void)
{
	while ( m_Head != nullptr )
	{
		MemoryBlock *pBlock = m_Head;
		m_Head = m_Head->m_pNext;
		delete pBlock;
		pBlock = nullptr;
	}
}

MemoryBlock * MemoryPool::getBlock()
{
	MemoryBlock *pRet = m_Head;
	m_Head = m_Head->m_pNext;
	return pRet;
}

void MemoryPool::setBlock( MemoryBlock *pBlock )
{
	MemoryBlock *pTemp = m_Head;
	m_Head = pBlock;
	m_Head->m_pNext = pTemp;
}