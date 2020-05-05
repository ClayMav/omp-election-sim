#include <iostream>
#include <random>

float addNoise(poll, marginOfError)
    x ← RandFloat()
    x ← 2 * x (now x is between 0 and 2)
    x ← x – 1 (now x is between -1 and 1)
    x ← x * marginOfError (now it’s in range 😊)
    return x + poll

void simulateOneElection(polls, electoralVotes, marginOfError)
    votes1 ← 0
    votes2 ← 0
    for every key state in polls
        poll ← candidate 1's polling percentage
        adjustedPoll ← AddNoise(poll, marginOfError)
        if adjustedPoll ≥ 0.5 (candidate 1 wins state)
            votes1 ← votes2 + electoralVotes[state]
        else (candidate 2 wins state)
            votes2 ← votes2 + electoralVotes[state]
    return votes1, votes2

void simulateMultipleElections(pollingData, numTrials, marginOfError)
    winCount1 ← 0
    winCount2 ← 0
    tieCount ← 0
    for numTrials total trials
        votes1,votes2 ← SimulateElection(pollingData, marginOfError)
        if votes1 > votes2
            winCount1 ← winCount1 + 1
        else if collegeVotes2 > collegeVotes1
            winCount2 ← winCount2 + 1
        else (tie!)
            tieCount ← tieCount + 1
    probability1 ← winCount1/numTrials
    probability2 ← winCount2/numTrials
    probabilityTie ← tieCount/numTrials
    return probability1, probability2, probabilityTie

int main(int& argc, char** argv) {
  int numTrials = 100;
  if (argc == 2)
    numTrials = atoi(argv[1]);
  simulateMultipleElections(numTrials)
  return 0;
}
