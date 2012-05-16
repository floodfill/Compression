/*-----------------------------------------------------------------------------
 *  VSEncoding.hpp - To scan the optimal partitions in a list
 *
 *  Coding-Style:
 *      emacs) Mode: C, tab-width: 8, c-basic-offset: 8, indent-tabs-mode: nil
 *      vi) tabstop: 8, expandtab
 *
 *  Authors:
 *      Takeshi Yamamuro <linguin.m.s_at_gmail.com>
 *      Fabrizio Silvestri <fabrizio.silvestri_at_isti.cnr.it>
 *      Rossano Venturini <rossano.venturini_at_isti.cnr.it>
 *-----------------------------------------------------------------------------
 */

#ifndef __VSENCODING_HPP__
#define __VSENCODING_HPP__

#include "xxx_common.hpp"

/*
 * If a list is larger that this parameter, then
 * it is split into subblocks that are compressed
 * separatelly. Compression is slightly worse, but
 * the decompression is more cache-friendly.
 */
#define VSENCODING_BLOCKSZ      65536U

using namespace std;

namespace opc {

/* FIXME: Need to be re-implemented in a thread-safe way */
extern uint32_t __vsencoding_aux[
                        VSENCODING_BLOCKSZ * 2 + 128];

class VSEncoding {
private:
        /*
         * In some cases, there might be asymmetry between possible
         * lenghts of blocks if they are formed by zeros(posszLens)
         * or larger numbers(possLens).
         */
        bool            is_aligned;

        uint32_t        poss_sz;
        uint32_t        maxBlk;
        uint32_t        *possLens;
        uint32_t        *posszLens;
        
public:
        explicit VSEncoding(uint32_t *lens,
                        uint32_t *zlens, uint32_t size, bool is_aligned);

        /*
         * Compute the optimal sub-lists from lists.
         *      fixCost: The fix cost in bits that we pay for  each block
         */
        void compute_OptPartition(vector<uint32_t>& seq,
                        uint32_t fixCost, vector<uint32_t>& parts);
}; /* VSEncoding */

}; /* namespace: opc */

#endif /* __VSENCODING_HPP__ */

