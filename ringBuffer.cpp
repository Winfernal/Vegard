#include "pch.h"
#include "ringBuffer.h"


template<class T>
ringBuffer<T>::ringBuffer()
{
}

template<class T>
ringBuffer<T>::ringBuffer(int size) //lager ringbuffer på heap med størrelsen man ønsker.
{
	mBuffer = new T[size];
	BufferSize = size;
	NumOfElements = 0; //Er nok ikke nødvendig
	WriteIndex = 0; //Er nok ikke nødvendig
	ReadIndex = 0; //Er nok ikke nødvendig



}

template<class T>
ringBuffer<T>::~ringBuffer() //garbage collector for ringbufferet.
{
	delete[]mBuffer; //destruktør
}

template<class T>
bool ringBuffer<T>::notEmpty() //dersom man kommer på den største indexen på bufferet, da er det fult.
{
	return NumOfElements != 0; //hvis antall elementer ikke er null.
}

template<class T>
bool ringBuffer<T>::notFull()
{
	return NumOfElements != BufferSize; //hvis antall elementer ikke er buffersize.
}

template<class T>
bool ringBuffer<T>::full()
{
	return NumOfElements == BufferSize; //denne funksjonen blir egentlig ikke brukt
}

template<class T>
void ringBuffer<T>::put(T val) //producer 
{
	unique_lock<mutex> lck(lock); //låser slik at bare put kan bruke delt variabel med mutexen som argument.
	EmptyCv.wait(lck, [this]() {return notFull(); }); //condition variable med unique_lock som argument.

	mBuffer[WriteIndex] = val; //legg til verdi på index plass val i buffer
	incWriteIndex(); //øker index for write variabel og øker index for numofelements variabel.

	lck.unlock(); //låser opp.

	FullCv.notify_one(); //Varsler full condition variablen, en ventende tråd.

}

template<class T>
T ringBuffer<T>::get() //consumer
{

	unique_lock<mutex> lck(lock); //låser slik at get bare kan bruke delt variabel.

	FullCv.wait(lck, [this]() {return notEmpty(); }); //condition variable med unique_lock som argument.

	T consumed = mBuffer[ReadIndex]; //hent verdi på index plass val i buffer
	incReadIndex(); //øker index for read variabel og reduserer index for numofelements variabel.

	lck.unlock(); //låser opp.
	EmptyCv.notify_one(); //varsler empty condition variablen, en ventende tråd.

	return consumed; //returnerer hva som ble hentet ut fra bufferet.

}


template<class T>
void ringBuffer<T>::printindex()
{


	cout << "numofelements: " << NumOfElements << endl; //dette har jeg brukt for å debuge, å se om bufferet har fungert som det skal.


}


template<class T>
void ringBuffer<T>::incWriteIndex()
{
	WriteIndex = (WriteIndex + 1) % BufferSize; //øker write index.
	++NumOfElements; //øker antall elementer i buffer.
}

template<class T>
void ringBuffer<T>::incReadIndex()
{
	ReadIndex = (ReadIndex + 1) % BufferSize; //øker read index.
	--NumOfElements; //minsker antall elementer i buffer.
}
