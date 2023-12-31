#include <list>
#include <glm/glm.hpp>

#include "../graphics/models/data.hpp"

using namespace std;
using namespace glm;

class MyMalloc
{
public:
	unsigned int memArr[8]; // memory array that is represented by data cubes
    unsigned int rootIdx = 0; // used to mark the start of the memArr search
    unsigned int rotateCount = 0; // used to check if full rotation has been made
    unsigned int currentIdx = 0; // used to track location of current datablock
    unsigned int currentSize = 0; // used to track current data block size
    vector<int> currentSpace; // current available space at the current index
    bool resetting; // bool to mark reset step for malloc

    vector<vector<Data>> dataBlocks; // vector of vectors that represent each datablock of size 1-4

    unsigned int allocStage = 0;

    enum allocCommand { CHECK = 0 , ROTATE = 1 , PLACE = 2 };

    bool cubesAreMoving = false; // used to check if all cubes have reached target position so next step in algorithm can start

    vec3 spawnPos = vec3(0.0f, 6.0f, 0.0f);

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

    void nextAllocStep(int command);

    void resetMalloc();

    void spawnAndSet(); // spawns new set of cubes at random index

    void checkSpace(); // check if current index has space for current data block
	
	void alloc(); // generate a data block to be inserted into the memArray

	void placeSingle(int size, int idx); // test function, unused

	void placeDataBlock(int size, int idx); // place ints of size into the memoryArray at the specified index

	vector<int> spaceAvailable(int size, int idx); // returns list of indices that can hold the current datablock

    void spawnCubes(int size); // spawn vector of cubes to represent the current datablock

    void moveCubesToStartIdx(int idx); // moves current dataBlock to the raised, 0 index position

    void rotateCubes();

    void moveCubes(int idx); // updates the target position for the currentDataBlock cubes

    void placeCubes(); // sets the target positions for the currentDataBlock cubes to the "lowered" position

    void positionCheck(); // function that runs every frame to check if current cubes are at their target position

};

