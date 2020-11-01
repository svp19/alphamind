#ifndef ALPHA_OPP_H
#define ALPHA_OPP_H

#include<iostream>
#include<fstream>
#include<string>
#include"alphaRandom.h"
using namespace std ;

string filename_turns ;
string filename_first ;

class alphaOpp{

	public : 
		fstream foppTurns ; // Turns to guess mySecretNumber
		fstream foppFirst ; // Opp's firstGuess
		string id ;
		int mySecretNumber ;
		//below written to file
		int oppFirstGuess ;
		// parameters to learn
		bool firstGuessLearned  ; // two matches played, learned if random or constant
		bool firstGuessConstant ;
		bool alphaWinsAlways ;
		int  sevenGuessNumber ;


	public : 
				alphaOpp(){
					
					firstGuessLearned = false ;
					firstGuessConstant = false ;
					alphaWinsAlways = false ;
					id.reserve(2) ;
					filename_turns.reserve(37) ;
					sevenGuessNumber = 5981 ; //randomly 
				}
				
				alphaOpp(string oppid,int msn) : alphaOpp(){
					
					mySecretNumber = msn ;
					alphaWinsAlways = false ;
					this->id = oppid ;
					if(id.length() == 1)
						id = "0" + id ;
					// fstream fout("id.txt",ios::out) ;
					// fout << oppid << "here" ;
					// fout.close() ;
					// create files if not exist
					filename_turns = string("alpha_resources/opps/turns/") + id + string(".alpha_t") ;
					filename_first = string("alpha_resources/opps/firstguess/") + id + string(".alpha_f") ;
					foppTurns.open(filename_turns,ios::app) ;
					foppFirst.open(filename_first,ios::app) ;
					
					if( !foppTurns ){	// not exists, create it

						foppTurns.open(filename_turns,ios::out) ;
						foppTurns << "" ;
						foppTurns.close() ;
					}
					
					if ( !foppFirst ){	// not exists, not exists create it 

						foppFirst.open(filename_first,ios::out) ;
						foppFirst << "" ;
						foppFirst.close() ;
					}
				}
				
				void set_params(string oppid, int msn){
					mySecretNumber = msn ;
					alphaWinsAlways = false ;
					this->id = oppid ;
					
					// fstream fout("id.txt",ios::out) ;
					// fout << oppid << "here" ;
					// fout.close() ;
					// create files if not exist
					filename_turns = string("alpha_resources/opps/turns/") + id + string(".alpha_t") ;
					filename_first = string("alpha_resources/opps/firstguess/") + id + string(".alpha_f") ;
					foppTurns.open(filename_turns,ios::app) ;
					foppFirst.open(filename_first,ios::app) ;
					
					if( !foppTurns ){	// not exists, create it

						foppTurns.open(filename_turns,ios::out) ;
						foppTurns << "" ;
						foppTurns.close() ;
					}
					
					if ( !foppFirst ){	// not exists, not exists create it 

						foppFirst.open(filename_first,ios::out) ;
						foppFirst << "" ;
						foppFirst.close() ;
					}
				}

				// Generate best possible secret number
				int genOptimalSecretNumber(){

					if( alphaWinsAlways )
						return sevenGuessNumber ;
					else{
							if( firstGuessLearned && firstGuessConstant ){
								return 1029 ;
							}
					}
					return genRandomSecretNumber() ;
				}

};

bool setOpponentLearner(alphaOpp &alphaopp, string oppid, int mySecretNumber){

	fstream fin("alpha_resources/opps/" + oppid + ".alphaOpp", ios::in) ;
	if( fin ){	//exists
		
		int _oppFirstGuess ;
		fin >> _oppFirstGuess ;
		alphaopp.oppFirstGuess = _oppFirstGuess ;
		
		bool _firstGuessLearned  ; // two matches played, learned if random or constant
		fin >> _firstGuessLearned ;
		alphaopp.firstGuessLearned = _firstGuessLearned ;

		bool _firstGuessConstant ;
		fin >> _firstGuessConstant ;
		alphaopp.firstGuessConstant = _firstGuessConstant ;

		bool _alphaWinsAlways ;
		fin >> _alphaWinsAlways ;
		alphaopp.alphaWinsAlways = _alphaWinsAlways ;

		int  _sevenGuessNumber ;
		fin >> _sevenGuessNumber ;
		alphaopp.sevenGuessNumber = _sevenGuessNumber ;

		fin.close() ;
		return true ;
	}
	
}

void writeToOppFile(alphaOpp &alphaopp,string oppid){

	fstream fout("alpha_resources/opps/" + oppid + ".alphaOpp", ios::out | ios::trunc ) ;
	fout <<  alphaopp.oppFirstGuess << "\n" ;
	
	fout <<	alphaopp.firstGuessLearned << "\n" ;

	fout <<  alphaopp.firstGuessConstant <<"\n" ;

	fout <<	alphaopp.alphaWinsAlways << "\n" ;

	fout <<	alphaopp.sevenGuessNumber << "\n" ;

	fout.close() ;
	return ;
}


#endif
