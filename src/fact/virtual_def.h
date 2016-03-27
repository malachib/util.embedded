// FACT_VE means Virtual Enabled
#if FACT_VE
#define VIRTUAL virtual
#define ABSTRACT = 0
#define OVERRIDE override
#else
#define VIRTUAL
#define ABSTRACT {}
#define OVERRIDE
#endif

#undef FACT_VE
