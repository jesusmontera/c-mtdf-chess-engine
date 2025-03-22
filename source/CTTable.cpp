#include "CTTable.h"

CTTable::CTTable()
{
    //ctor
}

CTTable::~CTTable()
{
    //dtor
}
void CTTable::Initialize(int MB)
{
    clear();
    this->entries.resize((MB * 1024 * 1024) / sizeof(TTEntry), TTEntry());
    std::fill(entries.begin(), entries.end(), TTEntry());

    //std::cout << "Transposition Table Initialized with " << entries.size() << " entries (" << MB << "MB)" << std::endl;
}
void CTTable::reset_values()
{
    std::fill(entries.begin(), entries.end(), TTEntry());
}


void CTTable::clear()
{

    entries.clear();
}

bool CTTable::find_score(uint64_t key, int depth, bool root,TT_LIMITS *limits)
{
    uint32_t k32=reduce_hash(key, entries.size());

    if (static_cast<uint16_t>(key) == entries[k32].key){
        if (entries[k32].root ==root && entries[k32].depth==depth){
            *limits=entries[k32].limits;
            return true;
        }
    }
    return false;

}


void CTTable::store_score(uint64_t key, int depth, uint8_t root,TT_LIMITS &limits)
{
    TTEntry& entry = entries[reduce_hash(key, entries.size())];
    entry.key = static_cast<uint16_t>(key);
    entry.depth = depth;
    entry.root=root;
    entry.limits=limits;

}
