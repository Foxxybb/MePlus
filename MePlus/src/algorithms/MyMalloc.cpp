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

	cout << dataSize << " : " << dataIdx << endl;

	// test function with single int placements
	//placeSingle(dataSize, dataIdx);

	placeDataBlock(dataSize, dataIdx);
}

// attempt to place single int at desired index, if no space, try next index until full rotation
void MyMalloc::placeSingle(int size, int idx) {
	int firstIdx = idx;

	// loop finding index to place
	while (true) {
		// if placing index == 0, place the data
		if (memArr[idx] == 0) {
			memArr[idx] = size;
			cout << "placing " << size << " : at " << idx << endl;
			break;
		}
		else {
			// else, try next index
			idx++;
			// if dataIdx > 7, reset to 0
			if (idx > 7) { idx = 0; }
		}

		// break loop if full rotation occurs
		if (idx == firstIdx) {
			cout << "Attempted to place " << size << " at " << idx << endl;
			cout << "Insufficient space in array." << endl;
			break;
		}
	}

	// print status of memArray
	for (int i = 0; i < 8; i++) {
		cout << memArr[i] << ", ";
	}
	cout << endl;
}

// attempt to place integer at index, then place additional integers for size int along array
// if end of array is reached, loop back to 0
// if insufficient space, move along array and check for space until full rotation
void MyMalloc::placeDataBlock(int size, int idx) {
	int firstIdx = idx; // used to break loop 

	// loop finding index to place
	while (true) {
		// check if target index is empty
		if (spaceAvailable(size, idx)) {
			// place data block
		}
		else {
			// else, try next index
			idx++;
			// if dataIdx > 7, reset to 0
			if (idx > 7) { idx = 0; }
		}

		// break loop if full rotation occurs
		if (idx == firstIdx) {
			cout << "Attempted to place " << size << " at " << idx << endl;
			cout << "Insufficient space in array." << endl;
			break;
		}
	}
}

bool MyMalloc::spaceAvailable(int size, int idx) {

	// based on input, make list of indexes to check if they are empty


	return false;
}

