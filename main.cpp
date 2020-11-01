 /* Inspired from Knuth's Minimax Solution , 
the algorithm is extended for the Bulls and Cows Game */

#include<iostream>
#include<fstream>
#include<string>
#include"alpha_resources/headers/alphaMind.h" 
#include"alpha_resources/headers/alphaRandom.h"
#include"alpha_resources/headers/alphaOpp.h"
using namespace std ;

int main(){

	//Initialize MyPlayer,startFlag and other variables.
	MasterMind MM ;
	int startFlag, oppGuess, oppNumGuess,turns = 0 ;
	bool gotOppGuessFirst = true ;
	
	/*	_______________________
		STEP 1 => Read Opp. Id
								*/
	string oppid ;
	cin >> oppid ;

	/*	______________________________
		STEP 2 => Print mySecretNumber
	
										*/
	//instantiate Alpha Opponent Learner
	int mySecretNumber = genRandomSecretNumber() ;
	alphaOpp alphaopp;
	
	if( !setOpponentLearner(alphaopp, oppid, mySecretNumber) ) 
		alphaopp.set_params(oppid, mySecretNumber) ;
	
	else
	{
		//played opp before, stats already loaded
		;
	}
	
	mySecretNumber = alphaopp.genOptimalSecretNumber() ;
	//print number
	cout << mySecretNumber << "\n" ;
	
 

	//Randomly initialize my first guess , like '1234'
	vc next_guess = alphaopp.genOptimalFirstGuess(oppid);	
	
	/*	________________________________________
		STEP 3 => Read startFlag
		if 1 , then guess first, score opp later
		if 0 , then score opp, guess later
												*/
	cin >> startFlag ;
	if(startFlag == 1){

		// 1. Guess opp's number first
		MM.guessOppNum(next_guess, oppid) ;
		gotOppGuessFirst = false ;
	}
	
	turns = 1 ;
	while(true){


		// 1. Score opp's guess
		cin >> oppGuess ;
		MM.scoreOppGuess(mySecretNumber, oppGuess) ;


		//update opp turns file
		update_file(alphaopp, oppGuess, turns, oppid, mySecretNumber) ;
		writeToOppFile(alphaopp, oppid) ;

		//if got oppfirstGuess, play as our firstGuess
		if( gotOppGuessFirst && !alphaopp.secretNumberConstant){
			next_guess = itovc(oppGuess) ;
			gotOppGuessFirst = false ;
		}


		// 2. Guess opp's number
		MM.guessOppNum(next_guess, oppid) ;
		turns ++ ;	
	}
	return 0 ;
}


