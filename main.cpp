#include <iostream>
#include "Windows.h"
#include <chrono>
#include <thread>

#include "2D_graphics/2D_graphics.h"
#include "2D_graphics/timer.h"
#include "2D_graphics/game_pad.h"
#include "config.h"
#include "camera.h"
#include "ran.h"
#include "menu.h"
#include "puck.h"
#include "gameObject.h"
#include "2D_graphics/UDP_com.h"
#include "gameNetworking.h"

using namespace std;

// Global debug variable to skip loading
bool QUICK_LAUNCH = false;

// Function prototypes
void initializeProgram();

void writeConfigSettings();

void launchDirectXWindow();

void loadingWindow();

void menus();

// Global variables
config *configFile;

int main() {
    initializeProgram();
    configFile->print();

//    cout << "Modifying a value from array \n\n";
//    configFile->setValue("WINDOW_HEIGHT", "CATs");
//    configFile->setValue("WINDOW_WIDTH", "uWu what's this bois");
//    configFile->print();

    menus();

   Sleep(1000);

    cout << "End of game!" << endl;
    return 0;
}

void initializeProgram() {
    configFile = getConfig();

    writeConfigSettings();

    launchDirectXWindow();

    initialize_graphics();

    clear();
    loadingWindow();

    double CENTER_WIDTH = configFile->getDoubleValue("WINDOW_WIDTH") / 2;
    double CENTER_HEIGHT = configFile->getDoubleValue("WINDOW_HEIGHT") / 2;
    initCamera(vector(CENTER_WIDTH, CENTER_HEIGHT));
}

void writeConfigSettings() {
    fstream DirectXWindowConfigFile;
    DirectXWindowConfigFile.open("2D_graphics/DirectX_Window/window_size.txt");

    if (DirectXWindowConfigFile.is_open()) {
        DirectXWindowConfigFile << "window width(use screen width for full - screen mode) :\n";
        DirectXWindowConfigFile << configFile->getDoubleValue("WINDOW_WIDTH") << "\n";
        DirectXWindowConfigFile << "window height(use screen height for full - screen mode) :\n";
        DirectXWindowConfigFile << configFile->getDoubleValue("WINDOW_HEIGHT") << "\n";
        DirectXWindowConfigFile << "use border(1 - yes, 0 - no) (use 0 for full - screen mode) :\n";
        DirectXWindowConfigFile << 1;
    }
    DirectXWindowConfigFile.close();
}

void launchDirectXWindow() {// Launches new DirectX process.
    ShellExecuteA(NULL, NULL, "2D_graphics\\DirectX_window\\launch_DirectX.bat", NULL, NULL,
                  SW_SHOW); // bat file kills any existing DirectX processes, then launches a new one.
    this_thread::sleep_for(
            chrono::seconds{1}); // Delay needed to allow DirectX window to launch fully. 2 seconds works consistently
}

void loadingWindow() {
    int blackBackground;
    create_sprite((char *) "assets/images/black.png", blackBackground);

    int gameLogo;
    create_sprite((char *) "assets/images/gameLogo2.png", gameLogo);

    double CENTER_WIDTH = configFile->getDoubleValue("WINDOW_WIDTH") / 2;
    double CENTER_HEIGHT = configFile->getDoubleValue("WINDOW_HEIGHT") / 2;

    if(QUICK_LAUNCH) return; // If quick launch is enabled skip drawing process

    vector vCamera(0, 0);
    double scale = 250, cameraAngle = 0;
    setCamera(vCamera, scale, cameraAngle);

    gameObject loadingScreen;
    loadingScreen.loadTable((char *) "loading.txt");
    loadingScreen.puckArray[0]->setVelocity(vector(3, 0));
    loadingScreen.setLoadingScreen(true);
    for (int i = 0; i < 150; ++i) {
        clear();
        draw_sprite(blackBackground, 0, 0, 0, -1);
        draw_sprite(gameLogo, CENTER_WIDTH, CENTER_HEIGHT, 0, 1);

        loadingScreen.draw();
        loadingScreen.update();
        update();
    }
}

void menus() {
    int menuResult; // Stores result from menu
    while (mainMenu()) {
        // While mainMenu returns a value > 0 (only when play game has been selected)
        game();

        cout << "Thanks for playing.\n";
    }
}