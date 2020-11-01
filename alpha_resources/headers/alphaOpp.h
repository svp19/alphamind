#ifndef ALPHA_OPP_H
#define ALPHA_OPP_H

#include<iostream>
#include<fstream>
#include<string>
#include"alphaRandom.h"
using namespace std ;

string filename_turns ;
string filename_first ;
string filename_secnum ;

vector<char> itovc2(int num){

	// Convert int to vector<char>
	vector<char> n;
	n.push_back( (num/1000)+'0' ) ;
	n.push_back( ((num/100) % 10) +'0') ;
	n.push_back( ((num/10) % 10) +'0') ;
	n.push_back( ((num) % 10) +'0') ;
	return n ;
}

class alphaOpp{

	public : 
		fstream foppTurns ; // Turns to guess mySecretNumber
		fstream foppFirst ; // Opp's firstGuess
		fstream foppSecret ;
		string id ;
		int mySecretNumber ;
		//below written to file
		int oppFirstGuess ;
		// parameters to learn
		bool firstGuessLearned  ; // two matches played, learned if random or constant
		bool firstGuessConstant ;
		bool secretNumberConstant ;
		bool alphaWinsAlways ;
		int  sevenGuessNumber ;


	public : 
				alphaOpp(){
					
					firstGuessLearned = false ;
					firstGuessConstant = false ;
					secretNumberConstant = false ;
					alphaWinsAlways = false ;
					id.reserve(2) ;
					filename_turns.reserve(37) ;
					sevenGuessNumber = 5981 ; //randomly 
				}
				
				alphaOpp(string oppid,int msn) : alphaOpp(){
					
					mySecretNumber = msn ;
					alphaWinsAlways = false ;
					this->id = oppid ;
					// create files if not exist
					filename_turns = string("alpha_resources/opps/turns/") + id + string(".alpha_t") ;
					filename_first = string("alpha_resources/opps/firstguess/") + id + string(".alpha_f") ;
					filename_secnum = string("alpha_resources/opps/secretnumber/") + id + string(".alpha_s") ;
					foppTurns.open(filename_turns,ios::app) ;
					foppFirst.open(filename_first,ios::app) ;
					foppSecret.open(filename_secnum,ios::app) ;
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
					
					if ( !foppSecret ){	// not exists, not exists create it 

						foppSecret.open(filename_first,ios::out) ;
						foppSecret << "" ;
						foppSecret.close() ;
					}
				}
				
				void set_params(string oppid, int msn){
					mySecretNumber = msn ;
					alphaWinsAlways = false ;
					this->id = oppid ;
					
					// create files if not exist
					filename_turns = string("alpha_resources/opps/turns/") + id + string(".alpha_t") ;
					filename_first = string("alpha_resources/opps/firstguess/") + id + string(".alpha_f") ;
					filename_secnum = string("alpha_resources/opps/secretnumber/") + id + string(".alpha_s") ;
					foppTurns.open(filename_turns,ios::app) ;
					foppFirst.open(filename_first,ios::app) ;
					foppSecret.open(filename_secnum,ios::app) ;
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
					
					if ( !foppSecret ){	// not exists, not exists create it 

						foppSecret.open(filename_first,ios::out) ;
						foppSecret << "" ;
						foppSecret.close() ;
					}
				}

				// Generate best possible secret number
				int genOptimalSecretNumber(){

					if( alphaWinsAlways )
						return sevenGuessNumber ;
					else{
							if( firstGuessLearned && firstGuessConstant ){// Still unsure whether to play our minimax sevenGuess
								return genRandomSecretNumber() ;
							}
					}
					return genRandomSecretNumber() ;
				}
				
				vector<char> genOptimalFirstGuess(string oppid){
					
					filename_secnum = string("alpha_resources/opps/secretnumber/") + oppid + string(".alpha_s") ;
					if( secretNumberConstant ){
						foppSecret.open(filename_secnum,ios::in) ;
						
						int secnum ;
						if(foppSecret >> secnum){
							
							foppSecret.close() ;
							
							vector<char> firstguess = itovc2(secnum) ;
							return firstguess ;
						}
						foppSecret.close(); 
					}
					
					return genRandomFirstGuess() ;
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
		
		bool _secretNumberConstant ;
		fin >> _secretNumberConstant ;
		alphaopp.secretNumberConstant = _secretNumberConstant ;

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
	
	fout << alphaopp.secretNumberConstant << "\n" ;

	fout.close() ;
	return ;
}

bool isfirstGuessConstant(string oppid){

	fstream foppFirst ;
	filename_first = string("alpha_resources/opps/firstguess/") + oppid + string(".alpha_f") ;					
	foppFirst.open(filename_first, ios::in) ;
//	fstream fout("alphalog.txt",ios::app) ;
	int oldGuess, newGuess ;
	if( foppFirst ){
		
		foppFirst >> oldGuess ;
//		fout << oldGuess ;
		while( foppFirst >> newGuess ){
			
//			fout << newGuess ;
			if( oldGuess != newGuess ){
				foppFirst.close() ;
//				fout.close() ;
				return false ;
			}
			oldGuess = newGuess ;
		}
//		fout.close() ;
		foppFirst.close() ;
		return true ;
	}
	return false ;
}

bool issecretNumberConstant(string oppid){

	fstream foppSecret ;
	filename_secnum = string("alpha_resources/opps/secretnumber/") + oppid + string(".alpha_s") ;					
	foppSecret.open(filename_secnum, ios::in) ;
//	fstream fout("alphalog.txt",ios::app) ;
	int oldNum, newNum ;
	if( foppSecret ){
		
		foppSecret >> oldNum ;
//		fout << oldGuess ;
		while( foppSecret >> newNum ){
			
//			fout << newGuess ;
			if( oldNum != newNum ){
				foppSecret.close() ;
//				fout.close() ;
				return false ;
			}
			oldNum = newNum ;
		}
//		fout.close() ;
		foppSecret.close() ;
		return true ;
	}
	return false ;
}
void update_file(alphaOpp &alphaopp, int oppGuess, int turns, string oppid, int mySecretNumber){

	string filename_turns = string("alpha_resources/opps/turns/") + oppid + string(".alpha_t") ;
	string filename_first = string("alpha_resources/opps/firstguess/") + oppid + string(".alpha_f") ;
	string filename_secnum = string("alpha_resources/opps/secretnumber/") + oppid + string(".alpha_s") ;					
	fstream foppTurns ;
	foppTurns.open(filename_turns, ios::out | ios::app ) ;
	foppTurns << oppGuess << " " << turns << "\n" ;
	foppTurns.close() ;
	
	//first guess
	if(turns == 1){

		fstream foppFirst ;
		foppFirst.open(filename_first, ios::app) ;
		foppFirst << oppGuess << "\n" ;
		foppFirst.close() ;
		
		// Check firstGuess constant
		if( isfirstGuessConstant(oppid) ){
			
//			fstream fout("peace.txt",ios::out) ;
//			fout << "peace" ;
//			fout.close();
			alphaopp.firstGuessConstant = true ;
			alphaopp.firstGuessLearned = true ;
			alphaopp.oppFirstGuess = oppGuess ;
		}
		else
			alphaopp.firstGuessConstant = false ;
	
	}

	//Check for winning move					
	if( turns == 7 ){
		if( alphaopp.firstGuessLearned && alphaopp.firstGuessConstant ){
			alphaopp.alphaWinsAlways = true ;
			alphaopp.sevenGuessNumber = mySecretNumber ;
		}
	}
	
	//Check secnum constant
	if( issecretNumberConstant(oppid) ){
		alphaopp.secretNumberConstant = true ;
		
	}
	else
		alphaopp.secretNumberConstant = false ;

}


#endif
