// code sizes for atmega32u4

#define TEST_DEPTH 1

// FEATURE_EVENT_ORIGINAL:
// depth 0:
// 4114 - 3894 = 220 code used
//  210 -  148 =  62 data used
// depth 1:
// 4146 - 4114 =  32 code used
//  211 -  210 =   1 data used
#define FEATURE_EVENT_ORIGINAL


// FEATURE_EVENT_VA:
// depth 0:
// 4186 - 3894 = 292 code used
//  210 -  148 =  62 data used
// depth 1:
// 4234 - 4186 =  48 code used
//  211 -  210 =   1 data used
//#define FEATURE_EVENT_VA

// FEATURE_EVENT_TEMPLATE1:
// depth 0:
// 4074 - 3894 = 180 code used
//  210 -  148 =  62 data used
// depth 1:
// 4128 - 4074 =  54 code used
//  211 -  210 =   1 data used
//#define FEATURE_EVENT_TEMPLATE1

// FEATURE_EVENT_TEMPLATE2:
// depth 0:
// 4162 - 3894 = 268 code used
//  210 -  148 =  62 data used
// depth 1:
// 4208 - 4162 =  46 code used
//  211 -  210 =   1 data used
//#define FEATURE_EVENT_TEMPLATE2


#if defined(FEATURE_EVENT_ORIGINAL) || defined(FEATURE_EVENT_VA) || defined(FEATURE_EVENT_TEMPLATE1) || defined(FEATURE_EVENT_TEMPLATE2)
#define FEATURE_EVENT
#endif
