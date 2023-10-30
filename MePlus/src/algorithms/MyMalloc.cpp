#include "MyMalloc.h"

#include <iostream>;

using namespace std;

// constructor
MyMalloc::MyMalloc() {

}

// generate a data block to be inserted into the memArray
void MyMalloc::alloc() {
	//cout << "Hello from MyMalloc" << endl;

	// generate a random int 1-5 as data block size
	int dataSize = rand() % 5 + 1;

	// generate a random int 0-7 as index to place data in
	int dataIdx = rand() % 8;
	int firstIdx = dataIdx;

	// loop finding index to place
	while (true) {
		// if placing index == 0, place the data
		if (memArray[dataIdx] == 0) {
			memArray[dataIdx] = dataSize;
			cout << "placing " << dataSize << " : at " << dataIdx << endl;
			break;
		}
		else {
			// else, try next index
			dataIdx++;
			// if dataIdx > 7, reset to 0
			if (dataIdx > 7) { dataIdx = 0; }
		}

		// break loop if full rotation occurs
		if (dataIdx == firstIdx) {
			cout << "Attempted to place " << dataSize << " at " << dataIdx << endl;
			cout << "Insufficient space in array." << endl;
			break;
		}
	}

	// print status of memArray
	for (int i = 0; i < 8; i++) {
		cout << memArray[i] << ", ";
	}
	cout << endl;
}