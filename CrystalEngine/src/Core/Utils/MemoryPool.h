#pragma once
#include <vector>
#include <cassert>

namespace crystal
{

    template<size_t AlignBits>
    class IMemPool
    {
    public:
        using word_t = unsigned long long;
        using byte_t = uint8_t;
        using offset_t = long long;

        static constexpr size_t    TotalSizeInBytes = 1 << 16;
        static constexpr size_t    WordSize = sizeof(word_t);

        static constexpr size_t    AlignMask = (1 << AlignBits) - 1;
        static constexpr size_t    Alignment = 1 << AlignBits;


        virtual ~IMemPool() = 0 {}

        virtual void* Malloc(size_t size) = 0;
        virtual void  Free(void* ptr) = 0;
        virtual void* Realloc(void* ptr, size_t bSize) = 0;

    protected:
        // Size for each block entry is the size of entire block
        inline size_t AlignDown(size_t size) const
        {
            return (size >> AlignBits) << AlignBits;
        }
        inline size_t AlignUp(size_t size) const
        {
            return ((size + Alignment - 1) >> AlignBits) << AlignBits;
        }
        inline void* heap_low() const { return (void*)&m_heap[0]; }
        inline void* heap_hi() const { return (void*)&m_heap[m_brk]; }
        inline size_t heap_size() const { return m_brk; }


        inline void* increase_bytes(void* ptr, offset_t bSize) { return (byte_t*)ptr + bSize; }
        inline void* increase_words(void* ptr, offset_t wSize) { return (word_t*)ptr + wSize; }


        inline size_t word_to_byte(size_t wSize) const { return wSize * WordSize; }
        inline void put_word(void* ptr, word_t value) { *((word_t*)ptr) = value; }
        inline void put_word_keep_alloc_tag(void* ptr, word_t value)
        {
            word_t prev_mask = *((word_t*)ptr) & 2;
            *((word_t*)ptr) = value | prev_mask;
        }
        inline void copy_word(void* src, void* dest) { *((word_t*)dest) = *((word_t*)src); }


        inline word_t create_word_with_alloc_tag(word_t bSize, bool prevAlloc, bool isAlloc)
        {
            assert(!(bSize & AlignMask));
            return bSize | ((word_t)prevAlloc << 1) | (word_t)isAlloc;
        }
        inline word_t create_word(word_t bSize, bool isAlloc)
        {
            assert(!(bSize & AlignMask));
            return bSize | (word_t)isAlloc;
        }


        void* sbrk(size_t bSize)
        {
            void* prev = &m_heap[m_brk];
            m_brk += bSize;
            assert(m_brk <= TotalSizeInBytes);
            return prev;
        }
    private:

        char    m_heap[TotalSizeInBytes];
        size_t  m_brk = 0;
    };


    class ImplicitListPool : public IMemPool<3>
    {
    public:
        static const size_t     BlockOverheadAlloced = WordSize;
        static const size_t     BlockOverheadUnAlloced = WordSize * 2;

        //static inline Header* nextHeader(Header* p) { return p + p->w_size; }
        //static inline Header* getEnd(Header* p) { return p + p->w_size - 1; }

        ImplicitListPool();
        virtual ~ImplicitListPool() override {}

        void* Malloc(size_t bSize) override;
        void Free(void* ptr) override;
        void* Realloc(void* ptr, size_t bSize) override;
        //{
        //    RemoveBlock(reinterpret_cast<Header*>(ptr) - 1);
        //}

    private:
        void* extend_heap(size_t bSize);
        void* coalesce(void* blockPtr);
        void* find_first(size_t bSize);
        void place_block(void* blockPtr, size_t bSize);
        void free_block(void* blockPtr);

        void* find_next_fit(size_t bSize)
        {
            static void* lastBlock = NULL;

            if (lastBlock == NULL)
            {
                lastBlock = heap_low();
            }

            void* end = heap_hi();
            for (void* p = get_next_adj_block(lastBlock); ;)
            {
                if (p == end)
                {
                    p = heap_low();
                }
                size_t curSize = get_block_size(p);
                if (!block_is_alloc(p) && curSize >= bSize)
                {
                    lastBlock = p;
                    return p;
                }
                if (p == lastBlock) break;
                p = get_next_adj_block(p);
            }
            return NULL;
        }

        inline void* get_block_footer(void* ptr)
        {
            return increase_words(increase_bytes(ptr, get_block_size(ptr)), -1);
        }
        inline void* get_block_header(void* ptr)
        {
            return increase_words(increase_bytes(ptr, -get_block_size(ptr)), 1);
        }


        inline size_t get_block_size(void* ptr)
        {
            return (*(word_t*)ptr) & (~AlignMask);
        }
        inline void* get_next_adj_block(void* ptr)
        {
            return increase_bytes(ptr, get_block_size(ptr));
        }
        inline bool block_is_alloc(void* ptr)
        {
            return (*(word_t*)ptr) & 1;
        }
        inline bool block_is_prev_alloc(void* ptr)
        {
            return (*(word_t*)ptr) & 2;
        }

        inline void set_block_is_alloc(void* ptr, bool value)
        {
            if (value)
            {
                (*(word_t*)ptr) |= 1;
            }
            else
            {
                (*(word_t*)ptr) &= ~1;
            }
        }
        inline void set_block_is_prev_alloc(void* ptr, bool value)
        {
            if (value)
            {
                (*(word_t*)ptr) |= 2;
            }
            else
            {
                (*(word_t*)ptr) &= ~2;
            }
        }
        //Header* FindFirst(size_t wSize)
        //{
        //    Header* p = reinterpret_cast<Header*>(&Heap[0]);
        //    while (p->w_size)
        //    {
        //        if (!p->is_alloc && p->w_size >= wSize)
        //        {
        //            return p;
        //        }
        //        p = nextHeader(p);
        //    }
        //    return nullptr;
        //}

        //void FillBlock(Header* p, size_t wSize, bool alloc)
        //{
        //    p->w_size = wSize;
        //    p->is_alloc = alloc;
        //    *(p + p->w_size - 1) = *p;
        //}

        //void AddBlock(Header* p, size_t wSize)
        //{
        //    assert(p);
        //    assert(wSize >= MinimumSizeInWords);
        //    assert(reinterpret_cast<Word*>(p) >= &Heap[0]
        //        && reinterpret_cast<Word*>(p) <= &Heap[TotalSizeInWords - 1]);

        //    Header* header = reinterpret_cast<Header*>(p);

        //    assert(!header->is_alloc);
        //    //assert(header->w_size >= wSize);

        //    // If free block size greater than alloc size
        //    size_t origSize = header->w_size;
        //    if (origSize > wSize)
        //    {
        //        FillBlock(header + wSize, origSize - wSize, false);
        //    }
        //    FillBlock(header, wSize, true);
        //}

        //void RemoveBlock(Header* p)
        //{
        //    assert(p);
        //    assert(reinterpret_cast<Word*>(p) >= &Heap[0]
        //        && reinterpret_cast<Word*>(p) <= &Heap[TotalSizeInWords - 1]);
        //    assert(p->is_alloc);
        //    assert(p->w_size >= 0);

        //    Header* start = p;
        //    Header* end = getEnd(start);
        //    Header* nextP = end + 1;
        //    if (!nextP->is_alloc)
        //    {
        //        start->w_size += nextP->w_size;
        //        end = getEnd(start);
        //    }

        //    Header* lastP = start - 1;
        //    if (lastP >= reinterpret_cast<Header*>(&Heap[0]) && !lastP->is_alloc)
        //    {
        //        start = lastP - lastP->w_size + 1;
        //        start->w_size += p->w_size;
        //    }

        //    start->is_alloc = 0;
        //    *end = *start;
        //}
    };

    class FreeListPool : public IMemPool<3>
    {
    public:
        static constexpr size_t    BlockOverhead = 4 * WordSize;

        FreeListPool();
        virtual ~FreeListPool() override {}

    private:

        inline size_t get_block_size(void* ptr)
        {
            return (*(word_t*)ptr) & (~AlignMask);
        }
        inline void* get_next_adj_block(void* ptr)
        {
            return increase_bytes(ptr, get_block_size(ptr));
        }
        inline bool block_is_alloc(void* ptr)
        {
            return (*(word_t*)ptr) & 1;
        }
        inline bool block_is_prev_alloc(void* ptr)
        {
            return (*(word_t*)ptr) & 2;
        }
        inline void* get_block_footer(void* ptr)
        {
            return increase_words(increase_bytes(ptr, get_block_size(ptr)), -1);
        }
        inline void* get_block_header(void* ptr)
        {
            return increase_words(increase_bytes(ptr, -get_block_size(ptr)), 1);
        }

        inline size_t get_block_next_ptr(void* ptr)
        {
            return *((word_t*)increase_words(ptr, 1));
        }
        inline size_t get_block_prev_ptr(void* ptr)
        {
            return *((word_t*)increase_words(ptr, 2));
        }
        inline void* get_next_block(void* ptr)
        {
            return (byte_t*)heap_low() + get_block_next_ptr(ptr);
        }
        inline void* get_prev_block(void* ptr)
        {
            return (byte_t*)heap_low() + get_block_prev_ptr(ptr);
        }
        inline void set_block_prev_ptr(void* ptr, word_t value)
        {
            *((word_t*)increase_words(ptr, 2)) = value;
        }
        inline void set_block_next_ptr(void* ptr, word_t value)
        {
            *((word_t*)increase_words(ptr, 1)) = value;
        }

        void* free_list_start = nullptr;

        void* extend_heap(size_t bSize);
        void* coalesce(void* blockPtr);
        void* find_first(size_t bSize);
        void place_block(void* blockPtr, size_t bSize);
        void free_block(void* blockPtr);
    };

    //class FreeListPool : public IMemPool<2>
    //{
    //public:

    //    struct Tag
    //    {
    //        union
    //        {
    //            struct
    //            {
    //                Word    is_alloc    : 1;
    //                Word    w_size      : 15;
    //            };
    //            Word b_size;
    //        };
    //    };

    //    struct Header
    //    {
    //        Tag      tag;
    //        short    i_prev;
    //        short    i_next;
    //    };

    //    struct Footer
    //    {
    //        Tag     tag;
    //    };

    //    Header* head;

    //    static constexpr size_t    MinimumSizeInWords = 4;

    //    static inline Header* nextHeader(Header* p) {
    //        return reinterpret_cast<Header*>(
    //            reinterpret_cast<Word*>(p) + p->tag.w_size);
    //    }
    //    static inline Footer* getFooter(Header* p) { return reinterpret_cast<Footer*>(nextHeader(p)) - 1; }

    //    short getId(Header* p) const { return reinterpret_cast<Word*>(p) - &Heap[0]; }
    //    Header* getHeader(short id) { return reinterpret_cast<Header*>(&Heap[id]); }

    //    inline Header* nextFree(Header* p) { return getHeader(p->i_next); }


    //    FreeListPool()
    //    {
    //        auto start = reinterpret_cast<Header*>(&Heap[0]);

    //        start->tag.w_size = AlignDown(TotalSizeInWords - 1);
    //        start->tag.is_alloc = 0;
    //        start->i_next = -1;
    //        start->i_prev = -1;

    //        head = start;

    //        auto last = nextHeader(head);
    //        last->tag.w_size = 0;
    //        last->tag.is_alloc = 1;
    //    }
    //    virtual ~FreeListPool() override {}

    //    void* Malloc(size_t bSize) override
    //    {
    //        size_t wSize = BytesToWords(bSize) + MinimumSizeInWords;
    //        wSize = AlignUp(wSize);

    //        Header* blockHeader = nullptr;
    //        if (!(blockHeader = FindFirst(wSize)))
    //        {
    //            return nullptr;
    //        }
    //        AddBlock(blockHeader, wSize);
    //        // Allocated blocks do not have linked list pointer
    //        return &blockHeader->i_prev; 
    //    }

    //    void Free(void* ptr) override
    //    {
    //        RemoveBlock(reinterpret_cast<Tag*>(ptr) - 1);
    //    }

    //private:
    //    void Erase(Header* p)
    //    {
    //        if (p->i_prev >= 0)
    //        {
    //            auto last = reinterpret_cast<Header*>(&Heap[p->i_prev]);
    //            last->i_next = p->i_next;
    //        }

    //        if (p->i_next >= 0)
    //        {
    //            auto next = reinterpret_cast<Header*>(&Heap[p->i_next]);
    //            next->i_prev = p->i_prev;
    //        }
    //    }

    //    void Move(Header* p, Header* q)
    //    {
    //        short qId = getId(q);
    //        if (p->i_prev >= 0)
    //        {
    //            auto last = reinterpret_cast<Header*>(&Heap[p->i_prev]);
    //            last->i_next = qId;
    //        }

    //        if (p->i_next >= 0)
    //        {
    //            auto next = reinterpret_cast<Header*>(&Heap[p->i_next]);
    //            next->i_prev = qId;
    //        }

    //        if (p == head)
    //        {
    //            head = q;
    //        }
    //    }

    //    Header* FindFirst(size_t wSize)
    //    {
    //        Header* p = head;
    //        while (true)
    //        {
    //            if (!p->tag.is_alloc && p->tag.w_size >= wSize)
    //            {
    //                return p;
    //            }
    //            if (p->i_next < 0) break;
    //            p = nextFree(p);
    //        }
    //        return nullptr;
    //    }

    //    void AddBlock(Header* p, size_t wSize)
    //    {
    //        assert(p);
    //        assert(wSize >= MinimumSizeInWords);
    //        assert(reinterpret_cast<Word*>(p) >= &Heap[0]
    //            && reinterpret_cast<Word*>(p) <= &Heap[TotalSizeInWords - 1]);

    //        Header* header = reinterpret_cast<Header*>(p);

    //        assert(!header->tag.is_alloc);
    //        //assert(header->w_size >= wSize);

    //        // If free block size greater than alloc size
    //        if (p->tag.w_size > wSize)
    //        {
    //            Header* splitHeader = reinterpret_cast<Header*>(reinterpret_cast<Word*>(p) + wSize);
    //            splitHeader->tag.w_size = p->tag.w_size - wSize;
    //            splitHeader->tag.is_alloc = 0;
    //            splitHeader->i_prev = p->i_prev;
    //            splitHeader->i_next = p->i_next;

    //            auto footer = getFooter(splitHeader);
    //            footer->tag = splitHeader->tag;

    //            p->tag.w_size = wSize;

    //            Move(p, splitHeader);
    //        }
    //        else
    //        {
    //            Erase(p);
    //        }
    //        p->tag.is_alloc = 1;
    //        auto footer = getFooter(p);
    //        footer->tag = p->tag;
    //    }

    //    void RemoveBlock(Tag* p)
    //    {
    //        assert(p);
    //        assert(reinterpret_cast<Word*>(p) >= &Heap[0]
    //            && reinterpret_cast<Word*>(p) <= &Heap[TotalSizeInWords - 1]);
    //        assert(p->is_alloc);
    //        assert(p->w_size >= 0);

    //        Header* header = reinterpret_cast<Header*>(p);
    //        Footer* footer = getFooter(header);
    //        Header* nextP = reinterpret_cast<Header*>((Word*)footer + 1);
    //        if (!nextP->tag.is_alloc)
    //        {
    //            header->tag.w_size += nextP->tag.w_size;
    //            Erase(nextP);

    //            footer = getFooter(header);
    //        }

    //        Footer* lastFoot = reinterpret_cast<Footer*>(header) - 1;
    //        if (lastFoot >= reinterpret_cast<Footer*>(&Heap[0]) && !lastFoot->tag.is_alloc)
    //        {
    //            header = reinterpret_cast<Header*>(
    //                reinterpret_cast<Word*>(lastFoot) - lastFoot->tag.w_size + 1);
    //            Erase(header);

    //            header->tag.w_size += p->w_size;
    //        }

    //        header->tag.is_alloc = 0;
    //        footer->tag = header->tag;
    //        header->i_prev = -1;
    //        header->i_next = getId(head);
    //        head = header;
    //    }
    //};
}