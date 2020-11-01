#ifndef ALPHA_MIND_H
#define ALPHA_MIND_H

#include<iostream>
#include<fstream>
#include<vector>
#include<set>
#define vc vector<char>
#define set_iter set<vector<char>>::iterator
using namespace std ;

struct Pair{
	
	int bulls ;
	int cows ;
	//constructor	
	Pair(){
		bulls = 0 ;
		cows = 0 ;
	}
	//overload == 
	bool operator==(const Pair second){
		return ((this->bulls == second.bulls) && (this->cows == second.cows)) ;
	}
};

class MasterMind{

	public :
	set<vc> all_n ;
	set<vc> my_n ;

	public : 

		MasterMind(){

			//make all_n and my_n
			for(int i=1000; i<10000 ; i++){
				
				vc num ; 
				num.push_back(	((i/1000)%10) + '0') ;
				num.push_back(	((i/100)%10) + '0') ;
				num.push_back(	((i/10)%10) + '0') ;
				num.push_back(	((i)%10) + '0') ;
				all_n.insert(num) ;
				
				bool unique_digits = true ;
				for(int p1=0; p1<4 ; p1++){
					for(int p2=p1+1 ; p2<4 ; p2++){
						
						if( num[p1] == num[p2] ){
							unique_digits = false ;
							break ;
						}
					}
				}

				if( unique_digits )
					my_n.insert(num) ;
			}
		}

		//// SIMULATE /////
//		void simulate(){
//
//			//start with a number, say '1122'
//			vc first_n = { '1','2','3','4' };
//			my_n = playGuess(first_n) ;
//
//			//Play using Knuth's Minimax until one number left 
//			while(my_n.size() > 1){
//				
//				vc next_guess = minimax(my_n) ;
//				my_n = playGuess(next_guess) ;
//			}
//			
//			if(my_n.size() == 1){
//				set_iter ans = my_n.begin() ;
//				for(auto i : *(ans))
//					cout<<i ;
//				cout<<"\n" ;
//			}
//		}
	
	public :

		////////////////////////////////////////////////////////////////////////////////
		/* 							Updates set of Possible Guesses					  */
		////////////////////////////////////////////////////////////////////////////////						
		set<vc> playGuess(vc guess, string oppid){

			set<vc> new_n ;
			
			//Print Guess Played
			for(char i : guess)
				cout << i ;
			cout << "\n" ;
			
			//Get Score
			Pair score ;
			cin >> score.bulls >> score.cows ;
			
			//Guess is Correct => return
			if(score.bulls == 4){
				//new_n.insert(guess) ;
				
				//write secnum to oppfile
				string filename_secnum = string("alpha_resources/opps/secretnumber/") + oppid + string(".alpha_s") ;
				fstream fs(filename_secnum, ios::app) ;
				for(char i : guess)
					fs << i ;
				fs << "\n" ;
				fs.close() ;
				
				//return
				return {} ;
			}

			//set digits used in guess
			bool used[10] ;
			for(int i=0 ; i<10 ; i++)
				used[i] = false ;
			for(int i=0 ; i<4 ; i++)
				used[guess[i]-'0'] = true ;

			//Remove from set my_n all numbers that cannot reproduce same score ;
			for(vc n : my_n){
				
				Pair score_n ;
				for(int i=0; i<4 ; i++){
					
					if(used[n[i]-'0']){
						score_n.cows ++ ;
					}
					if(n[i] == guess[i])
						score_n.bulls ++ ;	
				}
				score_n.cows -= score_n.bulls ;
				if(score_n == score){
					new_n.insert(n) ;
				}
				
			}
			return new_n ;
		}

		Pair Score(int orignum, int oppguess){
			
			
			vc guess ;
			guess.push_back( (oppguess/1000) ) ;
			guess.push_back( ((oppguess/100) % 10)) ;
			guess.push_back( ((oppguess/10) % 10) ) ;
			guess.push_back( ((oppguess) % 10) ) ;
			
			//set digits used in guess
			bool used[10] ;
			for(int i=0 ; i<10 ; i++)
				used[i] = false ;
			for(int i=0 ; i<4 ; i++)
				used[guess[i]] = true ;

			//conv orignum to vc
			vc orig ;
			orig.push_back( (orignum/1000) ) ;
			orig.push_back( ((orignum/100) % 10)  ) ;
			orig.push_back( ((orignum/10) % 10)  ) ;
			orig.push_back( ((orignum) % 10) ) ;
			

			Pair score ;
			for(int i=0; i<4 ; i++){
				
				if(used[orig[i]]){
					score.cows ++ ;
				}
				if(orig[i] == guess[i])
					score.bulls ++ ;	
			}
			score.cows -= score.bulls ;
			return score ;
		}
		///////////////////////////////////////////////////////////////////////
		/*					 		MINIMAX  								 */
		///////////////////////////////////////////////////////////////////////
		vc minimax(set<vc> my_n){

			/*																						
				->	14 possible score combos triangularly stored in a 5x5 array for each n in my_n
				->	Max of this score chosen
				->  Minimum of all (Max Scores for each n) chosen and corresponding n returned
																								*/
			vc next_guess ; 
			int minofmax = INT_MAX ;
			for(vc na : my_n){

				int scoreboard[5][5] ;
				for(int i=0;i<5;i++)
					for(int j=0;j<5;j++)
						scoreboard[i][j] = 0 ;

				bool used[10] ;
				for(int i=0; i<10 ;i++)
					used[i] = false ;
				for(int i=0 ; i<4 ; i++)
					used[na[i]-'0'] = true ;

				for(vc nm : my_n){

					Pair score ; 
					for(int i=0 ; i<4 ;i++){
						if(used[nm[i]-'0'])
							++ score.cows ;
						if(nm[i] == na[i])
							++ score.bulls ;
					}
					score.cows -= score.bulls ;
					scoreboard[score.bulls][score.cows] ++ ;
				}

				/*/Print Board
				for(int i=0;i<5;i++){
					for(int j=0;j<5;j++)
						cout << scoreboard[i][j] << " " ;
					cout << "\n" ;
				}*/

				int highscore = -1 ;
				//->Choosing max score
				for(int i=0 ; i<5 ;i++){
					for(int j=0 ; j<5 ; j++){
						highscore = max(scoreboard[i][j],highscore) ;
					}
				}
				//->Choosing MinofMax
				if( highscore < minofmax ){
					minofmax = highscore ;
					next_guess = na ;
				}	
				
			} 

			return next_guess ;
		}

		void guessOppNum(vc &next_guess, string oppid){

			this->my_n = playGuess(next_guess, oppid) ;
			next_guess = minimax(my_n) ;
			return ;
		}

		void scoreOppGuess(int mySecretNumber, int oppnum){

			
			Pair score = Score(mySecretNumber,oppnum) ;
			cout << score.bulls << "\n" << score.cows <<"\n" ;
		}
		////////////////// END ///////////////////////////////////////

};

//helper  functions

vc itovc(int num){

	// Convert int to vector<char>
	vc n;
	n.push_back( (num/1000)+'0' ) ;
	n.push_back( ((num/100) % 10) +'0') ;
	n.push_back( ((num/10) % 10) +'0') ;
	n.push_back( ((num) % 10) +'0') ;
	return n ;
}

#endif
