/*-----------------------------------------------------------------------------
 *  VariableByte.cpp - A implementation of VariableByte
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

#include "VariableByte.hpp"

#define VARIABLEBYTE_DESC       0x80
#define VARIABLEBYTE_DATA       (VARIABLEBYTE_DESC - 1)

#define VARIABLEBYTE_EXT7BITS(value, num)         (value >> (7 * num)) & 0x7f

using namespace opc;

void
VariableByte::encodeArray(uint32_t *in, uint32_t len,
                uint32_t *out, uint32_t &nvalue)
{
        BitsWriter wt(out);

        for (uint32_t i = 0; i < len; i++) {
                uint32_t nwords = __get_msb(in[i]) / 7;

                switch(nwords) {
                case 0:
                        wt.bit_writer(1, 1);
                        wt.bit_writer(VARIABLEBYTE_EXT7BITS(in[i], 0), 7);
                        break;

                case 1:
                        wt.bit_writer(0, 1);
                        wt.bit_writer(VARIABLEBYTE_EXT7BITS(in[i], 0), 7);
                        wt.bit_writer(1, 1);
                        wt.bit_writer(VARIABLEBYTE_EXT7BITS(in[i], 1), 7);
                        break;

                case 2:
                        wt.bit_writer(0, 1);
                        wt.bit_writer(VARIABLEBYTE_EXT7BITS(in[i], 0), 7);
                        wt.bit_writer(0, 1);
                        wt.bit_writer(VARIABLEBYTE_EXT7BITS(in[i], 1), 7);
                        wt.bit_writer(1, 1);
                        wt.bit_writer(VARIABLEBYTE_EXT7BITS(in[i], 2), 7);
                        break;

                case 3:
                        wt.bit_writer(0, 1);
                        wt.bit_writer(VARIABLEBYTE_EXT7BITS(in[i], 0), 7);
                        wt.bit_writer(0, 1);
                        wt.bit_writer(VARIABLEBYTE_EXT7BITS(in[i], 1), 7);
                        wt.bit_writer(0, 1);
                        wt.bit_writer(VARIABLEBYTE_EXT7BITS(in[i], 2), 7);
                        wt.bit_writer(1, 1);
                        wt.bit_writer(VARIABLEBYTE_EXT7BITS(in[i], 3), 7);
                        break;

                case 4:
                        wt.bit_writer(0, 1);
                        wt.bit_writer(VARIABLEBYTE_EXT7BITS(in[i], 0), 7);
                        wt.bit_writer(0, 1);
                        wt.bit_writer(VARIABLEBYTE_EXT7BITS(in[i], 1), 7);
                        wt.bit_writer(0, 1);
                        wt.bit_writer(VARIABLEBYTE_EXT7BITS(in[i], 2), 7);
                        wt.bit_writer(0, 1);
                        wt.bit_writer(VARIABLEBYTE_EXT7BITS(in[i], 3), 7);
                        wt.bit_writer(1, 1);
                        wt.bit_writer(VARIABLEBYTE_EXT7BITS(in[i], 4), 7);
                        break;

                default:
                        eoutput("Exception: nwords's out of range");

                }
        }

        wt.bit_flush();

        nvalue = wt.get_written();
}

void
VariableByte::decodeArray(uint32_t *in, uint32_t len,
                uint32_t *out, uint32_t nvalue)
{
        BitsReader rd(in);

        for (uint32_t i = 0; i < nvalue; i++) {
                uint32_t d = rd.bit_reader(8);

                *out = d & VARIABLEBYTE_DATA;

                for (uint32_t j = 1; (d & VARIABLEBYTE_DESC) == 0; j++) {
                        __assert(j <= 5);

                        d = rd.bit_reader(8);
                        *out |= (d & VARIABLEBYTE_DATA) << (7 * j);
                }

                out++;
        }
}

