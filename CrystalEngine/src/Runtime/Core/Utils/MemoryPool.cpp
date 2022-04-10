#include "MemoryPool.h"

namespace crystal
{
    ImplicitListPool::ImplicitListPool()
    {
        void* blockPtr = sbrk(BlockOverheadUnAlloced);

        // Create a block of initial word size
        word_t tag = create_word_with_alloc_tag(BlockOverheadUnAlloced, true, false);
        put_word(blockPtr, tag);

        void* footer = get_block_footer(blockPtr);
        copy_word(blockPtr, footer);

        void* nextBlock = get_next_adj_block(blockPtr);
        set_block_is_prev_alloc(nextBlock, false);
    }

    void* ImplicitListPool::Malloc(size_t bSize)
    {
        size_t allocSize = AlignUp(bSize) + BlockOverheadAlloced;
        void* block = NULL;
        if (!(block = find_first(allocSize)))
        {
            void* end = heap_hi();
            if (!block_is_prev_alloc(end))
            {
                size_t lastBlockSize = get_block_size(increase_words(end, -1));
                size_t newSize = allocSize - lastBlockSize;
                block = extend_heap(newSize);
            }
            else
            {
                block = extend_heap(allocSize);
            }
            if (!block)
            {
                return NULL;
            }
        }
        place_block(block, allocSize);
        return increase_words(block, 1);
    }

    void ImplicitListPool::Free(void* ptr)
    {
        free_block(increase_words(ptr, -1));
    }

    void* ImplicitListPool::Realloc(void* ptr, size_t bSize)
    {
        if (!bSize)
        {
            Free(ptr);
            return nullptr;
        }
        void* target = Malloc(bSize);
        size_t size = get_block_size(ptr) - WordSize * 2;
        if (bSize < size)
        {
            size = bSize;
        }
        memcpy(target, ptr, size);
        Free(ptr);
        return target;
    }

    void* ImplicitListPool::extend_heap(size_t bSize)
    {
        void* blockPtr = sbrk(bSize);

        // Create a block of initial word size
        word_t tag = create_word(bSize, false);
        put_word_keep_alloc_tag(blockPtr, tag);

        void* footer = get_block_footer(blockPtr);
        copy_word(blockPtr, footer);

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

            word_t tag = create_word_with_alloc_tag(get_block_size(header) + get_block_size(next), true, false);
            put_word(header, tag);
            footer = get_block_footer(next);
        }

        // Merge with previous block
        if (!block_is_prev_alloc(header))
        {
            assert(prev >= heap_low());
            assert(!block_is_alloc(prev));

            word_t tag = create_word_with_alloc_tag(get_block_size(header) + get_block_size(prev), true, false);
            header = get_block_header(prev);
            put_word(header, tag);
        }

        copy_word(header, footer);

        // Set next block's prev alloc to false
        void* nextBlock = get_next_adj_block(header);
        set_block_is_prev_alloc(nextBlock, false);
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
        return NULL;
    }

    void ImplicitListPool::place_block(void* blockPtr, size_t bSize)
    {
        assert(!block_is_alloc(blockPtr));
        assert(blockPtr >= heap_low() && blockPtr < heap_hi());
        size_t origSize = get_block_size(blockPtr);

        if (origSize - bSize >= BlockOverheadUnAlloced)
        {
            void* splitBlock = increase_bytes(blockPtr, bSize);
            put_word(splitBlock, create_word_with_alloc_tag(origSize - bSize, true, false));
            copy_word(splitBlock, get_block_footer(splitBlock));
        }
        else
        {
            bSize = origSize;
        }
        word_t tag = create_word_with_alloc_tag(bSize, true, true);
        put_word(blockPtr, tag);
        // Do not set footer for allocated blocks

        // Set prev alloc bit for next block
        void* nextBlock = get_next_adj_block(blockPtr);
        set_block_is_prev_alloc(nextBlock, true);
    }

    void ImplicitListPool::free_block(void* blockPtr)
    {
        assert(block_is_alloc(blockPtr));
        assert(blockPtr >= heap_low() && blockPtr < heap_hi());

        set_block_is_alloc(blockPtr, false);

        coalesce(blockPtr);
    }



    FreeListPool::FreeListPool()
    {
        void* head = sbrk(BlockOverhead + WordSize);
        put_word(head, WordSize);

        void* firstBlock = (byte_t*)head + WordSize;
        put_word(firstBlock, create_word(BlockOverhead, false));
        set_block_next_ptr(firstBlock, 0);
        set_block_prev_ptr(firstBlock, 0);
        copy_word(firstBlock, get_block_footer(firstBlock));
    }
}