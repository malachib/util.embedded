#include <stdint.h>

namespace FactUtilEmbedded
{

// TODO: break into layer1, layer2 etc for memory opt
template <class T>
class IDataConsumer
{
public:
    abstract bool consume(T& in);
};


template <class T>
class DataPump
{
    IDataConsumer& consumer;

public:
    DataPump(IDataConsumer& consumer) : consumer(consumer) {}
};


class IOStreamDataPump : DataPump<char>
{

};

}
