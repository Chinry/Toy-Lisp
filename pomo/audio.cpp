#include "Audio.hpp"
#include <iostream>

Audio::Audio()
{
    PaError err;
    PaStreamParameters outputParameters;
    
    err = Pa_Initialize();
    if (err != paNoError) goto error;

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if(outputParameters.device == paNoDevice) goto error;

    outputParamters.channelCount = 2;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(&stream,
                        NULL,
                        &outputParameters,
                        SAMPLE_RATE)



    return err;

error:
    Pa_Terminate();
    std::cout << "Error Message" << Pa_GetErrorText(err);
    return err;
}

void Audio::beep()
{

}

static int paTestCallback(const void *inputBuffer,
                          void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData)
{
    return 1;
}