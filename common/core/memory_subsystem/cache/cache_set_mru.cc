#include "cache_set.h"
#include "log.h"

// MRU: Most Recently Used

CacheSetMRU::CacheSetMRU(
      CacheBase::cache_t cache_type,
      UInt32 associativity, UInt32 blocksize) :
   CacheSet(cache_type, associativity, blocksize)
{
   m_lru_bits = new UInt8[m_associativity];
   for (UInt32 i = 0; i < m_associativity; i++)
      m_lru_bits[i] = i;
}

CacheSetMRU::~CacheSetMRU()
{
   delete [] m_lru_bits;
}

UInt32
CacheSetMRU::getReplacementIndex()
{
   // Invalidations may mess up the LRU bits

   for (UInt32 i = 0; i < m_associativity; i++) {
       if (!m_cache_block_info_array[i]->isValid())
          return i;
   }

   for (UInt32 i = 0; i < m_associativity; i++) {
       if (m_lru_bits[i] == 0)
          return i;
   }

   LOG_PRINT_ERROR("Error Finding LRU bits");
}

void
CacheSetMRU::updateReplacementIndex(UInt32 accessed_index)
{
   for (UInt32 i = 0; i < m_associativity; i++)
   {
      if (m_lru_bits[i] < m_lru_bits[accessed_index])
         m_lru_bits[i] ++;
   }
   m_lru_bits[accessed_index] = 0;
}