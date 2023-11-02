// Implementation file for TextMenu class
// By Mary Elaine Califf

#include <iostream>
#include <iomanip>
#include <cctype>
#include "TextMenu.h"
using namespace std;

void TextMenu::setUp(int numChoices, string *menuItems)
{
    // clear out any existing items
    this->menuItems.clear();

    // add these to the menu
    for (int i = 0; i < numChoices; i++)
    {
        this->menuItems.push_back(menuItems[i]);
    }
}

int TextMenu::getChoice()
{
    string inputStr;
    string junk;
    int choice;
    // display the menu
    displayMenu();
    // request choice and read as string
    cout << "Please enter your choice: ";
    cin >> inputStr;
    getline(cin, junk); // ensuring no issues with leftover data

    // validateChoice
    choice = validateChoice(inputStr);

    // repeat as needed
    while (choice == -1)
    {
        cout << "That was not a valid choice. You must enter a number between 1 and " << menuItems.size();
        cout << "\nPlease enter your choice: ";
        cin >> inputStr;
        getline(cin, junk); // ensuring no issues with leftover data
        choice = validateChoice(inputStr);
    }

    return choice;
}

void TextMenu::displayMenu()
{
    cout << "\n";
    for (int i = 0; i < menuItems.size(); i++)
    {
        cout << "  " << setw(2) << i + 1 << ") " << menuItems[i] << "\n";
    }
    cout << "\n";
}

int TextMenu::validateChoice(string strChoice)
{
    int choice = -1;
    bool isNum = true;
    // is it a number?
    for (int i = 0; isNum && i < strChoice.length(); i++)
    {
        isNum = isdigit(strChoice[i]);
    }
    if (isNum)
    {
        // convert to number and check range
        int tempNum = stoi(strChoice);

        // if in right range, put it in choice
        if (tempNum > 0 && tempNum <= menuItems.size())
        {
            choice = tempNum;
        }
    }

    return choice;
}
