#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <opus.h>
#include <stdio.h>
#include "OpusCodecConfiguration.h"

// int16_t opusArray[244];

unsigned char cbits[MAX_ENCODED_FRAME_SIZE];

/*Holds the state of the encoder and decoder */
OpusEncoder *encoder;
int err, nbBytes;

uint8_t opusEncodeCreate()
{
    /*Create a new encoder state */
    encoder = opus_encoder_create(SAMPLE_RATE, CHANNELS, APPLICATION, &err);
    if (err < 0)
    {
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }
}

uint8_t opusBitrateSet()
{
    err = opus_encoder_ctl(encoder, OPUS_SET_BITRATE(BITRATE));
    if (err < 0)
    {
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }
}

uint8_t opusEncodeFrame(int16_t *pcmData, uint8_t *opusData_out)
{
    /* Encode the frame. */
    nbBytes = opus_encode(encoder, (opus_int16 *)pcmData, FRAME_SIZE, (unsigned char *)opusData_out, MAX_ENCODED_FRAME_SIZE);
    if (nbBytes < 0)
    {
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }
}