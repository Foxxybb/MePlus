#include <list>

using namespace std;

class MyMalloc
{
public:
	unsigned int memArr[8];

	// constructor
	MyMalloc();

	// generate a data block to be inserted into the memArray
	void alloc();

	void placeSingle(int size, int idx);

	void placeDataBlock(int size, int idx);

	list<int> spaceAvailable(int size, int idx);

};

