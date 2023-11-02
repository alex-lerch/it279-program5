// Class to handle display of a menu for a text-based program and get validated choices from the user
// By Mary Elaine Califf
#ifndef TEXT_MENU_H
#define TEXT_MENU_H

#include <vector>
#include <string>
using namespace std;

class TextMenu
{
private:
    vector<string> menuItems;

public:
    // default constructor currently does nothing -- using a setup method to init menu
    TextMenu() {}

    // Sets up menu with the specified options, after clearing any current options
    void setUp(int numChoices, string *menuItems);

    // Displays the menu, gets a valid user choice, and returns the result (range of 1 to num menu items)
    int getChoice();

private:
    // displays the menu choices
    void displayMenu();

    // validate the choice -- returning -1 if not valid
    int validateChoice(string strChoice);
};

#endif