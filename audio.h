//
// Created by Patrick on 2020-12-02.
//

#ifndef INC_2CARS2RACING_AUDIO_H
#define INC_2CARS2RACING_AUDIO_H


class audio {
    char *p_buffer; // a dynamic memory buffer of N_buffer bytes that stores the binary file
    unsigned int N_buffer; // the number of bytes for the memory buffer
    char *p_data; // a pointer to the audio data in p_buffer
    unsigned int N_data; // the number of bytes for the audio data
public:
    unsigned short int NumChannels; // the number of audio channels (1 - mono, 2 - stereo), 2 byte int
    unsigned int SampleRate; // the number of samples per second for the audio data (Hz), 4 byte int
public:
    audio(char *file_name); // a constructor that initializes all member variables using a wave file
    ~audio();

    void play(); // use the Windows PlaySoundA function to play the wave file in p_buffer
};


#endif //INC_2CARS2RACING_AUDIO_H
