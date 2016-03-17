#pragma once

#if SAMD_SERIES
// samd is supposed to have a strncmp_P mapping to regular strncmp,
// but doesn't for some reason so do it here
#define strncmp_P(s1, s2, n) strncmp((s1), (s2), (n))
#endif
