// code sizes for atmega32u4

#define TEST_DEPTH 3

// FEATURE_EVENT_ORIGINAL:
// depth 0:
// 4114 - 3894 = 220 code used
//  210 -  148 =  62 data used
// depth 1:
// 4146 - 4114 =  32 code used
//  211 -  210 =   1 data used
// depth 2:
// 4178 - 4146 =  32 code used
//  212 -  211 =   1 data used
// depth 3:
// 4282 - 4178 = 104 code used
//  213 -  212 =   1 data used
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
// 4182 - 4074 = 108 code used
//  211 -  210 =   1 data used
// depth 2:
// 4152 - 4182 = -30 code used (what?)
//  212 -  211 =   1 data used
// depth 3:
// 4220 - 4152 =  68 code used
//  213 -  212 =   1 data used
//#define FEATURE_EVENT_TEMPLATE1

// FEATURE_EVENT_TEMPLATE2:
// depth 0:
// 4162 - 3894 = 268 code used
//  210 -  148 =  62 data used
// depth 1:
// 4208 - 4162 =  46 code used
//  211 -  210 =   1 data used
// depth 2:
// 4236 - 4208 =  28 code used
//  212 -  211 =   1 data used
// depth 3:
// 4310 - 4236 =  74 code used
//  213 -  212 =   1 data used
//#define FEATURE_EVENT_TEMPLATE2


#if defined(FEATURE_EVENT_ORIGINAL) || defined(FEATURE_EVENT_VA) || defined(FEATURE_EVENT_TEMPLATE1) || defined(FEATURE_EVENT_TEMPLATE2)
#define FEATURE_EVENT
#endif
