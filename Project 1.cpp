#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

void loadStats(int &wins, int &losses) {
    wins = 0;
    losses = 0;
    ifstream file("player_stats.txt");
    if (file.is_open()) {
        string label;
        file >> label >> wins; 
        file >> label >> losses;
        file.close();
    }
}
void saveStats(int wins, int losses) {
    ofstream file("player_stats.txt");
    file << "Wins: " << wins << endl;
    file << "Losses: " << losses << endl;
    file.close();
}
void loadLeaderboard(string names[3], int scores[3]) {
    names[0] = names[1] = names[2] = "---";
    scores[0] = scores[1] = scores[2] = 999;

    ifstream file("leaderboard.txt");
    if (file.is_open()) {
        for (int i = 0; i < 3; i++) {
            string name, dash, attemptsWord;
            if (!(file >>name>>names[i]>>dash>>scores[i]>>attemptsWord)) 
			break;
        }
        file.close();
    }
}
void saveLeaderboard(string names[3], int scores[3]) {
    ofstream file("leaderboard.txt");
    for (int i = 0; i < 3; i++) {
        file << (i + 1) << "." << names[i] << "-" <<scores[i] <<" attempts"<< endl;
    }
    file.close();
}
void showLeaderboard(string names[3], int scores[3]) {
    cout << "\n----- TOP 3 SCORES -----" << endl;
    for (int i = 0; i < 3; i++) {
        cout << (i + 1) << ". " << names[i] << " - " << scores[i] << " attempts" << endl;
    }
    cout << "-------------------------" << endl;
}
void updateLeaderboard(string names[3], int scores[3], string playerName, int attempts) {
    if (attempts < scores[0]) {
        names[2]=names[1]; 
		scores[2]=scores[1];
        names[1]=names[0]; 
		scores[1]=scores[0];
        names[0]=playerName; 
		scores[0]=attempts;
        cout << "New #1 high score!" << endl;
    }
    else if (attempts < scores[1]) {
        names[2] = names[1]; 
		scores[2] = scores[1];
        names[1] = playerName; 
		scores[1] = attempts;
        cout << "New #2 high score!" << endl;
    }
    else if (attempts < scores[2]) {
        names[2] = playerName;
		scores[2] = attempts;
        cout << "New #3 high score!" << endl;
    }
}
int chooseDifficulty() {
    int choice;
    cout << "\nChoose difficulty:" << endl;
    cout << "1. Easy   (1 - 50)" << endl;
    cout << "2. Medium (1 - 100)" << endl;
    cout << "3. Hard   (1 - 200)" << endl;
    cout << "Enter choice (1-3): ";
    cin >> choice;

    if (choice == 1){
    	return 50;
	}
    if (choice == 3){
    	 return 200;
	}
	else{
		return 100;
	}
}
int playRound(int maxNumber, int maxAttempts) {
    int secret = (rand() % maxNumber) + 1;
    int guess;
    int attempts = 0;
    int lastDistance = -1;

    cout << "\nPicked a number between 1 and " << maxNumber << "." << endl;
    cout << "You have " << maxAttempts << " attempts. Good luck!" << endl;

    for (int i = 1; i <= maxAttempts; i++) {
        cout << "\nAttempt " << i << "/" << maxAttempts << " - Your guess: ";
        cin >> guess;

        if (guess < 1 || guess > maxNumber) {
            cout << "Please guess between 1 and " << maxNumber << "!" << endl;
            i--;
            continue;
        }

        attempts++;
        int distance = guess - secret;
        if (distance < 0) {
        	distance = -distance;
		}
        if (distance == 0) {
            cout << "\nCORRECT! The number was " << secret << "!" << endl;
            cout << "You got it in " << attempts << " attempts!" << endl;
            return attempts;
        }

        if (guess < secret){
        	cout << "Too LOW! ";
		} 
        else {
        	cout << "Too HIGH! ";
		}
        if (lastDistance!=-1) {
            if (distance < lastDistance) {
            	cout << "Getting warmer!" << endl;
			}
            else{
            	cout << "Getting colder!" << endl;
			} 
        } else {
            cout << endl;
        }

        lastDistance = distance;
    }

    cout << "\nOut of attempts! The number was " << secret << "." << endl;
    return -1;
}

int main() {
    srand(time(0));
    int wins, losses;
    loadStats(wins, losses);

    string leaderNames[3];
    int leaderScores[3];
    loadLeaderboard(leaderNames, leaderScores);
    cout << "================================" << endl;
    cout << "  CLOUDEXIFY NUMBER GUESS GAME  " << endl;
    cout << "================================" << endl;
    string playerName;
    cout << "Enter your name: ";
    cin >> playerName;

    cout << "\nWelcome, " << playerName << "!" << endl;
    cout << "Wins: " << wins << " | Losses: " << losses << endl;
    showLeaderboard(leaderNames, leaderScores);

    char playAgain;
    do {
        int maxNumber =chooseDifficulty();
        int maxAttempts = 10;

        int attempts = playRound(maxNumber, maxAttempts);

        if (attempts != -1) {
            wins++;
            updateLeaderboard(leaderNames, leaderScores, playerName, attempts);
        } else {
            losses++;
        }

        saveStats(wins, losses);
        saveLeaderboard(leaderNames, leaderScores);

        cout<<"\nCurrent record -> Wins: "<< wins <<" | Losses: "<<losses<<endl;
        cout <<"\nPlay again? (y/n): ";
        cin >> playAgain;
    } while (playAgain =='y'||playAgain=='Y');
    showLeaderboard(leaderNames, leaderScores);
    cout << "\nThanks for playing, " << playerName << "! Goodbye!" << endl;
    return 0;
}