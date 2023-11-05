#include <list>
#include <glm/glm.hpp>

#include "../graphics/models/data.hpp"

using namespace std;
using namespace glm;

class MyMalloc
{
public:
	unsigned int memArr[8]; // memory array that is represented by data cubes
    //vector<Data> dataCubes; // list of data cube models
    vector<vector<Data>> dataBlocks; // vector of vectors that represent each datablock of size 1-4
    vector<Data> currentDataBlock; // data block that is currently being moved to find a space to be placed

    vec3 spawnPos = vec3(0.0f, 3.0f, 0.0f);

    // positions for data cubes "in" the array
    vec3 dataCubePlacedPos[8] = {
        vec3(3.2f, 1.0f, 1.0f), //0
        vec3(1.2f, 1.0f, 3.2f), //1
        vec3(-1.2f, 1.0f, 3.2f), //2
        vec3(-3.2f, 1.0f, 1.0f), //3
        vec3(-3.2f, 1.0f, -1.0f), //4
        vec3(-1.2f, 1.0f, -3.2f), //5
        vec3(1.2f, 1.0f, -3.2f), //6
        vec3(3.2f, 1.0f, -1.0f) //7
    };

    // positions for data cubes "out" of the array
    vec3 dataCubeRaisedPos[8] = {
        vec3(3.2f, 3.0f, 1.0f), //0
        vec3(1.2f, 3.0f, 3.2f), //1
        vec3(-1.2f, 3.0f, 3.2f), //2
        vec3(-3.2f, 3.0f, 1.0f), //3
        vec3(-3.2f, 3.0f, -1.0f), //4
        vec3(-1.2f, 3.0f, -3.2f), //5
        vec3(1.2f, 3.0f, -3.2f), //6
        vec3(3.2f, 3.0f, -1.0f) //7
    };

	// constructor
	MyMalloc();

	// generate a data block to be inserted into the memArray
	void alloc();

	void placeSingle(int size, int idx); // test function, unused

	void placeDataBlock(int size, int idx); // place ints of size into the memoryArray at the specified index

	vector<int> spaceAvailable(int size, int idx); // returns list of indices that can hold the current datablock

    void spawnCubes(int size); // spawn vector of cubes to represent the current datablock

    void moveCubes(); // updates the target position for the currentDataBlock cubes

    void positionCheck(); // function that runs every frame to check if current cubes are at their target position

};

