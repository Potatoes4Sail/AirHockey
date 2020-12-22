//
// Created by Patrick on 2020-12-02.
//

#include <iostream>
#include <fstream>
#include "audio.h"

#include <Windows.h>

// for PlaySoundA
#include <mmsystem.h>
#include "config.h"

#pragma comment(lib, "winmm.lib") // links a windows library

extern config* configFile; // Used to check if mute flag is enabled. 
bool bMute = true;

audio::audio(char *file_name) {
    // Open file, find size of file and then load into memory, then go to data location and get the size of the data.
    std::ifstream fin;
    fin.open(file_name, std::ios::binary);

    if (fin.is_open()) {
        // Get length of file
        fin.seekg(0, std::ios::end);
        N_buffer = fin.tellg();

        fin.seekg(0, std::ios::beg);
        p_buffer = new char[N_buffer];
        fin.read(p_buffer, N_buffer);
        N_data = N_buffer - 44;
        p_data = (p_buffer + 44);
    } else {
        std::cout << "Failed to open file. Exiting now.\n";
        exit(-1);
    }
    fin.close();

    // Populate file properties from object in memory.
    NumChannels = *((unsigned short int *) (p_buffer + 22));
    SampleRate = *((unsigned int *) (p_buffer + 24));

    bMute = configFile->getBoolValue("MUTE");
}

audio::~audio() {
    delete[] p_buffer;
}

void audio::play() {
    if(!bMute) PlaySoundA(this->p_buffer, NULL, SND_MEMORY | SND_ASYNC);
    // If config file doesn't have "mute" flag set to false, then play sounds
}
