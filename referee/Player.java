package com.java.evaluator;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.Arrays;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

public class Player {
	
	private String name;
	private Process player;
	private String secretNumber;
	private BufferedReader reader;
	private OutputStream writer;
	
	Player(String name, String[] arguments, String opponentId) throws Exception{
		try {
			player = new ProcessBuilder(arguments).start();
			reader = new BufferedReader(
					new InputStreamReader(player.getInputStream()));
			writer = (BufferedOutputStream) player.getOutputStream();
			this.name = name;
			
			try {
				receive(opponentId);
				secretNumber = readWithTimeout();
				if(!isValidSecretNumber())
					throw new IllegalArgumentException(
						String.format("Player %s has invalid secret number:%s\n",name, secretNumber));
				System.out.println(String.format("Player %s Secret Number:%s", name, secretNumber));
			}
			catch(Exception e){
					e.printStackTrace();
					throw new IOException(String.format("Player %s did not output secret number.", name));
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new Exception("Error caused by Player:"+name);
		}
	}
	
	/** Assigns the player ID. Player ID 1 starts the game. */
	void assignPlayerID(String id) throws IOException{
		try {
		 writer.write(id.getBytes());
		 writer.flush();
		}
		catch(Exception e) {
			e.printStackTrace();
			throw new IOException(String.format("%s:%s" ,"Error caused by Player:", name));
		}
	}
	
	/** Returns the guess of the player. */
	String guess() throws IOException {
		try {
				String guess = readWithTimeout();
				if(!isValidGuess(guess))
					throw new IllegalArgumentException(String.format("Player %s has invalid guess:%s",name, guess));
				return guess+"\n";
		}
		catch(Exception e){
			e.printStackTrace();
			throw new IOException(String.format("%s:%s" ,"Error caused by Player", name));
		}
	}
	
	/** Sends a string to the player. */
	void receive(String response) throws IOException {
		try {
			writer.write(response.getBytes());
			writer.flush();
		}
		catch(Exception e) {
			e.printStackTrace();
			throw new IOException(String.format("%s:%s" ,"Error caused by Player:", name));
		}
	}
	
	/** Receives the number of bulls and cows from the player and validates the response against the guess. */
	String getResponse(String guess) {
		 int bulls = 0, cows = 0;
		try {
				bulls = Integer.parseInt(readWithTimeout());
				cows = Integer.parseInt(readWithTimeout());
		} catch (Exception e) {
			 e.printStackTrace();
			throw new IllegalArgumentException(String.format("Player %s responded invalid bulls and cows.", name));
		}
		
		 checkValidResponse(guess, bulls, cows);
		 System.out.println(String.format("Player %s responds: %d bulls %d cows",name, bulls , cows));
		 return bulls + "\n" + cows + "\n";
	}
	
	String getSecretNumber() {
		return secretNumber;
	}
	
	/** Validates that the guess is a valid four digit integer. */
	private boolean isValidGuess(String guess) {
		try {
			if(guess.length()!=4)
				return false;
			Integer.parseInt(guess); 
			return true;
		}
		catch(Exception e) {
			return false;
		}
	}
	
	/** Validates that the player returns the correct response for a guess. */
	private void checkValidResponse(String guess, int bulls, int cows) {
		int expectedBulls = 0;
		int expectedCows = 0;
	
		//count the number of bulls first
		for(int i=0; i<guess.length()-1; ++i) {
			if(guess.charAt(i)==secretNumber.charAt(i)) {
				++expectedBulls;
				guess = guess.replace(guess.charAt(i),'\0'); /** A digit counted as bull should not be counted as cow. */
			}
		}
		
		//count the number of cows
		for(int i=0; i<guess.length()-1; ++i) {
			if(secretNumber.contains(guess.charAt(i)+"")) {
				++expectedCows;
				guess = guess.replace(guess.charAt(i),'\0');  /** A digit counted as cow once should not be counted again. */
			}
		}
		
		if(bulls != expectedBulls  || cows != expectedCows)
			 throw new IllegalArgumentException(String.format("Player %s has invalid response:%d, %d. "
			 		+ "Expected bulls:%d, Expected cows:%d",name, bulls, cows, expectedBulls, expectedCows));
	}
	
	 private boolean hasDistinctDigits(int number) {
	     int numMask = 0;
	     int numDigits = (int) Math.ceil(Math.log10(number+1));
	     for (int digitIdx = 0; digitIdx < numDigits; digitIdx++) {
	         int curDigit = (int)(number / Math.pow(10,digitIdx)) % 10;
	         int digitMask = (int)Math.pow(2, curDigit);             
	         if ((numMask & digitMask) > 0) return false;
	         numMask = numMask | digitMask;
	     }
	     return true;
	 }
	
	private boolean isValidSecretNumber() {
		// A valid secret number should contain 4 digits, must not start with 0 and all digits
		// should be unique
		if(secretNumber.length()!=4 || secretNumber.charAt(0)=='0')
			return false;
		try {
			int number = Integer.parseInt(secretNumber);
			return hasDistinctDigits(number);
		}
		catch(Exception e){
			return false;
		}
	}
	
	private String readWithTimeout() throws InterruptedException, ExecutionException, TimeoutException {
		ExecutorService executor = Executors.newSingleThreadExecutor();
		Future<String> future = executor.submit(new Callable() {

		    public String call() throws Exception {
		        return reader.readLine();
		    }
		});
		String response = future.get(60, TimeUnit.SECONDS);
		executor.shutdownNow();
		return response;
	}
}
