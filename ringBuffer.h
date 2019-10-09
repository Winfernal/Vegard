#pragma once
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "../../std_lib_facilities.h"

template <class T> //template for å utnytte polymorfisme
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
	T get(); //få tak i verdi i buffer //consumer
	void printindex(); //debuging





private:

	void incWriteIndex(); //øke plass på head(write) i bufferet
	void incReadIndex(); //øke plass på tail(read) i bufferet
	int WriteIndex = 0; //head skriver inn data på index plass 
	int ReadIndex = 0; //tail leser av data på index plass


	T* mBuffer; //dette er bufferet! dynamisk

	int NumOfElements = 0; //elementer i bufferet shared value
	int BufferSize = 0; //max size på buffer shared value


	mutex lock; //felles lås for begge funksjoner med bruk av unique lock.

	condition_variable FullCv; //condition variable for fult buffer
	condition_variable EmptyCv; //condition variable for tomt buffer



};

