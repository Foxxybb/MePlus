#pragma once
class MyMalloc
{
public:
	unsigned int memArray[8];

	// constructor
	MyMalloc();

	// generate a data block to be inserted into the memArray
	void alloc();


};

