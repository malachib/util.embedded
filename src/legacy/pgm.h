#pragma once

#ifdef AVR
class PgmExtract
{
  //bool nearMode = true;

  protected:
    //virtual bool ShouldContinue(byte value) = 0;
    // TODO: Time to break apart Render & Continue logic
    virtual bool Render(byte value) = 0;

  public:
    void Extract(byte* input);
#ifdef PGM_FAR_AVAILABLE
    void Extract(uint_farptr_t input);
#endif
};

class PgmSerialExtract : public PgmExtract
{
  protected:
    virtual bool Render(byte value) override;
  public:
    void println(const char* input)  { Extract((byte*)input); }
};


class PgmStream //: public PgmExtract
{
  Stream& stream;
public:
  PgmStream(Stream& _stream) : stream(_stream) {}

  void println(const char* input);
};


class PgmStringExtract : public PgmExtract
{
  char* output;

    protected:
    virtual bool Render(byte value) override;

    public:
    PgmStringExtract(char* _output) : output(_output) {}
    PgmStringExtract() {}

    char* Extract(const char* input);
    char* Extract(const char* input, char* output);
};
#endif

#ifdef ARDUINO


void pgm_extract_bytes(byte* input, byte* output, int length);
void pgm_extract_string(const char* input, char* output);
void pgm_extract_bytes(byte* input, bool (*foo)(byte,void*));

#endif
