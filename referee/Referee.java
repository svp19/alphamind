package com.java.evaluator;

import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Referee {
	Player player1;
	Player player2;
	
	Referee(Player player1, Player player2){
		this.player1 = player1;
		this.player2 = player2;
	}
	
	/** Simulates a match between two players. */
	void beginMatch() throws Exception {
		
		// Assign player ids : Player with id 1 starts first.
		
		player1.assignPlayerID("1\n");
		player2.assignPlayerID("0\n");
		 
		 //Start the game
		
		int nRounds = 0;
		
		 while(true) {
			 
			 ++nRounds;
			 
			 // Player 1 asks first
			 
			 String p1Guess = player1.guess();
			 System.out.print("Player 1 asks:" + p1Guess);
			 player2.receive(p1Guess);
			 
			 // Player 2 responds with number of bulls and cows
			 
			 String p2Response = player2.getResponse(p1Guess);
			 
			 // Send the response to player 1
			 
			 player1.receive(p2Response);
			 
			 // Player 2 asks next
			 
			 String p2Guess = player2.guess();
			 System.out.print("Player 2 asks:" + p2Guess);
			 player1.receive(p2Guess);
			 
			 // Player 1 responds with number of bulls and cows
			 
			 String p1Response = player1.getResponse(p2Guess);
			 
			 // Send the response to player 2
			 
			 player2.receive(p1Response);
			 
			 // Check if anybody has won
			 
			 if(nRounds >= 500) {
				 System.out.println("Draw: Both players are unable to guess secret number.");
				 break;
			 }
			 
			 if(p1Guess.equals(player2.getSecretNumber()+"\n") && 
					 p2Guess.equals(player1.getSecretNumber()+"\n"))
			 {
				 System.out.println("Draw");
				 break;
			 }
			 else if(p1Guess.equals(player2.getSecretNumber()+"\n"))
			 {
				 System.out.println("Player 1 Won");
				 break;
			 }
			 else if(p2Guess.equals(player1.getSecretNumber()+"\n"))
			 {
				 System.out.println("Player 2 Won");
				 break;
			 }
		 }
		 System.out.println("Game finished in: "+ nRounds + " round(s).");
	}
	
	public static void main(String args[]) throws Exception {
		
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		
		String[] arguments1 = br.readLine().split("\\s+");
		String[] arguments2 = br.readLine().split("\\s+");
		
		Referee referee = new Referee(new Player("1", arguments1, "2\n"), new Player("2", arguments2, "1\n"));
		referee.beginMatch();
	}

}
