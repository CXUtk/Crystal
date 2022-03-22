#pragma once
#include <vector>

namespace crystal
{

    class IMemAlloc
    {
    public:
        virtual ~IMemAlloc() = 0 {}

        virtual void* Malloc(size_t size) = 0;
        virtual void  Free(void* ptr) = 0;

    protected:
        struct Word
        {
            union
            {
                struct
                {
                    // High 15 bits
                    unsigned short wSize : 15;
                    // Low 1 bit
                    unsigned short isAlloc : 1;
                };
                // Total value
                unsigned short bSize;
            };
        };
        using WPtr = Word*;

        static constexpr size_t    TotalSizeInWords = 1 << 12;
        static constexpr size_t    AlignBits = 1;
        static constexpr size_t    AlignMask = (1 << AlignBits) - 1;
        static constexpr size_t    Align = sizeof(Word);
        Word                       Heap[TotalSizeInWords];

        // Size for each block entry is the size of entire block

        inline size_t WordsToBytes(size_t size)
        {
            return size << AlignBits;
        }

        inline size_t BytesToWords(size_t size)
        {
            return size >> AlignBits;
        }

        inline size_t AlignDown(size_t size)
        {
            return size & (~AlignMask);
        }

        inline size_t AlignUp(size_t size)
        {
            return (size + Align - 1) >> AlignBits;
        }
    };


    class ImplicitListAlloc : public IMemAlloc
    {
    public:
        ImplicitListAlloc()
        {
            // Block 1 space = TotalSize - 1 words
            Heap[0].wSize = TotalSizeInWords - 1;
            Heap[0].isAlloc = 0;

            // Special [size 0 / allocated] block
            auto& last = Heap[TotalSizeInWords - 1];
            last.wSize = 0;
            last.isAlloc = 1;
        }
        virtual ~ImplicitListAlloc() override {}

        void* Malloc(size_t bSize) override
        {
            size_t wSize = BytesToWords(AlignUp(bSize)) + 2;
            WPtr result = nullptr;
            if (!(result = FindFirst(wSize)))
            {
                return nullptr;
            }
            AddBlock(result, wSize);
            return result + 1;
        }

        void Free(void* ptr) override
        {
            RemoveBlock((WPtr)ptr);
        }

    private:

        WPtr FindFirst(size_t wSize)
        {
            WPtr p = &Heap[0];
            while (p->wSize)
            {
                if (!p->isAlloc && p->wSize >= wSize)
                {
                    return p;
                }
                p = p + p->wSize;
            }
            return nullptr;
        }

        void AddBlock(WPtr p, size_t wSize)
        {
            assert(p);
            assert(p >= &Heap[0] && p <= &Heap[TotalSizeInWords - 1]);
            assert(!p->isAlloc);
            assert(!p->wSize >= wSize);
            // If free block size greater than alloc size
            if (p->wSize > wSize)
            {
                size_t wSplitSize = p->wSize - wSize;
                (p + wSize)->wSize = wSplitSize;
                (p + wSize)->isAlloc = 0;

                p->wSize = wSize;
                p->isAlloc = 1;
            }
            else
            {
                p->isAlloc = 1;
            }
            *(p + wSize - 1) = *p;
        }

        void RemoveBlock(WPtr p)
        {
            assert(p);
            assert(p >= &Heap[0] && p <= &Heap[TotalSizeInWords - 1]);
            assert(p->isAlloc);

            WPtr startP = p;
            WPtr endP = startP + startP->wSize - 1;
            WPtr nextP = endP + 1;
            if (!nextP->isAlloc)
            {
                startP->wSize += nextP->wSize;
                endP = startP + startP->wSize - 1;
            }

            WPtr lastP = startP - 1;
            if (!lastP->isAlloc)
            {
                startP = lastP;
                startP->wSize += p->wSize;
            }

            startP->isAlloc = 0;
            *endP = *startP;
        }
    };
}