#include <Arduino.h>
#include <vector>
#include <string>

struct Candidate {
    String name;
    int votes;
};

void displayCandidates(const std::vector<Candidate>& candidates);
void castVote(std::vector<Candidate>& candidates);
void displayVoteCount(const std::vector<Candidate>& candidates);
void addCandidates(std::vector<Candidate>& candidates);
bool accessSettings(std::vector<Candidate>& candidates);
void showMenu();
String readStringFromSerial();
int readIntFromSerial();

std::vector<Candidate> candidates;
bool candidatesAdded = false;

void setup() {
    Serial.begin(115200);
    // Wait for Serial Monitor to open
    while (!Serial) {
        ; // Wait
    }
    Serial.println("\nVoting Machine Initialized.");
}

void loop() {
    int choice;
    showMenu();
    choice = readIntFromSerial();
    switch (choice) {
        case 1:
            if (candidates.empty()) {
                Serial.println("\nNo candidates available. Please add candidates first.");
                Serial.println("Use the settings menu to add candidates.");
            } else {
                displayCandidates(candidates);
                castVote(candidates);
            }
            break;
        case 2:
            accessSettings(candidates);
            break;
        case 3:
            Serial.println("\nExiting the Voting. Goodbye!");
            while (1); // Halt the program
            break;
        default:
            Serial.println("\nInvalid choice. Please try again.");
            break;
    }
}

void displayCandidates(const std::vector<Candidate>& candidates) {
    Serial.println("\nList of Candidates:");
    for (size_t i = 0; i < candidates.size(); i++) {
        Serial.print(i + 1);
        Serial.print(". ");
        Serial.println(candidates[i].name);
    }
}

void castVote(std::vector<Candidate>& candidates) {
    int choice;
    Serial.print("\nEnter the number corresponding to your candidate: ");
    choice = readIntFromSerial();
    if (choice > 0 && choice <= candidates.size()) {
        candidates[choice - 1].votes++;
        Serial.println("\nVote casted!");
    } else {
        Serial.println("\nInvalid choice. Try again.");
    }
}

void displayVoteCount(const std::vector<Candidate>& candidates) {
    Serial.println("\n--- Voting Results (Vote Count) ---");
    for (const auto& candidate : candidates) {
        Serial.print(candidate.name);
        Serial.print(": ");
        Serial.print(candidate.votes);
        Serial.println(" votes");
    }
}

void addCandidates(std::vector<Candidate>& candidates) {
    int numCandidates;
    Serial.print("Enter the number of candidates to add: ");
    numCandidates = readIntFromSerial();

    for (int i = 0; i < numCandidates; i++) {
        Serial.print("Enter the name of candidate ");
        Serial.print(i + 1);
        Serial.print(": ");
        String candidateName = readStringFromSerial();
        candidates.push_back({candidateName, 0});
    }
    Serial.println("\nCandidates added successfully!");
}

bool accessSettings(std::vector<Candidate>& candidates) {
    String password;
    Serial.print("\nEnter the admin password: ");
    password = readStringFromSerial();
    if (password == "Admin@123") {
        int option;
        do {
            Serial.println("\n--- Settings Menu ---");
            Serial.println("1. Display vote count for each candidate");
            Serial.println("2. Add more candidates");
            Serial.println("3. Exit to main menu");
            Serial.print("Enter your choice: ");
            option = readIntFromSerial();

            switch (option) {
                case 1:
                    if (candidates.empty()) {
                        Serial.println("\nNo candidates available. Please add candidates first.");
                        addCandidates(candidates);
                    } else {
                        displayVoteCount(candidates);
                    }
                    break;
                case 2:
                    addCandidates(candidates);
                    break;
                case 3:
                    Serial.println("\nExiting settings...");
                    break;
                default:
                    Serial.println("\nInvalid option. Please try again.");
                    break;
            }
        } while (option != 3);
        return true;
    } else {
        Serial.println("\nIncorrect password!");
        return false;
    }
}

void showMenu() {
    Serial.println("\n--- Main Menu ---");
    Serial.println("1. Cast Vote");
    Serial.println("2. Other Settings");
    Serial.println("3. Exit");
    Serial.print("Enter your choice: ");
}

String readStringFromSerial() {
    while (!Serial.available()) {
        // Wait for input
    }
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any leading/trailing whitespace
    return input;
}

int readIntFromSerial() {
    String inputString = readStringFromSerial();
    return inputString.toInt();
}
