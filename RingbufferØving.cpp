// RingbufferØving.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Ringbuffe kode til oppgaven vi fikk på andreåret dataingeniør, om parallelitet og mutexer. Mye av koden er inspirert av andre sin kode, der jeg har
//endret det til en siste versjon jeg mener fungerer best. 

#include "pch.h"
#include <iostream>
#include <thread>
#include "ringBuffer.cpp"
#include "../../std_lib_facilities.h"


void writer(ringBuffer<char>& buffer); //producer
void reader(ringBuffer<char>& buffer); //consumer
void keyboardInput(ringBuffer<char>& buffer); //producer med input



int main()
{
	ringBuffer<char> ringBuffer(5); //pappa

	thread writerThread(&writer, ref(ringBuffer)); //sønn 
	thread keyboardInputThread(&keyboardInput, ref(ringBuffer)); //sønn
	thread readerThread(&reader, ref(ringBuffer)); //sønn




	keyboardInputThread.join(); //start keyboard input tråd
	writerThread.join(); //start writer tråd.
	readerThread.join(); //start lesetråd.



	return 0; //all is well!

};



void writer(ringBuffer<char>& buffer) //skriver til bufferet med put. Valgfri string.
{

	string s = "0123456789";
	for (char c : s)
	{
		buffer.put(c);
		this_thread::sleep_for(chrono::milliseconds(200));
		//cout << "produced: " << c << '\t'; //produced er der for debug og visualisering
		//buffer.printindex(); //debug
		
	}

};

void keyboardInput(ringBuffer<char>& buffer) //skriver til bufferet med input fra bruker. 
{
	string s;
	do
	{
		cin >> s;

		{
			for (char c : s)
			{
				buffer.put(c);
				//this_thread::sleep_for(chrono::milliseconds(100)); //Tror ikke dette trengs i keyboard input
				//cout << "produced: " <<  c; //produced er der for visualisering og debug
				
			}

		}

	} while (s != "EXIT");
};


void reader(ringBuffer<char>& buffer) //leser fra bufferet med get
{

	//while ((buffer.notFull())) //kjør imens bufferet ikke er fylt opp.
		while ((true))
	{
		char tmp;

		tmp = buffer.get();
		this_thread::sleep_for(chrono::milliseconds(1000));

		//cout << "consumed: " << tmp << endl; //consumed er der for visualisering og debug
		cout << tmp; 
		//buffer.printindex(); //debug

	

	}
};





