/*-----------------------------------------------------------------------------
 *  BinaryInterpolative.hpp - A encoder/decoder for Binary Interpolative
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

#ifndef __BINARYINTERPOLATIVE_HPP__
#define __BINARYINTERPOLATIVE_HPP__

#include "xxx_common.hpp"

#include "io/BitsWriter.hpp"
#include "io/BitsReader.hpp"

namespace opc {

class BinaryInterpolative {
public:
        static void encodeArray(uint32_t *in, uint32_t len,
                        uint32_t *out, uint32_t &nvalue) {
                out[0] = in[len - 1];

                BitsWriter wt(out + 1);
                wt.InterpolativeArray(in, len, 0, 0, in[len - 1]);
                wt.bit_flush();

                nvalue = wt.get_written() + 1;
        }

        static void decodeArray(uint32_t *in, uint32_t len,
                        uint32_t *out, uint32_t nvalue) {
                BitsReader rd(in + 1);
                rd.InterpolativeArray(out, nvalue, 0, 0, *in);
        }
}; /* BinaryInterpolative */

}; /* namespace: opc */

#endif /* __BINARY_INTERPOLATIVE_HPP__ */

