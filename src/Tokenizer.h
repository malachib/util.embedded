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


// Use when buffer is already allocated elsewhere, we
// can re-use it via this mechanism - just note that
// null char (0) is not appended, so inspect getLength()
// to know how long the found token is
class TokenizerInPlace : public Tokenizer
{
public:
  TokenizerInPlace(char* buffer, char* delimiters) :
    Tokenizer(buffer, delimiters)
  {}

  bool parse();

  void advance()
  {
    buffer += bufferPos;
    bufferPos = 0;
  }

  uint8_t getLength() { return bufferPos; }

  // If we know it's safe to modify original buffer at the delimiter, then we can
  // do so and create a real string right in the original buffer
  char* getBufferDestructive()
  {
    buffer[bufferPos] = 0;
    return buffer;
  }
};
