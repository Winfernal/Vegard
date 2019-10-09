#pragma once
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "../../std_lib_facilities.h"

template <class T> //template for � utnytte polymorfisme
class ringBuffer
{

public:
	ringBuffer();
	ringBuffer(int size);
	~ringBuffer();

	bool notEmpty(); //buffer ikke tomt?
	bool notFull(); //buffer ikke fult?
	bool full(); //buffer fult?

	void put(T val); //legg til verdi i buffer //producer
	T get(); //f� tak i verdi i buffer //consumer
	void printindex(); //debuging





private:

	void incWriteIndex(); //�ke plass p� head(write) i bufferet
	void incReadIndex(); //�ke plass p� tail(read) i bufferet
	int WriteIndex = 0; //head skriver inn data p� index plass 
	int ReadIndex = 0; //tail leser av data p� index plass


	T* mBuffer; //dette er bufferet! dynamisk

	int NumOfElements = 0; //elementer i bufferet shared value
	int BufferSize = 0; //max size p� buffer shared value


	mutex lock; //felles l�s for begge funksjoner med bruk av unique lock.

	condition_variable FullCv; //condition variable for fult buffer
	condition_variable EmptyCv; //condition variable for tomt buffer



};

