#include <iostream>
#include <vector>
#include <list>
#include <cmath>
using namespace std;


struct LRU {
    int n = 0;
    list<int> queue; 
    
    LRU(int n) : n(n){
    }
    //function used for LRU that when we have a hit, it becomes the most recent
    //since the index could be in the middle, you erase it from the middle and put it in front
    void refer(int index) { 
        for (auto it = queue.cbegin(); it != queue.cend(); ++it)
            if (*it == index) {
                queue.erase(it);
                queue.push_front(index);
                break;
            }
    }
    //a function that gets the least recently used index which will be at the back of the queue
    int lru_index() {
        if (queue.size() < n) {
            // not full
            queue.push_front(queue.size());
        }
        else {
            // full
            int index = queue.back(); // least recently used in the back
            queue.pop_back(); // drop least recently used
            queue.push_front(index);
        }
        return queue.front(); 
    }
};

struct CacheBlock
{
    bool valid = false;
    int tag = 0;
    //std::vector<char> block;
    // CacheBlock(int blocksize) : block(blocksize) {}
};

struct Cache
{
    vector<vector<CacheBlock>> sets;
    vector<LRU> lrus;

    int blocksize;
    int assoc;
    char repl;
    int b, s, t; //How many bits take offset, set, and tag in an address
    int w_cnt, r_cnt, r_miss, w_miss;

    Cache(int nsets, int assoc, int blocksize, char repl) : 
        blocksize(blocksize), assoc(assoc), repl(repl), w_cnt(0), r_cnt(0),r_miss(0), w_miss(0)
    {
        sets.resize(nsets, vector<CacheBlock>(assoc, CacheBlock()));
        if (repl == 'l')
            lrus.resize(nsets, LRU(assoc));
        //computing the bits of the address 
        b = log2(blocksize);
        s = log2(nsets);
        t = 64 - b - s;
    }

    void access(char op, uint64_t addr)
    {  
        //increment since its an access
        if(op == 'r')
            r_cnt++;
        else
            w_cnt++;

        // addr --> iset, tag
        int tag = (addr >> b) >> s;
        int iset = ((addr << t) >> t) >> b; // set index found by shifting bits of the hexadecimal address

        bool hit = false;
        for(int i = 0; i < assoc; i++){ //looping until we get a hit
            if(sets[iset][i].valid && tag == sets[iset][i].tag){
                hit = true;
                //when we get a hit, make it most recent
                lrus[iset].refer(i);
                break;        
            }
        }
        //since we don't have a hit we have to add it to the cache
        if (!hit) {
            if(op == 'r')
                r_miss++;
            else
                w_miss++;
                int index = 0;
                //an inefficient way to add to the cache by getting a random index to add to the cache
                if (repl == 'r') {
                    index = rand() % assoc;
                } 
                //finds a place to replace using LRU
                else {
                    index = lrus[iset].lru_index();
                }
                //adding to the cache using the index that was gotten either by LRU or random
                sets[iset][index].tag = tag;
                sets[iset][index].valid = true;
            // }
        }
    }

    void print() {
        // TODO: print statistics
        /*
        The total number of misses,
        The percentage of misses (i.e. total misses divided by total accesses),
        The total number of read misses,
        The percentage of read misses (i.e. total read misses divided by total read accesses),
        The total number of write misses,
        The percentage of write misses (i.e. total write misses divided by total write accesses).        
        */
       //if (r_cnt + w_cnt == 0)

       cout 
        << r_miss + w_miss << " " 
        << 100.f * (r_miss + w_miss)/(w_cnt + r_cnt) << "% ";

        if (r_cnt > 0) {
            cout << r_miss << " "  << 100.f * (r_miss)/(r_cnt) << "% ";
        } else {
            cout << r_miss << " "  << 0 << "% ";
        }

        if (w_cnt > 0) {
            cout << w_miss << " "  << 100.f * (w_miss)/(w_cnt) << "% ";
        } else {
            cout << w_miss << " "  << 0 << "% ";
        }

    }
};


int main(int argc, char**argv){
    //getting the command line arguments and formatting them to proper type
    int nk = atoi(argv[1]);
    int assoc = atoi(argv[2]);
    int blocksize = atoi(argv[3]);
    char* repl = argv[4]; 

    //number of sets
    int nsets = (nk * 1024) / (blocksize * assoc);

    Cache cache(nsets, assoc, blocksize, repl[0]);

    char op;
    uint64_t addr;
    while(cin>>op>>hex>>addr){ //while there is input
        if(op != 'w' && op != 'r')
            break;

        cache.access(op, addr); //the function which will check if in cache
    }

    cache.print();

    return 0;
}