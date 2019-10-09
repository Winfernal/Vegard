#include "pch.h"
#include "ringBuffer.h"


template<class T>
ringBuffer<T>::ringBuffer()
{
}

template<class T>
ringBuffer<T>::ringBuffer(int size) //lager ringbuffer p� heap med st�rrelsen man �nsker.
{
	mBuffer = new T[size];
	BufferSize = size;
	NumOfElements = 0; //Er nok ikke n�dvendig
	WriteIndex = 0; //Er nok ikke n�dvendig
	ReadIndex = 0; //Er nok ikke n�dvendig



}

template<class T>
ringBuffer<T>::~ringBuffer() //garbage collector for ringbufferet.
{
	delete[]mBuffer; //destrukt�r
}

template<class T>
bool ringBuffer<T>::notEmpty() //dersom man kommer p� den st�rste indexen p� bufferet, da er det fult.
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
	unique_lock<mutex> lck(lock); //l�ser slik at bare put kan bruke delt variabel med mutexen som argument.
	EmptyCv.wait(lck, [this]() {return notFull(); }); //condition variable med unique_lock som argument.

	mBuffer[WriteIndex] = val; //legg til verdi p� index plass val i buffer
	incWriteIndex(); //�ker index for write variabel og �ker index for numofelements variabel.

	lck.unlock(); //l�ser opp.

	FullCv.notify_one(); //Varsler full condition variablen, en ventende tr�d.

}

template<class T>
T ringBuffer<T>::get() //consumer
{

	unique_lock<mutex> lck(lock); //l�ser slik at get bare kan bruke delt variabel.

	FullCv.wait(lck, [this]() {return notEmpty(); }); //condition variable med unique_lock som argument.

	T consumed = mBuffer[ReadIndex]; //hent verdi p� index plass val i buffer
	incReadIndex(); //�ker index for read variabel og reduserer index for numofelements variabel.

	lck.unlock(); //l�ser opp.
	EmptyCv.notify_one(); //varsler empty condition variablen, en ventende tr�d.

	return consumed; //returnerer hva som ble hentet ut fra bufferet.

}


template<class T>
void ringBuffer<T>::printindex()
{


	cout << "numofelements: " << NumOfElements << endl; //dette har jeg brukt for � debuge, � se om bufferet har fungert som det skal.


}


template<class T>
void ringBuffer<T>::incWriteIndex()
{
	WriteIndex = (WriteIndex + 1) % BufferSize; //�ker write index.
	++NumOfElements; //�ker antall elementer i buffer.
}

template<class T>
void ringBuffer<T>::incReadIndex()
{
	ReadIndex = (ReadIndex + 1) % BufferSize; //�ker read index.
	--NumOfElements; //minsker antall elementer i buffer.
}
