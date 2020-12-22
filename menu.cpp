//
// Created by Patrick on 2020-11-24.
//

#include "Windows.h"
#include "UI.h"
#include "menu.h"
#include "2D_graphics/2D_graphics.h"
#include "2D_graphics/game_pad.h"
#include "charFunctions.h"
#include "binaryIO.h"

extern config *configFile;

// Values used for spacing buttons in this file.
double CENTER_X;
double TOP_BUTTON;
double SPACING = 125;

bool GAMEPAD_ENABLED = false;
const int MAX_CHAR = 16; // Maximum number of characters to be able to write

using namespace std;

void updateGamepad(int *GS);

int mainMenu() {
    // Values used in calculations
    CENTER_X = configFile->getDoubleValue("WINDOW_WIDTH") / 2;
    TOP_BUTTON = 0.6875 * configFile->getDoubleValue("WINDOW_HEIGHT");

    // Checks if gamepad should be checked
    GAMEPAD_ENABLED = configFile->getBoolValue("GAMEPAD_ENABLED");

    // Creates menu object.
    UI menu;
    menu.clear();

    // Creates text on top of menu
    vector t1(CENTER_X-250, TOP_BUTTON + SPACING*1.5);
    menu.addText(t1, "Main Menu",2);

    vector b1(CENTER_X, TOP_BUTTON - SPACING * 0);
    vector b2(CENTER_X, TOP_BUTTON - SPACING * 1);
    vector b3(CENTER_X, TOP_BUTTON - SPACING * 2);
    vector b4(CENTER_X, TOP_BUTTON - SPACING * 3);
    vector b5(CENTER_X, TOP_BUTTON - SPACING * 4);

    // Creates 4 buttons in menu.
    char *buttonPlay = (char *) "Play";
    char *buttonNetworkSettings = (char *) "Network Settings";
    char *buttonGameSettings = (char *) "Game Settings";
    char *buttonScoreboard = (char *) "Scoreboard";
    char *buttonExit = (char *) "Exit";

    menu.addButton(b1, buttonPlay);
    menu.addButton(b2, buttonNetworkSettings);
    menu.addButton(b3, buttonGameSettings);
    menu.addButton(b4, buttonScoreboard);
    menu.addButton(b5, buttonExit);

    // Menu interactions;
    int GS[N_GS];

    // Loops to allow for dynamic movement inside menu
    while (true) {
        clear();
        bool select = false; // Variable used to detect if button was selected.

        updateGamepad(GS);
        menuMovement(menu, GS, select);

        // If the selection button was pressed, want to go and do that.
        if(select) {
            // Gets name of button that was pressed and then use it to call next function.
            char *buttonName = menu.getButtonName();
            if (buttonName != nullptr) {
                if (compareChars(buttonName, buttonPlay)) {
                    return 1;
                } else if (compareChars(buttonName, buttonNetworkSettings)) {
                    networkSettingsMenu();
                } else if (compareChars(buttonName, buttonGameSettings)) {
                        gameSettingsMenu();
                } else if (compareChars(buttonName, buttonScoreboard)) {
                        scoreboardMenu();
                } else if (compareChars(buttonName, buttonExit)) {
                    return 0;
                }
            }
        }
        menu.draw();
        update();
    }
}

void updateGamepad(int *GS) {// Update gamepad.
    if(gamepad_state(GS, 1) == 1){ // This will return 1 if issue with function
        // Want to ignore invalid gamepad state so will set everything equal 0;
        for (int i = 0; i < N_GS; ++i) {
            GS[i] = 0;
        }
    }
}

void menuMovement(UI &menu, const int *GS, bool &selection) {
    bool keyPress = false;
    int millisecondsDelay = 200;
    if ((GAMEPAD_ENABLED && GS[BUTTON_DOWN]) || KEY('S')) {
        menu.moveDown();
        keyPress = true;
    }

    if ((GAMEPAD_ENABLED && GS[BUTTON_UP]) || KEY('W')) {
        menu.moveUp();
        keyPress = true;
    }

    if ((GAMEPAD_ENABLED && GS[BUTTON_RIGHT]) || KEY('D') || KEY(0x0D)) { // (Checks for D or ENTER keys)
        selection = true;
        keyPress = true;
    }

    if ((GAMEPAD_ENABLED && GS[BUTTON_LEFT]) || KEY('A')) {
//        cout << "Back\n";
//            menu.back();
        Sleep(250);
    }
    if (keyPress) {
        PlaySoundA("assets/audio/ding2.wav", NULL, SND_ASYNC);
        Sleep(millisecondsDelay);
    }
}

// TODO: Properly implement these two items. Currently unsure of what would even need to be done for these.
void networkSettingsMenu(){
    // Creates text on top of menu
    vector t1(CENTER_X-250, TOP_BUTTON + SPACING*2);


    UI networkSettings;
    networkSettings.addText(t1, "Network Settings", 1); // Draw text at top for settings

    clear();
    const char *buttonNetworking = "Networking = ";
    const char *buttonHostIP = "Host IP = ";
    const char *buttonClientIP = "Client IP = ";
    const char *buttonIPv4 = "IPv4 = ";

    const char *buttonSave = "Save";
    const char *buttonExit = "Exit";


    vector position = vector(200, 600);
    networkSettings.addButton(position, (char *) buttonNetworking);
    int textIDNetworking = networkSettings.addText(position + vector(175, 25), (char *) configFile->getCharValue("NETWORK_GAME"), 1, "NETWORK_GAME");

    position.y -= 100; // Lowers y coord of position by 100
    networkSettings.addButton(position, (char *) buttonHostIP);
    int textIDHostIP = networkSettings.addText(position + vector(175, 25), (char *) configFile->getCharValue("HOST_IP"), 1, "HOST_IP");

    position.y -= 100; // Lowers y coord of position by 100
    networkSettings.addButton(position, (char *) buttonClientIP);
    int textIDClientIP = networkSettings.addText(position + vector(175, 25), (char *) configFile->getCharValue("CLIENT_IP"), 1, "CLIENT_IP");

    position.y -= 100; // Lowers y coord of position by 100
    networkSettings.addButton(position, (char *) buttonIPv4);
    int textIDIPv4 = networkSettings.addText(position + vector(175, 25), (char *) configFile->getCharValue("IPv4"), 1, "IPv4");


    // Buttons to save config options or exit without saving
    networkSettings.addButton(vector(200, 200), (char *) buttonSave);
    networkSettings.addButton(vector(200, 100), (char *) buttonExit);

    int GS[N_GS]; // Array for gamepad.

    // Loops to allow for dynamic movement inside menu
    while (true) {
        clear();
        bool select = false; // Variable used to detect if button was selected.

        // Update gamepad.
        updateGamepad(GS);

        menuMovement(networkSettings, GS, select);

        // If the selection button was pressed, want to go and do that.
        if(select) {
            // Gets name of button that was pressed and then use it to call next function.
            char *buttonName = networkSettings.getButtonName();
            if (buttonName != nullptr) {

                // TODO: See if there's a neater solution for this.
                if (compareChars(buttonName, buttonNetworking)) {
                    modifyValue(networkSettings, textIDNetworking); // Modify value stored in this button
                } else if (compareChars(buttonName, buttonHostIP)) {
                    modifyValue(networkSettings, textIDHostIP); // Modify value stored in this button
                } else if (compareChars(buttonName, buttonClientIP)) {
                    modifyValue(networkSettings, textIDClientIP); // Modify value stored in this button.
                } else if (compareChars(buttonName, buttonIPv4)) {
                    modifyValue(networkSettings, textIDIPv4); // Modify value stored in this button.
                } else if (compareChars(buttonName, buttonSave)) {
                    saveValues(networkSettings); // Save values in config object stored in text objects
                    configFile->saveFile();
                    break;
                    // Save config then exits.
                } else if (compareChars(buttonName, buttonExit)) {
                    break; // Exits and returns to main menu
                }
            }
        }
        networkSettings.draw();
        update();
    }
}

struct scoreInfo {
    char player[2][16];
    int iScore[2];
};

void printScoreboardValues(const scoreInfo *scoreInfoObj, UI &scoreboard, int startNumber, int endNumber);

scoreInfo *readScoreboard(binaryIO &fileObj){
    int numberOfEntries = fileObj.getLengthOfFile() / 48; // Gets the number of entries into this file.
    scoreInfo *scoreInfoObj = new scoreInfo[numberOfEntries];

    char *buffer = fileObj.readData(); // Read file into buffer
    int sizeOfBuffer = 0; // Stores size of buffer

    char *pChar; // Pointer for characters
    int *pInt; // Pointer for ints

    for (int i = 0; i < numberOfEntries; ++i) {
        // Stores names of both players into this object.
        for (int playerNumber = 0; playerNumber < 2; ++playerNumber) {
            for (int characterInName = 0; characterInName < 16; ++characterInName) {
                pChar = (char *) (buffer + sizeOfBuffer);
                scoreInfoObj[i].player[playerNumber][characterInName] = *pChar;
                sizeOfBuffer += sizeof(*pChar);
            }
        }
        // Stores scores of both players into object.
        for (int playerNumber = 0; playerNumber < 2; ++playerNumber) {
            pInt = (int *) (buffer + sizeOfBuffer);
            scoreInfoObj[i].iScore[playerNumber] = *pInt;
            sizeOfBuffer += sizeof(*pInt);
        }
        // Increment buffer another 8 characters to get to 48.
        sizeOfBuffer += 8;
    }
    return scoreInfoObj;
}

void scoreboardMenu(){
    // Scoreboard IOs
    // Loads last 6 items from buffer.
    binaryIO scoreFile("scoreboard.bin");
    int numOfEntries = scoreFile.getLengthOfFile() / 48;
    scoreInfo *scoreInfoObj = readScoreboard(scoreFile);

    UI scoreboard;

    // Creates text on top of menu
    vector t1(CENTER_X-150, TOP_BUTTON / 0.6875 * 0.75 + 150);
    scoreboard.addText(t1, "Scoreboard",1.5); // Draw text at top for scoreboard

    // Creates text for the actual scores
    if(numOfEntries > 6) { // If there are more than 6 items on load the last 6 items
        // Loads last items first.
        printScoreboardValues(scoreInfoObj, scoreboard, numOfEntries,numOfEntries - 6);
    } else {
        // Loads all items, last to first.
        printScoreboardValues(scoreInfoObj, scoreboard, numOfEntries,0);
    }

    // Create exit button.
    const char *buttonExit = "Exit";
    scoreboard.addButton(vector(1150, 75), buttonExit);

    int GS[N_GS]; // Holds GS
    while(true){
        clear();
        bool select = false;

        updateGamepad(GS);
        menuMovement(scoreboard, GS, select);
        if(select) break; // As there's only an exit button if it was pressed we want to exit.
        scoreboard.draw();
        update();

        Sleep(100); // Can sleep here to slow down how often this loop loops
    }

    // Safe deletes dynamic array created.
    if(scoreInfoObj != nullptr) {
        delete[] scoreInfoObj;
        scoreInfoObj = nullptr;
    }
}

void printScoreboardValues(const scoreInfo *scoreInfoObj, UI &scoreboard, int startNumber, int endNumber) {
    vector textPosition(25, TOP_BUTTON / .6875 * .75);

    for (int i = startNumber; i > endNumber; --i) {
        char bufferText[50];
        sprintf(bufferText, "%s vs. %s     %d - %d", scoreInfoObj[i - 1].player[0], scoreInfoObj[i - 1].player[1],
                scoreInfoObj[i - 1].iScore[0], scoreInfoObj[i - 1].iScore[1]); // Formats buffer text
        scoreboard.addText(textPosition, (char *) bufferText, 1); // Adds the formatted info to a text object
        textPosition.y -= 100; // Offsets following text value.

        cout << "i = " << i << "  bufferText = " << bufferText << "\n";
    }
}

void gameSettingsMenu() {
    // Creates text on top of menu
    vector t1(CENTER_X-250, TOP_BUTTON + SPACING*2);


    UI gameSettings;
    gameSettings.addText(t1, "Game Settings", 1); // Draw text at top for settings

    clear();
    const char *buttonNumberOfPlayers = "Player # = ";
    const char *buttonAIDifficulty = "AI difficulty = ";
    const char *buttonPlayerName1 = "Player Name1";
    const char *buttonPlayerName2 = "Player Name2";
    const char *buttonMap = "Map";

    const char *buttonSave = "Save";
    const char *buttonExit = "Exit";


    vector position = vector(200, 700);

    gameSettings.addButton(position, (char *) buttonNumberOfPlayers);
    const vector textOffset(200, 25);
    int textIDnumOfPlayers = gameSettings.addText(position + textOffset, (char *) configFile->getCharValue("NUMBER_OF_PLAYERS"), 1, "NUMBER_OF_PLAYERS");

    position.y -= 100; // Lowers y coord of position by 100
    gameSettings.addButton(position, (char *) buttonAIDifficulty);
    int textIDAIDifficulty = gameSettings.addText(position + textOffset, (char *) configFile->getCharValue("AI_LEVEL"), 1, "AI_LEVEL");

    position.y -= 100; // Lowers y coord of position by 100
    gameSettings.addButton(position, (char *) buttonPlayerName1);
    int textIDPlayerName1 = gameSettings.addText(position + textOffset, (char *) configFile->getCharValue("PLAYER_NAME1"), 1, "PLAYER_NAME1");

    position.y -= 100; // Lowers y coord of position by 100
    gameSettings.addButton(position, (char *) buttonPlayerName2);
    int textIDPlayerName2 = gameSettings.addText(position + textOffset, (char *) configFile->getCharValue("PLAYER_NAME2"), 1, "PLAYER_NAME2");

    position.y -= 100; // Lowers y coord of position by 100
    gameSettings.addButton(position, (char *) buttonMap);
    int textIDMap = gameSettings.addText(position + textOffset, (char *) configFile->getCharValue("TABLE_FILE"), 1, "TABLE_FILE");

    // Buttons to save config options or exit without saving
    gameSettings.addButton(vector(200, 200), (char *) buttonSave);
    gameSettings.addButton(vector(200, 100), (char *) buttonExit);

    int GS[N_GS]; // Array for gamepad.

    // Loops to allow for dynamic movement inside menu
    while (true) {
        clear();
        bool select = false; // Variable used to detect if button was selected.

        // Update gamepad.
        updateGamepad(GS);

        menuMovement(gameSettings, GS, select);

        // If the selection button was pressed, want to go and do that.
        if(select) {
            // Gets name of button that was pressed and then use it to call next function.
            char *buttonName = gameSettings.getButtonName();
            if (buttonName != nullptr) {
                // TODO: See if there's a neater solution for this.
                if (compareChars(buttonName, buttonNumberOfPlayers)) {
                    modifyValue(gameSettings, textIDnumOfPlayers); // Modify value stored in this button
                } else if (compareChars(buttonName, buttonAIDifficulty)) {
                    modifyValue(gameSettings, textIDAIDifficulty); // Modify value stored in this button
                } else if (compareChars(buttonName, buttonPlayerName1)) {
                    modifyValue(gameSettings, textIDPlayerName1); // Modify value stored in this button
                } else if (compareChars(buttonName, buttonPlayerName2)) {
                    modifyValue(gameSettings, textIDPlayerName2); // Modify value stored in this button
                } else if (compareChars(buttonName, buttonMap)) {
                    modifyValue(gameSettings, textIDMap); // Modify value stored in this button
                } else if (compareChars(buttonName, buttonSave)) {
                    saveValues(gameSettings); // Save values in config object stored in text objects
                    configFile->saveFile();
                    break;
                    // Save config then exits.
                } else if (compareChars(buttonName, buttonExit)) {
                    break; // Exits and returns to main menu
                }
            }
        }
        gameSettings.draw();
        update();
    }
}

/// Update item in text field
void updateTextField(char *value, bool &finishedEditing) {
    int length = lengthOfChars(value);
    bool keyPressed = false;

    do {
        if (KEY(VK_BACK) && length > 0) { // Remove last character if there's at least a single character
            value[--length] = '\0';
            keyPressed = true; // Tracks if a key was pressed and if so add a delay
        } else if (KEY(VK_RETURN)) { // If enter key is pressed then editing is finished
            finishedEditing = true;
            keyPressed = true; // Tracks if a key was pressed and if so add a delay
        } else {
            if(length < MAX_CHAR - 1) { // If neither one of these keys were pressed then check keys "A-Z" "." "0-9"
                // Checks if '.' was pressed.
                if (KEY(VK_OEM_PERIOD)) {
                    value[length++] = '.';
                    value[length] = '\0';
                    keyPressed = true; // Tracks if a key was pressed and if so add a delay
//                    break;
                }

                for (int c = 48; c <= 57; ++c) {
                    if (KEY(c)) {
                        value[length++] = (char) c;
                        value[length] = '\0';
                        keyPressed = true; // Tracks if a key was pressed and if so add a delay
//                        break;
                    }
                }

                // Checks for the letter keys.
                for (int c = 65; c <= 90; ++c) {
                    if (KEY(c)) { // This key was pressed.
                        // Checks if the shift key was pressed, otherwise character should be lowercase
                        if (!(KEY(VK_LSHIFT) || KEY(VK_RSHIFT) || KEY(VK_CAPITAL))) c += 32;
                        value[length++] = (char) c;
                        value[length] = '\0';
                        keyPressed = true; // Tracks if a key was pressed and if so add a delay
//                        break;
                    }
                }
            }
        }
    } while(false); // Loops once.
    if(keyPressed){
        Sleep(250);
    }

}

/// Modifies value stored in UI text object.
void modifyValue(UI &UIObj, const int textID) {
    char textValue[MAX_CHAR]{};

    // Copy one string to the other.
    int length = lengthOfChars(UIObj.getText(textID));
    length =  (length < MAX_CHAR) ? length : MAX_CHAR;
    for (int i = 0; i < length; ++i) {
        textValue[i] = UIObj.getText(textID)[i];
    }
    //    strcpy(textValue, UIObj.getText(textID));

    bool finishedEditing = false;
    while(!finishedEditing) {
        clear();
        updateTextField(textValue, finishedEditing);
        UIObj.updateText(textValue, textID);
        UIObj.draw();
        update();
    }
    cout << "huh.\n";
}

/// Save values from config
void saveValues(UI &UIObj){
    for (int i = 0; i < UIObj.getNumberOfTextObjects(); ++i) {
        // Checks every text object and save values if needed.

        if (!compareChars(UIObj.getTextConfig(i),"")) { // "" is the default value, don't want to use this.
            configFile->setValue(UIObj.getTextConfig(i), UIObj.getText(i));
        }
    }
}

