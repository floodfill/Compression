/*-----------------------------------------------------------------------------
 *  err_utils.cpp - Output routines for err/debug information
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

#include "err_utils.hpp"

using namespace std;

static void __init_push_log(void);
static void __sig_handler(int n);

static FILE     *__log;
static int      __init_utils;
static sigjmp_buf       ctx;

void
opc_misc::sigsegv_handler(int sig)
{
        siglongjmp(ctx, 1);
}

int
opc_misc::validate_address(void *ptr, uint32_t range)
{
        struct sigaction        act;
        struct sigaction        orig_act;

        int ret = 1;

        sigemptyset(&act.sa_mask);

        act.sa_flags = 0;
        act.sa_handler = opc_misc::sigsegv_handler;

        sigaction(SIGSEGV, &act, &orig_act);

        if (sigsetjmp(ctx, 1) == 0) {
                volatile uint8_t        d;
                volatile uint8_t        *p;

                p = (volatile uint8_t *)ptr;

                for (uint32_t i = 0; i < range; i++, p++)
                        d = *p;
        } else {
                ret = 0;
        }

        sigaction(SIGSEGV, &orig_act, NULL);

        return ret;
}

void 
opc_misc::err_print(const char *func,
                int32_t line, const char *fmt, ...)
{
        int             n;
        char            buf[1024];
        va_list         ap;

        memset(buf, 0x00, 1024);

        va_start(ap, fmt);
        if ((n = vsnprintf(buf, sizeof(buf), fmt, ap)) == -1)
                exit(EXIT_FAILURE);
        va_end(ap);

        buf[n] = '\n';

        cerr << func << "(" << line << "): " << buf;

        exit(EXIT_FAILURE);
}

void 
opc_misc::push_log(const char *func,
                int32_t line, const char *fmt, ...)
{
        int             n;
        char            buf[1024];
        va_list         ap;

        memset(buf, 0x00, 1024);

        if (!__init_utils++)
                __init_push_log();

        va_start(ap, fmt);

        if ((n = vsnprintf(buf, sizeof(buf), fmt, ap)) == -1) {
                cerr << "_push_log(): Irregal format errors" << endl; 
                exit(EXIT_FAILURE);
        }

        va_end(ap);

        buf[n] = '\n';

        fprintf(__log, "%s(%d): ", func, line);
        fprintf(__log, "%s", buf);
}

void
opc_misc::flush_log(void)
{
        int __errno = errno;

        if (__log != NULL) {
                fflush(__log);
                fclose(__log);
        }

        errno = __errno;
}

/* --- Intra functions below */

#define SIGSEGV 11

void
__sig_handler(int n)
{
        opc_misc::flush_log();

        cerr << "SIGSEGV is catched" << endl;
        exit(EXIT_FAILURE);
}

void
__init_push_log(void)
{
        /* Open a file for debug information */
        __log = fopen("output.log", "w");

        if (__log == NULL) {
                cerr << "_push_log(): Can't open a log file" << endl;
                exit(EXIT_FAILURE);
        }

        /* Register sinal functions */
        if (SIG_ERR == signal(SIGSEGV, __sig_handler)) {
                cerr << "_push_log(): Can't register signal functions" << endl;
                exit(EXIT_FAILURE);
        }
}

