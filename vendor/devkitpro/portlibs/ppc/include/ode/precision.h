#ifndef _ODE_PRECISION_H_
#define _ODE_PRECISION_H_

/* Define dSINGLE for single precision, dDOUBLE for double precision,
 * but never both!
 */

#if defined(dIDESINGLE)
#define dSINGLE
#elif defined(dIDEDOUBLE)
#define dDOUBLE
#else
#define dSINGLE
#endif

#endif
