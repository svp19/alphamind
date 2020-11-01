#ifndef ALPHA_RANDOM_H
#define ALPHA_RANDOM_H

#include<fstream>
#include<sstream>	// for number to string
#include<stdlib.h>	// for random number
#include<time.h> 	// seed random
#include<vector>

using namespace std ;

int genRandomSecretNumber(bool firstGuess=false){

	fstream fin("alpha_resources/uniqnum.rapd",ios::in) ;
	
	//Deepak H R | True Randomization
	srand(time(0));
	int u = 5,t = rand()%10;
	u = rand() ;
	while(t--){
		if( firstGuess == true )
			break ;
		u = (rand()%(rand()+108))*(rand()*rand()%(rand()+19));
	}
	//
	int randindex = u % 4536 ;
	int randSecretNumber ;
	for(long i=0; i <= randindex ; i++ && !fin.eof()){
		fin >> randSecretNumber ;
	}
	if( randSecretNumber == 1023 || randSecretNumber == 1234 || randSecretNumber == 9876 || randSecretNumber == 4581 ){
		return genRandomSecretNumber() ;
	}
	if( randSecretNumber <= 1023 || randSecretNumber >= 9876 )
		return 5931 ;
		
	return randSecretNumber ;
}

vector<char> genRandomFirstGuess(){

	int firstGuess = genRandomSecretNumber(true) ;
	vector<char> next_guess ;
	next_guess.push_back( (firstGuess/1000) + '0' ) ;
	next_guess.push_back( ((firstGuess/100) % 10) + '0' ) ;
	next_guess.push_back( ((firstGuess/10) % 10) + '0' ) ;
	next_guess.push_back( ((firstGuess) % 10) + '0' ) ;
	return next_guess ;
}
#endif
