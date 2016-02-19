template <class TInput, class TOutput>
class TokenizerBase
{
public:
};

class Tokenizer : TokenizerBase<char, char*>
{
  char* delimiters;

protected:
  uint8_t bufferPos = 0;
  char* buffer;

public:
  Tokenizer(char* buffer, char* delimiters) : buffer(buffer), delimiters(delimiters)
  {}

  bool parse(char input);
  bool isDelimiter(char input);

  char* getBuffer() { return buffer; }

  void reset()
  {
    bufferPos = 0;
  }
};


class TokenizerInPlace : public Tokenizer
{
public:
  bool parse();

  void reset()
  {
    buffer += bufferPos;
    bufferPos = 0;
  }

  uint8_t getLength() { return bufferPos; }
};
