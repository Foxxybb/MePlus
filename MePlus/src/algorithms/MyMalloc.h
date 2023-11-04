#include <list>
#include <glm/glm.hpp>

#include "../graphics/models/data.hpp"

using namespace std;
using namespace glm;

class MyMalloc
{
public:
	unsigned int memArr[8]; // memory array that is represented by data cubes
    vector<Data> dataCubes; // list of data cube models

    // positions for data cubes "in" the array
    vec3 dataCubePos[8] = {
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

	void placeSingle(int size, int idx);

	void placeDataBlock(int size, int idx);

	list<int> spaceAvailable(int size, int idx);

    void positionCheck();

};

