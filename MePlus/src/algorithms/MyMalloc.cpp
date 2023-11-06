#include "MyMalloc.h"

#include <iostream>;
#include <list>;

using namespace std;

// constructor
MyMalloc::MyMalloc() {

}

// main script to run endlessly
// performs a basic memory allocation algorithm
// uses cubes as a visual representation of the process
void MyMalloc::autoAlloc() {

	// GET RANDOM INTS TO PLACE =================================
	// generate a random int 1-4 as data block size
	int dataSize = rand() % 4 + 1;
	// generate a random int 0-7 as index to place data in
	int dataIdx = rand() % 8;
	cout << dataSize << " : " << dataIdx << endl;

	// SPAWN CUBES TO REPRESENT INTS ============================
	spawnCubes(dataSize);

	// MOVE CUBES TO POSITION ===================================
	moveCubes(dataIdx);

	// once cubes have arrived, they need to perform the next step
	// wait for cubes to arrive without stopping render and deadlocking
	// try to use multithreading for this
	while (cubesAreMoving) {
		// wait until cubes are positioned
		// this should deadlock the program until it is multithreaded
	}
}

// generate a data block to be inserted into the memArray
void MyMalloc::alloc() {

	// generate a random int 1-4 as data block size
	int dataSize = rand() % 4 + 1;

	// generate a random int 0-7 as index to place data in
	int dataIdx = rand() % 8;

	cout << dataSize << " : " << dataIdx << endl;

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
		vector<int> curIdxSpace = spaceAvailable(size, idx);
		
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
vector<int> MyMalloc::spaceAvailable(int size, int idx) {

	// based on input, make list of indices to check if they are empty (0)
	vector<int> idxList;
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
			return vector<int>{}; // return empty list if all space isn't fully available
		}
	}

	return idxList;
}

// spawn vector of cubes to represent the current datablock
void MyMalloc::spawnCubes(int size) {

	vector<Data> newVector;

	for (int i = 0; i < size; i++) {
		Data newData;
		newData.init();
		newData.size = vec3(0.7f);
		newData.rb.pos = spawnPos;
		newData.targetPos = newData.rb.pos;
		newVector.push_back(newData);
	}

	dataBlocks.push_back(newVector);
}

// updates the target position for the currentDataBlock cubes
void MyMalloc::moveCubes(int cubeIdx) {

	// for each cube in the current data block, assign the target position, and set velocity
	// the target position is determined by each available index in the Memory Array
	// dataCubeRaisedPos[availableIndex]
	
	// generate a random int 0-7 as index to place data in
	//int cubeIdx = rand() % 8;

	// get list of available indices
	// use current size of dataBlocks.back()
	vector<int> targetSpace = spaceAvailable(dataBlocks.back().size(), cubeIdx);

	for (auto it = targetSpace.begin(); it !=
		targetSpace.end(); ++it)
		cout << ' ' << *it;
	cout << endl;
	
	// currentDataBlock is accessed with dataBlocks.back()
	// use targetSpace to assign target positions for each cube
	int i = 0;
	for (Data &dataCube : dataBlocks.back()) {
		dataCube.targetPos = dataCubeRaisedPos[targetSpace[i]];
		dataCube.rb.velocity = dataCube.targetPos - dataCube.rb.pos;
		i++;
	}
	cubesAreMoving = true;

}

// lower all cubes in the currentDataBlock to the "allocated" position
void MyMalloc::placeCubes() {
	for (Data& dataCube : dataBlocks.back()) {
		dataCube.targetPos = vec3(dataCube.rb.pos.x, dataCube.rb.pos.y - 2, dataCube.rb.pos.z);
		dataCube.rb.velocity = dataCube.targetPos - dataCube.rb.pos;
	}
}

// for each dataCube, check if the dataCube is at its target position (within 0.1f range)
// if so, snap the dataCube to the target position and set velocity to 0
void MyMalloc::positionCheck() {

	// position check now occurs for every cube in the 2D vector: dataBlocks
	for (vector<Data> &dataBlock : dataBlocks) {
		for (Data &dataCube : dataBlock) {
			if (dataCube.rb.pos != dataCube.targetPos) {

				cubesAreMoving = true;
				cout << "Cubes are moving..." << endl;

				// if a cube is at target position, snap
				if ((dataCube.rb.pos.x > (dataCube.targetPos.x - 0.1f)) && (dataCube.rb.pos.x < (dataCube.targetPos.x + 0.1f))
					&& (dataCube.rb.pos.y > (dataCube.targetPos.y - 0.1f)) && (dataCube.rb.pos.y < (dataCube.targetPos.y + 0.1f))
					&& (dataCube.rb.pos.z > (dataCube.targetPos.z - 0.1f)) && (dataCube.rb.pos.z < (dataCube.targetPos.z + 0.1f)))
				{
					dataCube.rb.velocity = vec3(0.0f);
					dataCube.rb.pos = dataCube.targetPos;
					cubesAreMoving = false;
				}
				else {
					
				}
			}
		}
	}
}
