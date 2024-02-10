// PasswordManager, 2024
// Contributors: MichaelDev11, evelyn-murillo, GitNC6203

// ------- Includes ------------
#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include <cctype>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <conio.h>

using namespace std;
// -----------------------------


// PROTOTYPE Definitions
void clear();
void sign_in();
string encryptPassword(string password);
string decryptPassword(string password);
void generatePassword();
int getShiftValue();
void storePassword(string password, string application);
void loadPassword();
void createFiles();

// Function to clear the terminal
void clear() {
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Linux/Mac
#endif
}

void createFiles() {
    // Create userinfo file if it doesn't exist
    ofstream userinfoFile("userinfo.txt", ios::app | ios::out);
    if (!userinfoFile.is_open()) {
        cerr << "Error creating/opening userinfo.txt." << endl;
        return;
    }
    userinfoFile.close();

    // Create passwords file if it doesn't exist
    ofstream passwordsFile("passwords.txt", ios::app | ios::out);
    if (!passwordsFile.is_open()) {
        cerr << "Error creating/opening passwords.txt." << endl;
        return;
    }
    passwordsFile.close();

    // Create secureshift file if it doesn't exist
    ofstream secureshiftFile("secureshift.txt", ios::app | ios::out);
    if (!secureshiftFile.is_open()) {
        cerr << "Error creating/opening secureshift.txt." << endl;
        return;
    }

    // Check if secureshift.txt is empty
    secureshiftFile.seekp(0, ios::end);
    if (secureshiftFile.tellp() == 0) {
        // Add random number to secureshift.txt
        srand(static_cast<unsigned int>(time(0)));
        int randomNumber = rand() % 15;
        secureshiftFile << randomNumber << endl;
    }

    secureshiftFile.close();
}

// Have the user sign in on first time they run the program. If they exist as a user have the user login.
void sign_in() {
    string user;
    string userpass;
    string pass;
    string accountCreated;

    createFiles();

    // Open userinfo file and check for username and password
    ifstream inputFile("userinfo.txt");
    ofstream outputFile("userinfo.txt", ios::app | ios::out);
    

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return;
    }

    getline(inputFile, accountCreated);

    if (accountCreated != "true") {
        clear();

        // Create account
        cout << "Create a login." << endl;
        cout << "Username: ";
        cin >> user;
        cout << "Password: ";
        cin >> userpass;

        // Store account information in the file
        outputFile << "true" << endl;
        outputFile << user << endl;
        outputFile << userpass << endl;

        cout << "\nLogin created." << endl;
        clear();
    }

    inputFile.close();  // Close the file before reopening

    // Open the file again for reading
    inputFile.open("userinfo.txt");

    // Check if the file is opened successfully
    if (!inputFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return;
    }

    // Read username and password from the file
    getline(inputFile, accountCreated);
    getline(inputFile, user);
    getline(inputFile, userpass);

    clear();
    cout << "Welcome to PasswordManager " << user << "!\n";

    do {
        std::cout << "Password: ";
        
        char ch;
        pass = "";
        int i = 0;

        while (1) {
            ch = _getch();

            if (ch == 13) // Enter key
                break;
            else if (ch == 8) { // Backspace key
                if (i > 0) {
                    std::cout << "\b \b"; // Move back, erase character, move back again
                    pass.pop_back();
                    i--;
                }
            } else {
                std::cout << '*';
                pass += ch;
                i++;
            }
        }
        std::cout << std::endl;
    } while (pass != userpass);

    inputFile.close();
}

// Get the value of the shift for the caesar cipher
int getShiftValue() {
    int shift = 0;
    ifstream inputFile("secureshift.txt");

    if (inputFile.is_open()) {
        // Read the shift value from secureshift.txt
        string shiftStr;
        getline(inputFile, shiftStr);
        inputFile.close();

        // Convert the string to an integer
        try {
            shift = stoi(shiftStr);
        } catch (const invalid_argument& e) {
            cerr << "Invalid shift value in secureshift.txt." << endl;
        } catch (const out_of_range& e) {
            cerr << "Shift value in secureshift.txt is out of range." << endl;
        }
    } else {
        cerr << "Error opening secureshift.txt." << endl;
    }

    return shift;
}

// Encrypt the password with caesar cipher
string encryptPassword(string password) {
    int shift = getShiftValue();

    for (char& c : password) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            c = (c - base + shift) % 26 + base;
        } else if (isdigit(c)) {
            c = (c - '0' + shift) % 10 + '0';
        }
        // You may want to add additional cases for special characters if needed
    }

    return password;
}

// Decrypt the password with caesar cipher
string decryptPassword(string password) {
    int shift = getShiftValue();

    for (char& c : password) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            c = (c - base - shift + 26) % 26 + base;
        } else if (isdigit(c)) {
            c = (c - '0' - shift + 10) % 10 + '0';
        }
    }

    return password;
}

// Function to generate passwords and then store them in the passwords file
void generatePassword() {
    bool includeSpecialCharacters;
    int passwordLength;
    char enter_own;
    string application;
    string password;

    while (true) {
        cout << "Would you like to enter your own password?\n\t[1] Yes\n\t[2] Generate one\nOption: ";
        cin >> enter_own;

        if (enter_own == '1') {
            cout << "Enter your password: ";
            cin >> password;
            cout << "For which application will it be used for? ";
            cin.ignore(); // Clear newline from the buffer
            getline(cin, application);
            break; // Break out of the loop if '1' is entered
        } else if (enter_own == '2') {
            clear();

            char buffer;

            cout << "Special Characters? (Y/N): ";
            cin >> buffer;
            buffer = tolower(buffer);

            while (buffer != 'y' && buffer != 'n') {
                cout << "Invalid Selection. Please enter 'Y' or 'N': ";
                cin >> buffer;
                buffer = tolower(buffer);
            }

            includeSpecialCharacters = (buffer == 'y');

            cout << "How long would you like your password to be? ";
            while (!(cin >> passwordLength)) {
                // Handle non-numeric input
                cout << "Invalid input. Please enter a numeric value: ";
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            }

            cout << "For which application will it be used for? ";
            cin.ignore(); // Clear newline from the buffer
            getline(cin, application);

            const string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
            const string specialChar = "!@#$%^&*()";  

            srand(time(0));

            const int charsetSize = charset.size();
            const int specialSize = specialChar.size();

            for (int i = 0; i < passwordLength; i++) {
                if (includeSpecialCharacters) {
                    password += (i % 2 == 0) ? charset[rand() % charsetSize] : specialChar[rand() % specialSize];
                } else {
                    password += charset[rand() % charsetSize];
                }
            }
            break;
        } else {
            clear();
            cout << "Invalid Selection. Please enter '1' or '2': ";
        }
    }

    clear();
    cout << "Created password for " << application << ": " << password << endl;
    storePassword(password, application);
}

// Function to store the password and encrypt the file
void storePassword(string password, string application) {
    ofstream outputFile("passwords.txt", ios::app);
    if (!outputFile.is_open()) {
        cerr << "Error opening the passwords file." << endl;
        return;
    }

    password = encryptPassword(password);

    outputFile << password << "\t\t" << application << endl;

    outputFile.close();

    cout << "Password saved successfully." << endl;
}

// Function to print all passwords by decrypting them as they are printed
void loadPassword() {
    ifstream inputFile("passwords.txt");

    if (!inputFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return;
    }

    string encryptedPassword;
    string application;

    clear();
    cout << "---------------Passwords---------------\n" << endl;
    while (inputFile >> encryptedPassword >> application) {
        string decryptedPassword = decryptPassword(encryptedPassword);

        cout << "Application: " << application << "\t\tDecrypted Password: " << decryptedPassword << endl;
    }

    cout << endl;
    inputFile.close();
}

int main() {
    // Sign in
    sign_in();
    clear();
    char gen_or_load;

    // Run main function
    bool create;
    string password;

    cout << "Welcome to your personalized Password Manager" << endl;

    do {
        std::cout << "Please choose: \n\t[1] Create New Password \n\t[2] Load Passwords\nChoice: ";
        std::cin >> gen_or_load;

        switch (gen_or_load) {
            case '1':
                std::cout << "You have chosen to Generate a new password" << endl;
                break;

            case '2':
                std::cout << "You have chosen to Load a password" << endl;
                break;

            default:
                clear();
                std::cout << "Not a valid choice. Please choose again." << endl;
                break;
        }
    } while (gen_or_load != '1' && gen_or_load != '2');

    if (gen_or_load == '1') {

        clear();
        generatePassword();
           
        
        char create;
        bool continueCreating = true;

        while(continueCreating) {
            std::cout << "Would you like to: \n\t[1] Keep Creating Passwords\n\t[2] Save and Exit\n\t[3] Load Password\nChoice: ";
            std::cin >> create;
            create = tolower(create);

            if (create == '1') {
                clear();
                generatePassword(); 
            } else if (create == '2') {
                continueCreating = false;
            } else if (create == '3') {
                clear();
                loadPassword();
            } else {
                clear();
                std::cout << "Invalid input. Please enter a valid option." << endl;
            }
        }

    } else if (gen_or_load == '2') {
        clear();
        loadPassword();
        char create;
        bool continueCreating = true;

        while(continueCreating) {
            std::cout << "Would you like to: \n\t[1] Keep Creating Passwords\n\t[2] Save and Exit\n\t[3] Load Password\nChoice: ";
            std::cin >> create;
            create = tolower(create);

            if (create == '1') {
                clear();
                generatePassword(); 
            } else if (create == '2') {
                continueCreating = false;
            } else if (create == '3') {
                loadPassword();
            } else {
                clear();
                std::cout << "Invalid input. Please choose a valid option" << endl;
            }
        }
    } else {
        cout << "Easter egg ? ravioli ravioli, give me the formuoli";
    }
}
