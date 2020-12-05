#include "main.h"
#include <iostream>
#include <string>
#include <vector>
#include "States.h"
#include "Districts.h"
#include "Voter.h"
#include "DataHandler.h"
#include "TreeNode.h"
#include "Tree.h"
using namespace std;

void createVoters();
int* nationalInfoMap(unordered_map<string, States>& sMap);
void senatorialInfoAVL(TreeNode* root);
void nationalInfoAVL(TreeNode* root, int* info);

int main()
{
    DataHandler::initData();
    unordered_map<string, States>& sMap = DataHandler::stateMap;
    Tree* sTree = DataHandler::stateTree;

    createVoters();
    string presidentDem;
    string presidentRep;

    string states[50] = { "Alabama", "Alaska", "Arizona", "Arkansas", "California", "Colorado", "Connecticut", "Delaware",
         "Florida", "Georgia", "Hawaii", "Idaho", "Illinois", "Indiana", "Iowa", "Kansas", "Kentucky", "Louisiana",
        "Maine", "Maryland", "Massachusetts", "Michigan", "Minnesota", "Mississippi", "Missouri", "Montana", "Nebraska",
        "Nevada", "New Hampshire", "New Jersey", "New Mexico", "New York", "North Carolina", "North Dakota", "Ohio",
        "Oklahoma", "Oregon", "Pennsylvania", "Rhode Island", "South Carolina", "South Dakota", "Tennessee", "Texas", "Utah",
        "Vermont", "Virginia", "Washington", "West Virginia", "Wisconsin", "Wyoming" };

    cout << "Welcome to the 539 Election Data Search!" << endl;
    cout << "\nPlease input one of the following to select the method by which the data will be accessed:" << endl;
    cout << "[0] Exit 539 Election Data Search" << endl;
    cout << "[1] Unordered Map" << endl;
    cout << "[2] AVL Tree\n" << endl;
    

    int sortInput;
    cin >> sortInput;

    if (sortInput == 0)
    { 
        cout << "Thank you for using 539 Election Data Search!" << endl;
        return 0;
    }
    cout << "Please input one of the folowing to select the National Election Data, all Senatorial data, or a specific state" << endl;
    cout << "[0] Return to Data Access Method Selection" << endl;
    cout << "[51] National Election Data " << endl;
    cout << "[52] All Senatorial Election Data\n" << endl;

    cout << "    [1] Alabama     | [11] Hawaii   | [21] Massachusetts | [31] New Mexico     | [41] South Dakota\n\
    [2] Alaska      | [12] Idaho    | [22] Michigan      | [32] New York       | [42] Tennessee\n\
    [3] Arizona     | [13] Illinois | [23] Minnesota     | [33] North Carolina | [43] Texas\n\
    [4] Arkansas    | [14] Indiana  | [24] Mississippi   | [34] North Dakota   | [44] Utah\n\
    [5] California  | [15] Iowa     | [25] Missouri      | [35] Ohio           | [45] Vermont\n\
    [6] Colorado    | [16] Kansas   | [26] Montana       | [36] Oklahoma       | [46] Virginia\n\
    [7] Connecticut | [17] Kentucky | [27] Nebraska      | [37] Oregon         | [47] Washington\n\
    [8] Deleware    | [18] Loisiana | [28] Nevada        | [38] Pennsylvania   | [48] West Virginia\n\
    [9] Florida     | [19] Maine    | [29] New Hampshire | [39] Rhode Island   | [49] Wisconsin\n\
    [10] Georgia    | [20] Maryland | [30] New Jersey    | [40] South Carolina | [50] Wyoming\n\n";


    int input = 1;
    while (input != 0)
    {
        cin >> input;
        if (input == 51)
        {
            cout << "National Election Data:" << endl;
            int* info = nationalInfoMap(sMap);

            cout << "Candidate Name: Joe Biden    | Vote Count: " << info[0] << " | Vote Percentage: " << ((double)info[0])/info[3] << endl;
            cout << "Candidate Name: Donald Trump | Vote Count: " << info[1] << " | Vote Percentage: " << ((double)info[1])/info[3] << endl;
            cout << "Candidate Name: Other        | Vote Count: " << info[2] << " | Vote Percentage: " << ((double)info[2])/info[3] << endl;
            cout << endl;
        }
        else if (input == 52)
        {
            if (sortInput == 1)
            {
                for (auto j = sMap.begin(); j != sMap.end(); j++)
                {
                    if (j->second.getDemSenator().compare("NONE") != 0)
                    {
                        cout << "\nSenatorial Election Data for " << j->second.getState() << ": " << endl;
                            cout << "Candidate Name: " << j->second.getDemSenator() << " | Vote Count: " << j->second.getDemSenVotes() << "   | Vote Percentage: " << j->second.getPercent(3) << "%" << endl;
                            cout << "Candidate Name: " << j->second.getRepSenator() << " | Vote Count: " << j->second.getRepSenVotes() << "   | Vote Percentage: " << j->second.getPercent(4) << "%" << endl;
                            cout << "Candidate Name: Other      | Vote Count: " << j->second.getOtherSenVotes() << " | Vote Percentage: " << j->second.getPercent(5) << "%" << endl;
                    }
                    else
                    {
                        cout << "No Senatorial Election Data available for " << j->second.getState() << endl;
                    }
                }
            }
            else if (sortInput == 2)
            {
                senatorialInfoAVL(sTree->root);
            }
        }
        else
        {
            input--;
            States state;
            TreeNode* stateNode;
            if(sortInput == 1)
                state = sMap[states[input]];
            else
            {
                stateNode = sTree->search(sTree->root, states[input]);
                state = stateNode->state;
            }

            cout << "State: " << state.getState() << endl;
            cout << "Democrat Senator: " << state.getDemSenator() << endl;
            cout << "Republican Senator: " << state.getRepSenator() << endl;
            cout << "Number of voters: " << state.getVoterCapacity() << endl;
            cout << "Number of electoral votes: " << state.getElectoralVotes() << endl << endl;

            cout << "\nPlease input one of the following to select the data for the state " << state.getState() << ":" << endl;
            cout << "[0] Return to State selection" << endl;
            cout << "[1] National Election data" << endl;
            cout << "[2] Senatorial Election Data" << endl;
            cout << "[3] ALL District Election Data" << endl;
            cout << "[4] A Single District's Election Data\n" << endl;
            int input2;
            cin >> input2;
            if (input2 == 1)
            {
                cout << "National Election Data for " << state.getState() << ": " << endl;
                cout << "Candidate Name: Xxxxx Xxxx | Vote Count: " << state.getDemPresVotes() << "   | Vote Percentage: " << state.getPercent(0) << "%" << endl;
                cout << "Candidate Name: Xxxxx Xxxx | Vote Count: " << state.getRepPresVotes() << "   | Vote Percentage: " << state.getPercent(1) << "%" << endl;
                cout << "Candidate Name: Other      | Vote Count: " << state.getOtherPresVotes() << " | Vote Percentage: " << state.getPercent(2) << "%" << endl;
            }

            if (input2 == 2)
            {
                if (state.getDemSenator().compare("NONE") != 0)
                {
                    cout << "Senatorial Election Data for " << state.getState() << ": " << endl;
                    cout << "Candidate Name: " << state.getDemSenator() << " | Vote Count: " << state.getDemSenVotes() << "   | Vote Percentage: " << state.getPercent(3) << "%" << endl;
                    cout << "Candidate Name: " << state.getRepSenator() << " | Vote Count: " << state.getRepSenVotes() << "   | Vote Percentage: " << state.getPercent(4) << "%" << endl;
                    cout << "Candidate Name: Other      | Vote Count: " << state.getOtherSenVotes() << " | Vote Percentage: " << state.getPercent(5) << "%" << endl;
                }
                else
                {
                    cout << "No Senatorial Election Data available for " << state.getState() << endl;
                }
            }

            if (input2 == 3)
            {
                state.districtInfo();
            }

            if (input2 == 4)
            {
                int distInput = 1;
                cout << "There are " << state.getNumDistricts() << " congressional districts in the state of " << state.getState() << ". Input number for a district. Press 0 to exit District search:";
                while (distInput != 0)
                {
                    cin >> distInput;
                    Districts district = state.districtMap[--distInput];
                    cout << "District Number: " << district.getDistrict() << endl;
                    cout << "Number of voters: " << district.getVoterCapacity() << endl;

                    int dataInput = 1;
                    cout << "\nPlease input one of the following to select the data for district " << distInput << ":" << endl;
                    cout << "[0] Leave Single District Data Search " << endl;
                    cout << "[1] National Election data" << endl;
                    cout << "[2] Senatorial Election Data" << endl;
                    cout << "[3] District Election Data" << endl;
                   
                    while (dataInput != 0)
                    {
                        cin >> dataInput;
                        if (dataInput == 1)
                        {
                            cout << "National Election Data for district " << distInput << " in " << state.getState() << ": " << endl;
                            cout << "Candidate Name: Xxxxx Xxxx | Vote Count: " << district.getVotersPresDem() << "   | Vote Percentage: " << district.getPercent(0) << "%" << endl;
                            cout << "Candidate Name: Xxxxx Xxxx | Vote Count: " << district.getVotersPresRep() << "   | Vote Percentage: " << district.getPercent(1) << "%" << endl;
                            cout << "Candidate Name: Other      | Vote Count: " << district.getVotersPresOther() << " | Vote Percentage: " << district.getPercent(2) << "%" << endl;
                        }
                        if (dataInput == 2)
                        {
                            if (state.getDemSenator().compare("NONE") != 0)
                            {
                                cout << "Senatorial Election Data for " << state.getState() << ": " << endl;
                                cout << "Candidate Name: " << state.getDemSenator() << " | Vote Count: " << state.getDemSenVotes() << "   | Vote Percentage: " << state.getPercent(3) << "%" << endl;
                                cout << "Candidate Name: " << state.getRepSenator() << " | Vote Count: " << state.getRepSenVotes() << "   | Vote Percentage: " << state.getPercent(4) << "%" << endl;
                                cout << "Candidate Name: Other      | Vote Count: " << state.getOtherSenVotes() << " | Vote Percentage: " << state.getPercent(5) << "%" << endl;
                            }
                            else
                            {
                                cout << "No Senatorial Election Data available for " << state.getState() << endl;
                            }
                        }
                        if (dataInput == 3)
                        {
                            cout << "Representative Election Data for district " << distInput << " in " << state.getState() << ": " << endl;
                            cout << "Candidate Name: " << district.getDemocrat() << " | Vote Count: " << district.getVotersDem() << "   | Vote Percentage: " << district.getPercent(6) << "%" << endl;
                            cout << "Candidate Name: " << district.getRepublican() << " | Vote Count: " << district.getVotersRep() << "   | Vote Percentage: " << district.getPercent(7) << "%" << endl;
                            cout << "Candidate Name: Other      | Vote Count: " << district.getVotersOther() << " | Vote Percentage: " << district.getPercent(8) << "%" << endl;
                        }
                        
                        cout << "\nPlease input one of the following to select the data for district " << distInput << ":" << endl;
                        cout << "[0] Leave Single District Data Search " << endl;
                        cout << "[1] National Election data" << endl;
                        cout << "[2] Senatorial Election Data" << endl;
                        cout << "[3] District Election Data" << endl;
                    }

                    cout << "There are " << state.getNumDistricts() << " congressional districts in the state of " << state.getState() << ". Input number for a district. Press 0 to exit District search:";
                }
            }
        }


        cout << "\nPlease input the number for a state, or enter 51 for National Election Data. Press 0 to exit program:\n" << endl;
        cout << "[1] Alabama     | [11] Hawaii   | [21] Massachusetts | [31] New Mexico     | [41] South Dakota\n\
    [2] Alaska      | [12] Idaho    | [22] Michigan      | [32] New York       | [42] Tennessee\n\
    [3] Arizona     | [13] Illinois | [23] Minnesota     | [33] North Carolina | [43] Texas\n\
    [4] Arkansas    | [14] Indiana  | [24] Mississippi   | [34] North Dakota   | [44] Utah\n\
    [5] California  | [15] Iowa     | [25] Missouri      | [35] Ohio           | [45] Vermont\n\
    [6] Colorado    | [16] Kansas   | [26] Montana       | [36] Oklahoma       | [46] Virginia\n\
    [7] Connecticut | [17] Kentucky | [27] Nebraska      | [37] Oregon         | [47] Washington\n\
    [8] Deleware    | [18] Loisiana | [28] Nevada        | [38] Pennsylvania   | [48] West Virginia\n\
    [9] Florida     | [19] Maine    | [29] New Hampshire | [39] Rhode Island   | [49] Wisconsin\n\
    [10] Georgia    | [20] Maryland | [30] New Jersey    | [40] South Carolina | [50] Wyoming\n\n";
    }

    return 0;
}

void createVoters() {
    int demCount = 0;
    int repCount = 0;
    int otherCount = 0;
    auto iter = DataHandler::stateMap.begin();
    //loops through every state
    for (iter; iter != DataHandler::stateMap.end(); iter++) {
        //loops through every district within that state
        for (int i = 1; i < iter->second.getNumDistricts() + 1; i++) {
            auto& curDistrict = iter->second.districtMap[i];
            //loops through the number of voters within that district
            for (int voter = 0; voter < curDistrict.getVoterCapacity(); voter++) {
                //generates what party the voter is voting for
                Voter::party castVote = Voter::vote(curDistrict.getInitPercentDem(), curDistrict.getInitPercentRep(), curDistrict.getInitPercentOther());
                //Insert into Splay tree here
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
            curDistrict.setVotersDem(demCount);
            curDistrict.setVotersRep(repCount);
            curDistrict.setVotersOther(otherCount);
        }
    }
}

int* nationalInfoMap(unordered_map<string, States>& sMap)
{
    int demVotes = 0;
    int repVotes = 0;
    int othVotes = 0;
    int total = 0;
    int info[4];
    for (auto i = sMap.begin(); i != sMap.end(); i++)
    {
        demVotes += i->second.getDemPresVotes();
        repVotes += i->second.getRepPresVotes();
        othVotes += i->second.getOtherPresVotes();
        total += i->second.getVoterCapacity();
    }
    info[0] = demVotes;
    info[1] = repVotes;
    info[2] = othVotes;
    info[3] = total;

    return info;

}

void nationalInfoAVL(TreeNode* root, int* info)
{
	if (root == nullptr)
		return;

	nationalInfoAVL(root->left, info);

	info[0] += root->state.getDemPresVotes();
    info[1] += root->state.getRepPresVotes();
    info[3] += root->state.getOtherPresVotes();

	nationalInfoAVL(root->right, info);
}

void senatorialInfoAVL(TreeNode* root)
{
    if (root == nullptr)
        return;

    senatorialInfoAVL(root->left);

    cout << "\nSenatorial Election Data for " << root->state.getState() << ": " << endl;
    cout << "Candidate Name: " << root->state.getDemSenator() << " | Vote Count: " << root->state.getDemSenVotes() << "   | Vote Percentage: " << root->state.getPercent(3) << "%" << endl;
    cout << "Candidate Name: " << root->state.getRepSenator() << " | Vote Count: " << root->state.getRepSenVotes() << "   | Vote Percentage: " << root->state.getPercent(4) << "%" << endl;
    cout << "Candidate Name: Other      | Vote Count: " << root->state.getOtherSenVotes() << " | Vote Percentage: " << root->state.getPercent(5) << "%" << endl;

    senatorialInfoAVL(root->right);
}











/*#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "States.h"
#include "Districts.h"
#include "Voter.h"
#include "DataHandler.h"
using namespace std;

void printStates();
void createVoters();

/*
 * TODO:
 *  - Fill out CSV for every state
 *  - Create Splay tree to pass in voters
 *  - Calculate winners for each candidate
 *  - Calculate electoral college wins
 *  - User interface to display data
 *  - Search to display data
 * 
 * Potentially?
 *  Make this project more dynamic and modular:
 *  - Allow user to increase/decrease voter turnout at the nation, state, or district level
 *  - Allow user to increase/decrease voter turnout for a specific party at the nation, state, or district level
 *      - lets users see how turnout can affect the electoral college
 *      - would set caps for max turnout at each district
 */

//int main()
//{
//    DataHandler::initData();
//    printStates();
//    createVoters();
//
//    return 0;
//}

/*
 * All state and district information are obtained in the CSV master file
 * The values in the CSV are based off the results of the 2020 election
 * All information is gathered from here: http://www.electproject.org/2020g
 */

/*void printStates()
{
    auto iter = DataHandler::stateMap.begin();
    for (iter; iter != DataHandler::stateMap.end(); iter++) {
        string state = iter->second.getState();
        cout << fixed << setprecision(1);
        cout << "State: " << state << endl;
        cout << "Democrat Senator: " << iter->second.getDemSenator() << endl;
        cout << "Republican Senator: " << iter->second.getRepSenator() << endl;
        cout << "Number of voters: " << iter->second.getVoterCapacity() << endl;
        cout << "Number of electoral votes: " << iter->second.getElectoralVotes() << endl << endl;

        for (int i = 1; i < iter->second.getNumDistricts()+1; i++) {
            auto& curDistrict = iter->second.districtMap[i];
            cout << "District Number: " << curDistrict.getDistrict() << endl;
            cout << "Number of voters: " << curDistrict.getVoterCapacity() << endl;
            cout << "Democrat Representative: " << curDistrict.getDemocrat() << endl;
            cout << "Republican Representative: " << curDistrict.getRepublican() << endl;
        }
    }
}

/*
 * Generates random voters
 * Starts at first state, then first district within that state
 * Generates voters based on percent dem, rep, other
 * Push vote to splay tree
 *
 * Question: Do we want the potential for voters to not vote for all races? (presidential, Senate, House)
 *           Do we want the potential for voters to not vote down party line?
 */

/*void createVoters() {
    int demCount = 0;
    int repCount = 0;
    int otherCount = 0;
    auto iter = DataHandler::stateMap.begin();
    //loops through every state
    for (iter; iter != DataHandler::stateMap.end(); iter++) {
        //loops through every district within that state
        for (int i = 1; i < iter->second.getNumDistricts()+1; i++) {
            auto& curDistrict = iter->second.districtMap[i];
            //loops through the number of voters within that district
            for (int voter = 0; voter < curDistrict.getVoterCapacity(); voter++) {
                //generates what party the voter is voting for
                Voter::party castVote = Voter::vote(curDistrict.getInitPercentDem(), curDistrict.getInitPercentRep(), curDistrict.getInitPercentOther());
                //Insert into Splay tree here
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
        }
    }
    cout << "Dem Count: " << demCount << endl;
    cout << "Rep Count: " << repCount << endl;
    cout << "Third Party Count: " << otherCount << endl;
}

*/


