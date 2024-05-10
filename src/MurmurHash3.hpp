//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

#ifndef _MURMURHASH3_H_
#define _MURMURHASH3_H_

//-----------------------------------------------------------------------------
// #include <stdlib.h>
// i think we use cstdint since cpp now?
#include <cstdint>

//-----------------------------------------------------------------------------

void MurmurHash3_x86_32  ( const uint32_t* key, uint32_t seed, uint32_t* out );

//-----------------------------------------------------------------------------

#endif // _MURMURHASH3_H_