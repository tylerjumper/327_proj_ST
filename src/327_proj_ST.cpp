// smalltalk_VirtualAbstractVector.cpp : Defines the entry point for the console application.
//
//fffffff
#include <iostream>
#include <stdlib.h>
#include <memory>
#include <vector>
#include <time.h>
//for delays
#include <chrono>
#include <thread>

#include "../../327_proj_ST_lib/includes/Smalltalk_American.h"
#include "../../327_proj_ST_lib/includes/ST_American_DonutEnthusiest.h"
#include "../../327_proj_ST_lib/includes/Smalltalk_Brit.h"
#include "../../327_proj_ST_lib/includes/Watch.h"
#include "../../327_proj_ST_lib/includes/constants.h"
#include "../../327_proj_ST_lib/includes/Functions.h"

using namespace std;

//tracks how many points you will get
//yeah, yeah its a global
int total_points =25;

//this is a template class, its mostly here as a helper for me
//the T and U are generic params, I can substitute any type for them
//they must be comparable with ==, templates are extremely hard to get right BTW
template<typename T, typename U>
bool EXPECT_EQ(T expectedVal, U actualVal,string testnumb = "", int pts=1){
	bool bout = (expectedVal == actualVal);
	try{
		if (bout){
			total_points+=pts;
			cout<<"SUCCESS "+testnumb<<" points:"<<total_points;
		}
		else
			cout<<"FAIL "+ testnumb;
		cout<<endl;
	}catch(...)
	{
		std::cout<<"THREW EXCEPTION IN EXPECT_EQ"<<std::endl;
	}
	return bout;
}
bool EXPECT_CONTAINS(string expectedVal, string actualVal,string testnumb = "", int pts=1, bool bVerbose = false){
	bool bout =(actualVal.find(expectedVal) != string::npos );
	if (bout){
		total_points+=pts;
		cout<<"SUCCESS "+testnumb<<" points:"<<total_points;
	}
	else
		cout<<"FAIL "+ testnumb;
	if(!bout && bVerbose)
		cout<<" Expected contains:"<<expectedVal<<" received:"<<actualVal;
	cout<<endl;
	return bout;
}
void demo(){
	int numEachGroup = 2;
		int numwatches = 2;

		std::vector<std::unique_ptr<Smalltalk> > myv1 = getPeople(numEachGroup,
				numEachGroup, numEachGroup, numwatches);
		for (int i = 0; i < myv1.size(); i++) {
			std::cout << myv1[i]->saySomething() << endl;
			std::cout << myv1[i]->getTime() << endl;
		}
}

void testRandomRun(int numEachGroup, int numwatches, bool saySomething=true, bool getTime = true) {
	std::vector<std::unique_ptr<Smalltalk> > myv1 = getPeople(numEachGroup,
			numEachGroup, numEachGroup, numwatches);
	for (int j = 0; j < myv1.size(); j++) {
		int i = rand() % myv1.size();
		if (saySomething)
			std::cout<< myv1[i]->saySomething()<<endl;
		if(getTime)
			std::cout<< myv1[i]->getTime()<<endl;

		//snooze for a bit, get at least 1 ms and yield the time slice to boot
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

void testDeterministicRun(int numEachGroup, int numwatches, bool saySomething=true, bool getTime = true) {
	std::vector<std::unique_ptr<Smalltalk> > myv1 = getPeople(numEachGroup,
			numEachGroup, numEachGroup, numwatches);
	for (int j = 0; j < myv1.size(); j++) {
		if (saySomething)
			std::cout<< myv1[j]->saySomething()<<endl;
		if(getTime)
			std::cout<< myv1[j]->getTime()<<endl;

		//snooze for a bit, get at least 1 ms and yield the time slice to boot
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

/**
 *
 *verifies the time from this smalltalker
 *takes the watch from the smalltalker
 * verifies watch still works
 * verifiessmalltalker says I dont have a watch
 * tries to take the watch from a watchless smalltal
 *INPUTS
 *		st - a watchless smalltalker
 *		pWatch - a watch
 *	RETURNS
 * 	st - same watchless smalltalker
 *		pWatch - same watch
 *st
 */
void testGiveAndTakeWatch(Smalltalk *st, unique_ptr<Watch> & pWatch) {
	unique_ptr<Watch> pTakenWatch;

	//gives a watch to a watchless smalltalker
	//verifies the time from this smalltalker
	st->giveWatch(pWatch);
	EXPECT_CONTAINS(THE_CURRENT_TIME_IS, st->getTime(), "4 - gettime()", 2,
			true);

	//take the watch from the smalltalker, verifies watch works, verifies smallter says I_DO_NOT_HAVE_A_WATCH
	pTakenWatch = st->takeWatch();
	EXPECT_CONTAINS(THE_CURRENT_TIME_IS, pTakenWatch->getTime(), "4 - gettime()", 1,	true);
	EXPECT_CONTAINS(I_DO_NOT_HAVE_A_WATCH, st->getTime(), "4 - gettime()", 2,
			true);

	//try to take watch from the smalltalker again should return null
	unique_ptr<Watch> pSecondTaking_pWatch;
	pSecondTaking_pWatch = st->takeWatch();
	bool bval = (pSecondTaking_pWatch == 0);
	EXPECT_EQ(bval, true,
			" verify that taking a watch twice results in NULL", 2);

	//return watch in orig pointer, and the orig small talker should have no watch
	pWatch=move(pTakenWatch);
}

//test creating and moving a watch from smalltalker to smalltalker
void testWatchMove(){
	cout <<endl<< "testWatchMove with just 1 watch, giving and taking it from all 3 derived classes"<<endl;
	total_points +=1;	//makes this section count as 25

	//lets create a watch and some people, make sure we can give and take the watch properly
	Smalltalk_Brit aBrit;
	Smalltalk_American aAmerican;
	ST_American_DonutEnthusiest aAmericanDonutEnthusiest;

	//create a watch
	std::unique_ptr<Watch >pWatch(new Watch());

	try{
		//verify no watches
		EXPECT_CONTAINS(I_DO_NOT_HAVE_A_WATCH, aBrit.getTime(),"4 - gettime()",1,true);
		EXPECT_CONTAINS(I_DO_NOT_HAVE_A_WATCH, aAmerican.getTime(),"4 - gettime()",1,true);
		EXPECT_CONTAINS(I_DO_NOT_HAVE_A_WATCH, aAmericanDonutEnthusiest.getTime(),"4 - gettime()",1,true);
	}
		catch(...){
			cout<<"Threw exception in testWatchMove:EXPECT_CONTAINS"<<endl;
	}

	try{
		testGiveAndTakeWatch(&aBrit, pWatch);
	}catch(...){
					cout<<"Threw exception in testGiveAndTakeWatch(&aBrit, pWatch)"<<endl;
	}

	try{
	testGiveAndTakeWatch(&aAmerican, pWatch);
	}catch(...){
						cout<<"Threw exception in testGiveAndTakeWatch(&aBrit, pWatch)"<<endl;
	}

	try{
		testGiveAndTakeWatch(&aAmericanDonutEnthusiest, pWatch);
	}catch(...){
						cout<<"Threw exception in testGiveAndTakeWatch(&aBrit, pWatch)"<<endl;
		}
}


void test_getPeople(int numEachGroup, int numwatches) {
	cout <<endl<< "test_getPeople" << " numEachGroup=" <<numEachGroup<<" numWatches="<<numwatches<<endl;

	//create vector of people
	std::vector<std::unique_ptr<Smalltalk> > myv1 = getPeople(numEachGroup,
			numEachGroup, numEachGroup, numwatches);
	int myv1_size = myv1.size();
	//go up to size of array or numb watches, whichever is smallest
	int max_watches_given = (myv1_size<=numwatches)?myv1_size:numwatches;

	//right number of people
	EXPECT_EQ(numEachGroup*3, myv1_size,"1  -right number of people",2);

	//right number of watches
	int inumbWatches=0;
	for (int i = 0; i < myv1_size; i++) {
		string myval = myv1[i]->getTime();

		if (myval.find(I_DO_NOT_HAVE_A_WATCH) == string::npos )
			inumbWatches++;
	}
	EXPECT_EQ(max_watches_given, inumbWatches,"2 - right number of watches",2);

	//right watch order
	inumbWatches=0;

	for (int i = 0; i < max_watches_given; i++) {
		string myval = myv1[i]->getTime();

		if (myval.find(I_DO_NOT_HAVE_A_WATCH) == string::npos )
			inumbWatches++;
	}
	EXPECT_EQ(max_watches_given, inumbWatches,"3 - right watch order",2);
}

enum nationality{ST_A,ST_A_DE,ST_B};
void testPhrasing(nationality mn, Smalltalk *ptalk) {
	if (!ptalk)
		return;

	switch(mn){
	case ST_A:
		try{
			cout <<endl<< "test saySomething for American" <<endl;
			EXPECT_CONTAINS(AMERICAN_PHRASE_1, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(AMERICAN_PHRASE_2, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(AMERICAN_PHRASE_3, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(AMERICAN_PHRASE_4, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(AMERICAN_PHRASE_5, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(AMERICAN_PHRASE_1, ptalk->saySomething(),"4 - phrasing",1,true);
		}
		catch(...){
				cout<<"Threw exception in testPhrasing:ST_A"<<endl;
		}
		break;
	case ST_A_DE:
		try{
			cout <<endl<< "test saySomething for American Donut Enthusiest" <<endl;
			EXPECT_CONTAINS(AMERICAN_PHRASE_1, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(AMERICAN_PHRASE_2, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(AMERICAN_PHRASE_3, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(AMERICAN_PHRASE_4, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(AMERICAN_PHRASE_5, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(AMERICAN_DE_PHRASE_1, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(AMERICAN_DE_PHRASE_2, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(AMERICAN_DE_PHRASE_3, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(AMERICAN_DE_PHRASE_4, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(AMERICAN_DE_PHRASE_5, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(AMERICAN_PHRASE_1, ptalk->saySomething(),"4 - phrasing",1,true);
		}
			catch(...){
				cout<<"Threw exception in testPhrasing:ST_A_DE"<<endl;
			}
		break;

	case ST_B:
		try{
			cout <<endl<< "test saySomething for Brit" <<endl;
			EXPECT_CONTAINS(BRIT_1, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(BRIT_2, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(BRIT_3, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(BRIT_4, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(BRIT_5, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(BRIT_6, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(BRIT_7, ptalk->saySomething(),"4 - phrasing",1,true);
			EXPECT_CONTAINS(BRIT_1, ptalk->saySomething(),"4 - phrasing",1,true);
		}
		catch(...){
				cout<<"Threw exception in testPhrasing:ST_B"<<endl;
		}
		break;
	}
}

//enum nationality{ST_A,ST_A_DE,ST_B};
int main() {

//	test getPeople
	test_getPeople(0,0);	//test with 0  people 0 watches
	test_getPeople(4,1);	//3*4 =		12 people 1  watches
	test_getPeople(4,10);	//3*4 =		12 people 12 watches
	test_getPeople(4,20);	//3*4 =		12 people 20 watches
	total_points +=1;		//free point

	//test order of phrasing
	Smalltalk_American sta;
	testPhrasing(ST_A,&sta);	//run through utterances, make sure they
								//iterate in the right order and correctly
								//restart at beginning once all phrases uttered

	ST_American_DonutEnthusiest stade;
	testPhrasing(ST_A_DE,&stade);	//same as above but will go through both
									//ST_A then ST_DE utterances
									//before it restarts at beginning
	Smalltalk_Brit stb;
	testPhrasing(ST_B,&stb);

	//testWatchMove with just 1 watch, giving and taking it from all 3 derived
	//classes.Verify correct pointer movement, verify derived class responds
	//appropriately when given a watch and when watch is taken, (and when watch taken twice)
	testWatchMove();
	return 0;
}
