#include <iostream>
#include <random>
#include <map>
#include <fstream>
#include <string>
#include <tuple>

using namespace std;

struct PollResult{
  float democrat;
  float republican;

  PollResult(float d, float r) : democrat(d), republican(r) {}
};

map<string, int> getElectoralVotes(const string& electoralVotesFile) {
  ifstream file(electoralVotesFile);

  map<string, int> electoralVotes;

  string state;
  int votes;
  string temp;

  while(file.good()) {
    getline(file, state, ',');

    getline(file, temp);
    votes = stoi(temp);

    electoralVotes.insert(pair<string, int>(state, votes));
  }
  return electoralVotes;
  
}

random_device r;
default_random_engine generator{r()};


float addNoise(float poll, float marginOfError) {
  normal_distribution<float> d(0, .5);
  float number;
  while (true) {
    number = d(generator);
    if (number >= -1 && number <= 1)
      break;
  }
  float offset = number * marginOfError; 
  return offset + poll;
}

PollResult simulateOneElection(map<string, PollResult> polls, map<string, int> electoralVotes, float marginOfError){
  int votes1 = 0;
  int votes2 = 0;

  float poll;
  float adjustedPoll;

  map<string, PollResult>::iterator it = polls.begin();
  while (it != polls.end()) {
    poll = it->second.democrat; 
    adjustedPoll = addNoise(poll, marginOfError);
    if (adjustedPoll >= 50.0) 
      votes1 += electoralVotes[it->first];
    else
      votes2 += electoralVotes[it->first];
    it++;
  }
  return PollResult(votes1, votes2);
}


tuple<float,float,float> simulateMultipleElections(map<string, PollResult> pollingData, map<string, int> electoralVotes, int numTrials, float marginOfError) {
    int winCount1 = 0;
    int winCount2 = 0;
    int tieCount = 0;
    for (int i = 0; i < numTrials; i++) {
      PollResult votes = simulateOneElection(pollingData, electoralVotes, marginOfError);
      if (votes.democrat > votes.republican)
        winCount1 += 1;
      else if (votes.republican > votes.democrat)
        winCount2 += 1;
      else
        tieCount += 1;
    }
    float probabilityD = (float)winCount1/numTrials;
    float probabilityR = (float)winCount2/numTrials;
    float probabilityTie = (float)tieCount/numTrials;
    return tuple<float, float, float>(probabilityD, probabilityR, probabilityTie);
}

map<string, PollResult> loadPollData(string& pollFile) {
  ifstream file(pollFile);

  map<string, PollResult> pollingData;

  string state;
  float democrat;
  float republican;
  string temp;

  while(file.good()) {
    getline(file, state, ',');

    getline(file, temp, ',');
    democrat = stof(temp);

    getline(file, temp);
    republican = stof(temp);

    pollingData.insert(pair<string, PollResult>(state, PollResult(democrat, republican)));
  }
  return pollingData;
}

int main(int argc, char** argv) {
  // need to specify the poll to use, numTrials is optional
  if (argc < 3) {
    cout << "Specify the poll file to use, electoral votes file, and optionally the number of trials" << endl;
    return 1;
  }

  // There are three polls in the data - earlyPolls, debates, conventions
  string pollFile = argv[1];
  string electoralVotesFile = argv[2];
  int numTrials = 100;
  if (argc == 4)
    numTrials = atoi(argv[3]);

  map<string, PollResult> pollingData = loadPollData(pollFile);
  map<string, int> electoralVotes = getElectoralVotes(electoralVotesFile);


  float pDemocrat, pRepublican, pTie;

  tie(pDemocrat, pRepublican, pTie) = simulateMultipleElections(pollingData, electoralVotes, numTrials, 15);
  cout << pDemocrat << ", " << pRepublican << ", " << pTie << endl;
  return 0;
}
