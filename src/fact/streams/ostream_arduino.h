// TODO: put fact_ostream in the proper namespace - it's in NONE right now!
#ifndef FEATURE_IOSTREAM
typedef FactUtilEmbedded::std::ostream fact_ostream;
typedef FactUtilEmbedded::std::istream fact_istream;
#else
typedef ::std::ostream fact_ostream;
typedef ::std::istream fact_istream;
#endif

inline fact_ostream& operator <<(
    fact_ostream& out, const __FlashStringHelper* arg)
{ 
    out << (const char*)(arg); 
    return out; 
}
