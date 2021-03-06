#include "DataHandler.h"

unordered_map<string, States> DataHandler::stateMap;
Tree* DataHandler::stateTree = new Tree();
StoringData DataHandler::st;
int DataHandler::demElectors;
int DataHandler::repElectors;


/*
 * Reads the District_map CSV file to form the stateMap and DistrictMap structures
 *  The information is: state name, names of senators, electoral college voters, number of voters,
 *			    	    number of districts, distict candidates, party representation of that district
 *
 * States are in alphabetical order in csv
 **
 */
void DataHandler::readCSV(string filePath, int offset) {
	ifstream file(filePath);
	string lineFromFile;

	/*
	 * Offset brings you to the state that you want to visit.
	 * Ex: Offset = 0 brings you to first state, Alabama
	 * This automatically skips the first two rows of the csv
	 */
	for (int i = 0; i < offset + 2; i++) {
		getline(file, lineFromFile);
	}

	while (getline(file, lineFromFile)) {
		istringstream streamFromAString(lineFromFile);

		string stateName;
		getline(streamFromAString, stateName, ',');

		string demRep;
		getline(streamFromAString, demRep, ',');

		string repRep;
		getline(streamFromAString, repRep, ',');

		string voters;
		getline(streamFromAString, voters, ',');
		int numVoters = stoi(voters);

		string electoralVotes_;
		getline(streamFromAString, electoralVotes_, ',');
		int electoralVotes = stoi(electoralVotes_);

		string numDistrics_;
		getline(streamFromAString, numDistrics_, ',');
		int numDistricts = stoi(numDistrics_);

		States state(stateName, numVoters, numDistricts, demRep, repRep, electoralVotes);
		
		for (int i = 1; i < numDistricts+1; i++) {
			
			string districtNumber_;
			getline(streamFromAString, districtNumber_, ',');
			int districtNumber = stoi(districtNumber_);

			getline(streamFromAString, voters, ',');
			numVoters = stoi(voters);

			getline(streamFromAString, demRep, ',');
			getline(streamFromAString, repRep, ',');

			string percentDem;
			getline(streamFromAString, percentDem, ',');
			float perDem = stof(percentDem);

			string percentRep;
			getline(streamFromAString, percentRep, ',');
			float perRep = stof(percentRep);

			Districts district(districtNumber, numVoters, perDem, perRep, demRep, repRep);
			state.districtMap[i] = district;
		}

		//Used for time comparisons between AVL and hashmap insertions
		auto start = chrono::high_resolution_clock::now();
		DataHandler::stateTree->root = DataHandler::stateTree->insert(DataHandler::stateTree->root, &state);
		auto end = chrono::high_resolution_clock::now();

		double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
		time_taken *= 1e-9;
		
		DataHandler::st.updateTimeTreeIns(time_taken);

		start = chrono::high_resolution_clock::now();
		stateMap[stateName] = state;
		end = chrono::high_resolution_clock::now();

		time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
		time_taken *= 1e-9;

		DataHandler::st.updateTimeMapIns(time_taken);
	}
}


/*
 * This function loops through every state and every district within that state
 * It calls upon the voter class to generate a every voter based on party percentages within that district
 * The add vote methods are then called upon to store this voting data within each state
 */
void DataHandler::createVoters() {
	auto iter = DataHandler::stateMap.begin();
	//loops through every state
	int treeCount = 0;
	for (iter; iter != DataHandler::stateMap.end(); iter++) {
		//loops through every district within that state
		auto& curState = iter->second;
		TreeNode* curTreeState = stateTree->search(stateTree->root, curState.getState());
		for (int i = 1; i < iter->second.getNumDistricts() + 1; i++) {
			auto& curDistrict = iter->second.districtMap[i];
			int demCount = 0;
			int repCount = 0;
			int otherCount = 0;
			//loops through the number of voters within that district
			for (int voter = 0; voter < curDistrict.getVoterCapacity(); voter++) {
				//generates what party the voter is voting for
				Voter::party castVote = Voter::vote(curDistrict.getInitPercentDem(), curDistrict.getInitPercentRep(), curDistrict.getInitPercentOther());
				switch (castVote)
				{
				case Voter::DEM:
					demCount++;
					break;
				case Voter::REP:
					repCount++;
					break;
				case Voter::THIRD:
					otherCount++;
					break;
				default:
					break;
				}
			}
			// Adds the votes to the state object after every district. Done for both AVL state and map state
			curTreeState->state.addDemVotes(demCount, i);
			curTreeState->state.addRepVotes(repCount, i);
			curTreeState->state.addOtherVotes(otherCount, i);
			curState.addDemVotes(demCount, i);
			curState.addRepVotes(repCount, i);
			curState.addOtherVotes(otherCount, i);
		}
	}
}


/*
 * This method calculates the amount of electoral votes assigned to each candidate by looping
 * through the state and retrieving which candidate won the popular vote
 * Note: third party candidates are not considered and states with split votes are not taken into account
 */
void DataHandler::calculateElectoralVotes() {
	auto iter = DataHandler::stateMap.begin();
	demElectors = 0;
	repElectors = 0;
	for (iter; iter != DataHandler::stateMap.end(); iter++) {
		auto& curState = iter->second;
		Voter::party winner = curState.determineWinner();
		switch (winner)
		{
		case Voter::DEM:
			demElectors += curState.getElectoralVotes();
			break;
		case Voter::REP:
			repElectors += curState.getElectoralVotes();
			break;
		default:
			break;
		}
	}
}


// These are getter functions for the elector votes
int DataHandler::demElectoralVotes() {
	return demElectors + 3;
}

int DataHandler::repElectoralVotes() {
	return repElectors;
}

// This calls upon these methods to initialize all data used for the menu
void DataHandler::initData() {
	readCSV("District_map.csv");
	createVoters();
	calculateElectoralVotes();
}
