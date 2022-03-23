#pragma once
#include <vector>

namespace crystal
{

    template<size_t AlignBits>
    class IMemPool
    {
    public:
        using Word = unsigned short;

        static constexpr size_t    TotalSizeInWords = 1 << 15;

        static constexpr size_t    AlignMask = (1 << AlignBits) - 1;
        static constexpr size_t    Align = 1 << AlignBits;

        static constexpr size_t    WordSizeToBytesBits = 1;


        virtual ~IMemPool() = 0 {}

        virtual void* Malloc(size_t size) = 0;
        virtual void  Free(void* ptr) = 0;

        // Size for each block entry is the size of entire block

        inline size_t WordsToBytes(size_t size)
        {
            return size << WordSizeToBytesBits;
        }

        inline size_t BytesToWords(size_t size)
        {
            return (size + (1 << WordSizeToBytesBits) - 1) >> WordSizeToBytesBits;
        }

        inline size_t AlignDown(size_t size)
        {
            return size & (~AlignMask);
        }

        inline size_t AlignUp(size_t size)
        {
            return ((size + Align - 1) >> AlignBits) << AlignBits;
        }

    protected:

        Word    Heap[TotalSizeInWords];
    };


    class ImplicitListPool : public IMemPool<1>
    {
    public:
        struct Header
        {
            union
            {
                struct
                {
                    Word    is_alloc    : 1;
                    Word    w_size      : 15;
                };
                Word b_size;
            };
        };
        static constexpr size_t    MinimumSizeInWords = 2;

        static inline Header* nextHeader(Header* p) { return p + p->w_size; }
        static inline Header* getEnd(Header* p) { return p + p->w_size - 1; }

        ImplicitListPool()
        {
            // Block 1 space = TotalSize - 1 words
            auto start = reinterpret_cast<Header*>(&Heap[0]);
            AddBlock(start, AlignDown(TotalSizeInWords - 1));
            start->is_alloc = 0;

            // Special [size 0 / allocated] block
            auto last = nextHeader(start);
            last->w_size = 0;
            last->is_alloc = 1;
        }
        virtual ~ImplicitListPool() override {}

        void* Malloc(size_t bSize) override
        {
            size_t wSize = BytesToWords(bSize) + 2;
            wSize = AlignUp(wSize);

            Header* blockHeader = nullptr;
            if (!(blockHeader = FindFirst(wSize)))
            {
                return nullptr;
            }
            AddBlock(blockHeader, wSize);
            return blockHeader + 1;
        }

        void Free(void* ptr) override
        {
            RemoveBlock(reinterpret_cast<Header*>(ptr) - 1);
        }

    private:

        Header* FindFirst(size_t wSize)
        {
            Header* p = reinterpret_cast<Header*>(&Heap[0]);
            while (p->w_size)
            {
                if (!p->is_alloc && p->w_size >= wSize)
                {
                    return p;
                }
                p = nextHeader(p);
            }
            return nullptr;
        }

        void FillBlock(Header* p, size_t wSize, bool alloc)
        {
            p->w_size = wSize;
            p->is_alloc = alloc;
            *(p + p->w_size - 1) = *p;
        }

        void AddBlock(Header* p, size_t wSize)
        {
            assert(p);
            assert(wSize >= MinimumSizeInWords);
            assert(reinterpret_cast<Word*>(p) >= &Heap[0]
                && reinterpret_cast<Word*>(p) <= &Heap[TotalSizeInWords - 1]);

            Header* header = reinterpret_cast<Header*>(p);

            assert(!header->is_alloc);
            //assert(header->w_size >= wSize);

            // If free block size greater than alloc size
            size_t origSize = header->w_size;
            if (origSize > wSize)
            {
                FillBlock(header + wSize, origSize - wSize, false);
            }
            FillBlock(header, wSize, true);
        }

        void RemoveBlock(Header* p)
        {
            assert(p);
            assert(reinterpret_cast<Word*>(p) >= &Heap[0]
                && reinterpret_cast<Word*>(p) <= &Heap[TotalSizeInWords - 1]);
            assert(p->is_alloc);
            assert(p->w_size >= 0);

            Header* start = p;
            Header* end = getEnd(start);
            Header* nextP = end + 1;
            if (!nextP->is_alloc)
            {
                start->w_size += nextP->w_size;
                end = getEnd(start);
            }

            Header* lastP = start - 1;
            if (lastP >= reinterpret_cast<Header*>(&Heap[0]) && !lastP->is_alloc)
            {
                start = lastP - lastP->w_size + 1;
                start->w_size += p->w_size;
            }

            start->is_alloc = 0;
            *end = *start;
        }
    };


    class FreeListPool : public IMemPool<2>
    {
    public:

        struct Tag
        {
            union
            {
                struct
                {
                    Word    is_alloc    : 1;
                    Word    w_size      : 15;
                };
                Word b_size;
            };
        };

        struct Header
        {
            Tag      tag;
            short    i_prev;
            short    i_next;
        };

        struct Footer
        {
            Tag     tag;
        };

        Header* head;

        static constexpr size_t    MinimumSizeInWords = 4;

        static inline Header* nextHeader(Header* p) {
            return reinterpret_cast<Header*>(
                reinterpret_cast<Word*>(p) + p->tag.w_size);
        }
        static inline Footer* getFooter(Header* p) { return reinterpret_cast<Footer*>(nextHeader(p)) - 1; }

        short getId(Header* p) const { return reinterpret_cast<Word*>(p) - &Heap[0]; }
        Header* getHeader(short id) { return reinterpret_cast<Header*>(&Heap[id]); }

        inline Header* nextFree(Header* p) { return getHeader(p->i_next); }


        FreeListPool()
        {
            auto start = reinterpret_cast<Header*>(&Heap[0]);

            start->tag.w_size = AlignDown(TotalSizeInWords - 1);
            start->tag.is_alloc = 0;
            start->i_next = -1;
            start->i_prev = -1;

            head = start;

            auto last = nextHeader(head);
            last->tag.w_size = 0;
            last->tag.is_alloc = 1;
        }
        virtual ~FreeListPool() override {}

        void* Malloc(size_t bSize) override
        {
            size_t wSize = BytesToWords(bSize) + MinimumSizeInWords;
            wSize = AlignUp(wSize);

            Header* blockHeader = nullptr;
            if (!(blockHeader = FindFirst(wSize)))
            {
                return nullptr;
            }
            AddBlock(blockHeader, wSize);
            // Allocated blocks do not have linked list pointer
            return &blockHeader->i_prev; 
        }

        void Free(void* ptr) override
        {
            RemoveBlock(reinterpret_cast<Tag*>(ptr) - 1);
        }

    private:
        void Erase(Header* p)
        {
            if (p->i_prev >= 0)
            {
                auto last = reinterpret_cast<Header*>(&Heap[p->i_prev]);
                last->i_next = p->i_next;
            }

            if (p->i_next >= 0)
            {
                auto next = reinterpret_cast<Header*>(&Heap[p->i_next]);
                next->i_prev = p->i_prev;
            }
        }

        void Move(Header* p, Header* q)
        {
            short qId = getId(q);
            if (p->i_prev >= 0)
            {
                auto last = reinterpret_cast<Header*>(&Heap[p->i_prev]);
                last->i_next = qId;
            }

            if (p->i_next >= 0)
            {
                auto next = reinterpret_cast<Header*>(&Heap[p->i_next]);
                next->i_prev = qId;
            }

            if (p == head)
            {
                head = q;
            }
        }

        Header* FindFirst(size_t wSize)
        {
            Header* p = head;
            while (true)
            {
                if (!p->tag.is_alloc && p->tag.w_size >= wSize)
                {
                    return p;
                }
                if (p->i_next < 0) break;
                p = nextFree(p);
            }
            return nullptr;
        }

        void AddBlock(Header* p, size_t wSize)
        {
            assert(p);
            assert(wSize >= MinimumSizeInWords);
            assert(reinterpret_cast<Word*>(p) >= &Heap[0]
                && reinterpret_cast<Word*>(p) <= &Heap[TotalSizeInWords - 1]);

            Header* header = reinterpret_cast<Header*>(p);

            assert(!header->tag.is_alloc);
            //assert(header->w_size >= wSize);

            // If free block size greater than alloc size
            if (p->tag.w_size > wSize)
            {
                Header* splitHeader = reinterpret_cast<Header*>(reinterpret_cast<Word*>(p) + wSize);
                splitHeader->tag.w_size = p->tag.w_size - wSize;
                splitHeader->tag.is_alloc = 0;
                splitHeader->i_prev = p->i_prev;
                splitHeader->i_next = p->i_next;

                auto footer = getFooter(splitHeader);
                footer->tag = splitHeader->tag;

                p->tag.w_size = wSize;

                Move(p, splitHeader);
            }
            else
            {
                Erase(p);
            }
            p->tag.is_alloc = 1;
            auto footer = getFooter(p);
            footer->tag = p->tag;
        }

        void RemoveBlock(Tag* p)
        {
            assert(p);
            assert(reinterpret_cast<Word*>(p) >= &Heap[0]
                && reinterpret_cast<Word*>(p) <= &Heap[TotalSizeInWords - 1]);
            assert(p->is_alloc);
            assert(p->w_size >= 0);

            Header* header = reinterpret_cast<Header*>(p);
            Footer* footer = getFooter(header);
            Header* nextP = reinterpret_cast<Header*>((Word*)footer + 1);
            if (!nextP->tag.is_alloc)
            {
                header->tag.w_size += nextP->tag.w_size;
                Erase(nextP);

                footer = getFooter(header);
            }

            Footer* lastFoot = reinterpret_cast<Footer*>(header) - 1;
            if (lastFoot >= reinterpret_cast<Footer*>(&Heap[0]) && !lastFoot->tag.is_alloc)
            {
                header = reinterpret_cast<Header*>(
                    reinterpret_cast<Word*>(lastFoot) - lastFoot->tag.w_size + 1);
                Erase(header);

                header->tag.w_size += p->w_size;
            }

            header->tag.is_alloc = 0;
            footer->tag = header->tag;
            header->i_prev = -1;
            header->i_next = getId(head);
            head = header;
        }
    };
}