/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

/*
 * @Hardwares: M5CoreS3/M5Core2/Basic/Fire/Gray(PortA) + Unit AudioPlayer
 * @Dependent Library:
 * M5GFX: https://github.com/m5stack/M5GFX
 * M5Unified: https://github.com/m5stack/M5Unified
 * M5Unit-AudioPlayer:https://github.com/m5stack/M5Unit-AudioPlayer
 */

#include <M5Unified.h>
#include <unit_audioplayer.hpp>

AudioPlayerUnit audioplayer;

void setup()
{
    auto cfg            = M5.config();
    cfg.serial_baudrate = 115200;
    M5.begin(cfg);
    M5.Power.setExtOutput(true);
    M5.Lcd.setTextFont(&fonts::DejaVu18);
    M5.Display.drawString("Unit AudioPlayer Example", 0, 0);

    int8_t port_a_pin1 = -1, port_a_pin2 = -1;
    port_a_pin1 = M5.getPin(m5::pin_name_t::port_a_pin1);
    port_a_pin2 = M5.getPin(m5::pin_name_t::port_a_pin2);
    Serial.printf("getPin: RX:%d TX:%d\n", port_a_pin1, port_a_pin2);
    while (!audioplayer.begin(&Serial1, port_a_pin1, port_a_pin2)) {
        delay(1000);
    }
    Serial.println("Unit AudioPlayer is ready");
    audioplayer.setVolume(15);
    audioplayer.setPlayMode(AUDIO_PLAYER_MODE_SINGLE_STOP);
    audioplayer.playAudio();
    audioplayer.selectAudioNum(1);
    M5.Display.drawString("Audio Num:1", 0, 80);
    M5.Display.drawString("<            >||           >      ", 47, 215);
}

void loop()
{
    static uint8_t lastPlayStatus = 255;
    static uint8_t lastAudioNum = 0, lastVolume = 0;
    static bool refreshAudioNum = false;

    uint8_t playStatus = audioplayer.checkPlayStatus();
    uint8_t volume     = audioplayer.getVolume();
    uint8_t audioNum   = 0;

    if (refreshAudioNum) {
        audioNum        = audioplayer.getCurrentAudioNumber();
        refreshAudioNum = false;
    } else {
        audioNum = lastAudioNum;
    }

    if (playStatus != lastPlayStatus) {
        static String playStatusStr;
        if (playStatus == AUDIO_PLAYER_STATUS_PAUSED) {
            playStatusStr = "Paused";
        } else if (playStatus == AUDIO_PLAYER_STATUS_STOPPED) {
            playStatusStr = "Stopped";
        } else if (playStatus == AUDIO_PLAYER_STATUS_PLAYING) {
            playStatusStr = "Playing";
        }

        M5.Display.fillRect(0, 40, 320, 20, BLACK);
        M5.Display.drawString("Play Status:" + playStatusStr, 0, 40);

        lastPlayStatus = playStatus;
    }

    if (volume != lastVolume) {
        M5.Display.fillRect(0, 120, 320, 20, BLACK);
        M5.Display.drawString("Volume:" + String(volume), 0, 120);
        lastVolume = volume;
    }

    if (audioNum != lastAudioNum && audioNum != AUDIO_PLAYER_STATUS_ERROR) {
        M5.Display.fillRect(0, 80, 320, 20, BLACK);
        M5.Display.drawString("Audio Num:" + String(audioNum), 0, 80);
        lastAudioNum = audioNum;
    }

    M5.update();

    if (M5.BtnA.wasReleased()) {
        Serial.println("BtnA pressed");
        audioplayer.previousAudio();
        refreshAudioNum = true;
    } else if (M5.BtnB.wasReleased()) {
        Serial.println("BtnB pressed");
        if (playStatus == AUDIO_PLAYER_STATUS_PLAYING) {
            audioplayer.pauseAudio();
            Serial.println("pause_audio");
        } else {
            audioplayer.playAudio();
            Serial.println("play_audio");
        }
    } else if (M5.BtnC.wasReleased()) {
        Serial.println("BtnC pressed");
        audioplayer.nextAudio();
        refreshAudioNum = true;
    }
}
