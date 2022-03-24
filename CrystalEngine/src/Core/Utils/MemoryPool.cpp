#include "MemoryPool.h"

namespace crystal
{
    ImplicitListPool::ImplicitListPool()
    {
        extend_heap(64);
    }

    void* ImplicitListPool::Malloc(size_t bSize)
    {
        size_t allocSize = AlignUp(bSize) + 2 * WordSize;
        void* block = nullptr;
        if (!(block = find_first(allocSize)))
        {
            block = extend_heap(allocSize);
            if (!block)
            {
                return nullptr;
            }
        }
        place_block(block, allocSize);
        return increase_words(block, 1);
    }

    void ImplicitListPool::Free(void* ptr)
    {
        free_block(increase_words(ptr, -1));
    }

    void* ImplicitListPool::extend_heap(size_t bSize)
    {
        if (heap_size() + bSize > TotalSizeInBytes)
        {
            return nullptr;
        }
        void* blockPtr = sbrk(bSize);

        // Create a block of initial word size
        word_t tag = create_word(bSize, false);
        put_word(blockPtr, tag);

        void* footer = get_block_footer(blockPtr);
        put_word(footer, tag);

        return coalesce(blockPtr);
    }

    void* ImplicitListPool::coalesce(void* blockPtr)
    {
        assert(!block_is_alloc(blockPtr));

        void* next = get_next_adj_block(blockPtr);
        void* prev = increase_words(blockPtr, -1);

        void* header = blockPtr;
        void* footer = get_block_footer(blockPtr);

        // Merge with next block
        if (next != heap_hi() && !block_is_alloc(next))
        {
            assert(next < heap_hi());

            word_t tag = create_word(get_block_size(blockPtr) + get_block_size(next), false);
            footer = get_block_footer(next);
            put_word(blockPtr, tag);
        }

        // Merge with previous block
        if (blockPtr != heap_low() && !block_is_alloc(prev))
        {
            assert(prev >= heap_low());

            word_t tag = create_word(get_block_size(blockPtr) + get_block_size(prev), false);
            header = get_block_header(prev);
            put_word(header, tag);
        }

        copy_word(header, footer);
        return header;
    }

    void* ImplicitListPool::find_first(size_t bSize)
    {
        void* end = heap_hi();
        for (void* p = heap_low(); p != end; p = get_next_adj_block(p))
        {
            if (!block_is_alloc(p) && get_block_size(p) >= bSize)
            {
                return p;
            }
        }
        return nullptr;
    }

    void ImplicitListPool::place_block(void* blockPtr, size_t bSize)
    {
        assert(!block_is_alloc(blockPtr));
        assert(blockPtr >= heap_low() && blockPtr < heap_hi());
        size_t origSize = get_block_size(blockPtr);

        if (origSize - bSize >= WordSize * 2)
        {
            void* splitBlock = increase_bytes(blockPtr, bSize);
            put_word(splitBlock, create_word(origSize - bSize, false));
            copy_word(splitBlock, get_block_footer(splitBlock));
        }
        else
        {
            bSize = origSize;
        }
        put_word(blockPtr, create_word(bSize, true));
        copy_word(blockPtr, get_block_footer(blockPtr));
    }

    void ImplicitListPool::free_block(void* blockPtr)
    {
        assert(block_is_alloc(blockPtr));
        assert(blockPtr >= heap_low() && blockPtr < heap_hi());

        // Set to un-alloc
        word_t* tag = (word_t*)blockPtr;
        *tag = (*tag & (~AlignMask));

        coalesce(blockPtr);
    }
}