template <class TInput, class TOutput>
class TokenizerBase
{
public:
};

class Tokenizer : TokenizerBase<char, char*>
{
  char* buffer;
  char* delimiters;
  uint8_t bufferPos = 0;

public:
  Tokenizer(char* buffer, char* delimiters) : buffer(buffer), delimiters(delimiters)
  {}

  bool parse(char input);
  bool isDelimiter(char input);
  
  char* getBuffer();
};
