template <class TInput, class TOutput>
class TokenizerBase
{
public:
};

class Tokenizer : TokenizerBase<char, char*>
{
  const char* delimiters;

protected:
  uint8_t bufferPos = 0;
  char* buffer;

public:
  Tokenizer(char* buffer, const char* delimiters) : buffer(buffer), delimiters(delimiters)
  {}

  bool parse(char input);
  bool isDelimiter(char input);

  // returns length of token including trailing \0
  // meaning that an actual return value of 0 means end-of-input
  // and no token can be found
  uint8_t parseToken(char* input)
  {
    if(*input == 0) return 0;

    while(!parse(*input++));
    // include \0 so that caller can += their input
    // remember 'input' itself is expected to be \0 terminated, so
    // + 1 at worst should put us right on the already-existing \0
    return bufferPos + 1;
  }

  char* getBuffer() { return buffer; }

  void reset()
  {
    bufferPos = 0;
  }
};


// Use when buffer is already allocated elsewhere, we
// can re-use it via this mechanism - just note that
// null char (0) is not assumed to be appended, so either
// inspect getLength() to know how long the found token is,
// or use the destructive operations which write back
// to the buffer
class TokenizerInPlace : public Tokenizer
{
public:
  TokenizerInPlace(char* buffer, const char* delimiters) :
    Tokenizer(buffer, delimiters)
  {}

  // parse a single character from buffer and advance
  bool parse();

  uint8_t parseToken()
  {
    while(!parse());

    return bufferPos;
  }

  // grabs a token, parses it, appends a \0 (destructive) and auto-advances
  // to next token
  char* parseTokenDestructive();

  void advance()
  {
    buffer += bufferPos + 1;
    bufferPos = 0;
  }

  // return length of discovered buffer sans \0
  uint8_t getLength() { return bufferPos; }

  // If we know it's safe to modify original buffer at the delimiter, then we can
  // do so and create a real string right in the original buffer
  char* getBufferDestructive()
  {
    buffer[bufferPos] = 0;
    return buffer;
  }

  // destructively parse a token + issues a callback with results
  template <class T>
  bool parseTokenDestructive(T id,
    bool (*callback)(T id, char* buffer, void* context),
    void* context = NULL)
  {
    // grab token, append \0 and auto-advance
    char* token = parseTokenDestructive();

    // callback indicates whether to abort the call early.  This is not an error, but rather
    // the consumer telling us we don't need to process anything further
    return callback(id, token, context);
  }
};
