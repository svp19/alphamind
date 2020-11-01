# -*- coding: utf-8 -*-
"""
Created on Sat Aug 11 14:16:40 2018

@author: Anudeep Tubati (https://github.com/neuralflux)
"""

import random

def repetitive(n):
    n = str(n)
    return n[0] in n[1:] or n[1] in n[2:] or n[2] == n[3]

class bnc(object):
    def __init__(self):
        self.pos = []
        self.start = '2846'
        self.score = [0,0]
        for i in range(1000, 10000):
            if not repetitive(i):
                self.pos.append(str(i))
        self.dup = self.pos[:]
    def playGuess(self, guess):
        print(guess)
        self.score = []
        bulls, cows = input('B '), input('C ')
        self.score.append(int(bulls))
        self.score.append(int(cows))
        self.dup.remove(guess)
        self.pos.remove(guess)
    def calcScore(self, secret, guess):
        bulls, cows = 0, 0
        for i in range(len(secret)):
            if secret[i]==guess[i]:
                bulls += 1
            elif secret[i] in guess:
                cows +=1
        return [bulls, cows]
    def elim(self, secret):
        for j in range(len(self.pos)):
            if self.calcScore(secret, self.pos[j]) != self.score:
                self.dup.remove(self.pos[j])
        self.pos = self.dup[:]
    def playGame(self):
        self.playGuess(self.start)
        self.elim(self.start)
        while self.score[0] != 4:
            temp = self.dup[len(self.dup)//2]
            self.playGuess(temp)
            if self.score[0] != 4:
                self.elim(temp)
                
# Start of the referee compatible program

trial = bnc()

id = int(input())

out = random.choice(trial.pos)

print(out)

firstIn = int(input())

if firstIn == 0:
    opponents = input()
    print(trial.calcScore(out, opponents)[0])
    print(trial.calcScore(out, opponents)[1])

while True:
    print(trial.dup[len(trial.dup)//2])
    bulls = int(input())
    cows = int(input())
    trial.score[0] = bulls
    trial.score[1] = cows
    trial.elim(trial.dup[len(trial.dup)//2])
    
    opponents = input()
    print(trial.calcScore(out, opponents)[0])
    print(trial.calcScore(out, opponents)[1])
