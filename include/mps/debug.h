/* 
 * File:   debug.h
 * Author: leonardo
 *
 * Created on 23 aprile 2011, 11.00
 */

#ifndef DEBUG_H
#define	DEBUG_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef __WINDOWS
#include <unistd.h>
#else
#include <io.h>
#endif


/* Keep away assert() when compiling without debug */
#ifdef DISABLE_DEBUG
#define NDEBUG
#endif

#ifndef DISABLE_DEBUG
/**
 * @brief Debug cluster approximations of the roots in the
 * case of multiprecision computation.
 */
#define MPS_DEBUG_MCLUSTER_ROOTS(s, i) if (s->DOLOG) { \
  __MPS_DEBUG(s, "Dumping cluster approximations:\n"); \
  for(s->debug_i = s->punt[i]; s->debug_i < s->punt[i+1]; s->debug_i++) { \
    __MPS_DEBUG(s, "%d: Approximation: ", s->debug_i - s->punt[i]); \
    mpc_out_str(s->logstr, 10, 10, s->mroot[s->clust[s->debug_i]]); \
    fprintf(s->logstr, " - Radius: "); \
    rdpe_outln(s->drad[s->clust[s->debug_i]]); \
} \
}

/**
 * @brief Print a debug information.
 */
#if __STDC_VERSION__ >= 199901L
#define MPS_DEBUG(s, templ...) __MPS_DEBUG(s,templ) ; \
if (s->DOLOG) { \
    fprintf(s->logstr, "\n"); \
}
#endif

/**
 * @brief Debug the value of a complex multiprecision
 * variable.
 */
#define MPS_DEBUG_MPC(s, name, digits, c) __MPS_DEBUG(s, name " = "); \
if (s->DOLOG) { \
    mpc_outln_str(s->logstr, 10, digits, c); \
}

/**
 * @brief Debug the value of a rdpe variable.
 */
#define MPS_DEBUG_RDPE(s, name, r) __MPS_DEBUG(s, name " = "); \
if (s->DOLOG) { \
    rdpe_outln(r); \
}

/**
 * @brief Debug the value of a cdpe variable.
 */
#define MPS_DEBUG_CDPE(s, name, c) __MPS_DEBUG(s, name " = "); \
if (s->DOLOG) { \
    cdpe_outln(c); \
}

/**
 * @brief Make some space in the debug stream to make clean that
 * another section is starting.
 */
#define MPS_DEBUG_BREAK(s) if (s->DOLOG) { \
    fprintf(s->logstr, "\n"); \
}

/**
 * @brief Debug that a function is going to be called.
 */
#define MPS_DEBUG_CALL(s, function) __MPS_DEBUG(s, "Calling \033[31;1m"); \
if (s->DOLOG) { \
    fprintf(s->logstr, function); fprintf(s->logstr, "()\033[0m\n"); \
}


/**
 * @brief Low-level DEBUG() used by other MPS_DEBUG_* statements.
 */
#if __STDC_VERSION__ >= 199901L
#ifndef __WINDOWS
#define __MPS_DEBUG(s, templ...) if (s->DOLOG) {\
if (isatty(s->logstr->_fileno)) { \
    fprintf(s->logstr, "%s:%d \033[32;1m%s()\033[;0m ", \
    __FILE__, __LINE__, __FUNCTION__); \
} \
else { \
    fprintf(s->logstr, "%s:%d %s() ", \
    __FILE__, __LINE__, __FUNCTION__); \
} \
gmp_fprintf(s->logstr, templ); \
}
#else
#define __MPS_DEBUG(s, templ...) if (s->DOLOG) {\
if (_isatty(_fileno(s->logstr))) { \
    fprintf(s->logstr, "%s:%d \033[32;1m%s()\033[;0m ", \
    __FILE__, __LINE__, __FUNCTION__); \
} \
else { \
    fprintf(s->logstr, "%s:%d %s() ", \
    __FILE__, __LINE__, __FUNCTION__); \
} \
gmp_fprintf(s->logstr, templ); \
}
#endif
#endif
#else
#define MPS_DEBUG(args...)
#define MPS_DEBUG_MPC(args...)
#define MPS_DEBUG_RDPE(args...)
#define MPS_DEBUG_CDPE(args...)
#define MPS_DEBUG_CALL(args...)
#define MPS_DEBUG_MCLUSTER_ROOTS(args...)
#endif

#ifndef DISABLE_DEBUG
#if __STDC_VERSION__ < 199901L
#include <mps/interface.h>
void MPS_DEBUG(mps_status* s, const char* templ, ...);
#endif
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* DEBUG_H */
