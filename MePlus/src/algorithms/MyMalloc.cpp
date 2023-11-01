#include "MyMalloc.h"

#include <iostream>;
#include <list>;

using namespace std;

// constructor
MyMalloc::MyMalloc() {

}

// generate a data block to be inserted into the memArray
void MyMalloc::alloc() {
	//cout << "Hello from MyMalloc" << endl;

	// generate a random int 1-4 as data block size
	int dataSize = rand() % 4 + 1;

	// generate a random int 0-7 as index to place data in
	int dataIdx = rand() % 8;

	cout << dataSize << " : " << dataIdx << endl;

	// test function with single int placements
	//placeSingle(dataSize, dataIdx);

	placeDataBlock(dataSize, dataIdx);

	// print status of memArray
	for (int i = 0; i < 8; i++) {
		cout << memArr[i] << ", ";
	}
	cout << endl;
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
}

// attempt to place integer at index, then place additional integers for size int along array
// if end of array is reached, loop back to 0
// if insufficient space, move along array and check for space until full rotation
void MyMalloc::placeDataBlock(int size, int idx) {
	int firstIdx = idx; // used to break loop 

	// loop finding index to place
	while (true) {
		// check if target indices are available
		list<int> curIdxSpace = spaceAvailable(size, idx);
		
		if (!curIdxSpace.empty()) {
			cout << "curIdxSpace: ";
			for (auto it = curIdxSpace.begin(); it !=
				curIdxSpace.end(); ++it)
				cout << ' ' << *it;
			cout << endl;

			// if current space is available
			// place data block at target indices
			for (int space : curIdxSpace) {
				memArr[space] = size;
			}
			break;
		}
		else {

			cout << "No space available, bumping index" << endl;

			// else, try next index
			idx++;
			// if dataIdx > 7, reset to 0
			if (idx > 7) { idx = 0; }
		}

		// break loop if full rotation occurs
		if (idx == firstIdx) {
			cout << "Attempted to place " << size << " at " << idx << endl;
			cout << "Insufficient space in array." << endl;
			cout << "Resetting memArr" << endl;
			for (int i = 0; i < 8; i++) {
				memArr[i] = 0;
			}
			break;
		}
	}
}

// returns list of indices that we are trying to occupy with the current data block
// if indices are occupied, returns empty list
list<int> MyMalloc::spaceAvailable(int size, int idx) {

	// based on input, make list of indices to check if they are empty (0)
	list<int> idxList;
	idxList.push_back(idx);

	// add all indices that datablock should occupy
	for (int i = 1; i < size; i++) {
		// if index exceeds 7, then subtract by 8 to loop to 0
		if ((idx + i) > 7) {
			idxList.push_back(idx + i - 8);
		}
		else {
			idxList.push_back(idx + i);
		}
	}

	// finally, check memArr if indices are empty (0)
	// if they are not all empty, return an empty list
	for (int memIdx : idxList) {
		if (memArr[memIdx] != 0) {
			return list<int>{}; // return empty list if all space isn't fully available
		}
	}

	return idxList;
}
