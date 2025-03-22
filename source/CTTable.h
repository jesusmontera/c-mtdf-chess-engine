#ifndef CTTABLE_H
#define CTTABLE_H
#include <cstdint>
#include <vector>
struct TT_LIMITS
{
    int lower=0;
    int upper=0;
};

struct TTEntry {
    uint8_t depth = 0;
    uint16_t key = 0;
    uint8_t root =0;
    TT_LIMITS limits;
};


class CTTable
{
    public:
        CTTable();
        virtual ~CTTable();
        void Initialize(int MB);
        void clear();
        void store_score(uint64_t key, int depth, uint8_t root,TT_LIMITS &limits);
        void reset_values();
        bool find_score(uint64_t key, int depth, bool root,TT_LIMITS *limits);
    protected:

    private:
        std::vector<TTEntry> entries;
        static inline uint32_t reduce_hash(uint32_t x, uint32_t N) { return ((uint64_t)x * (uint64_t)N) >> 32; };

};

#endif // CTTABLE_H
