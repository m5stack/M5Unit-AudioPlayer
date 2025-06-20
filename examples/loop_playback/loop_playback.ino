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

void displayCurrentAudioNumber(void *pvParameters)
{
    uint16_t audioNumber = 0;
    while (1) {
        audioNumber = audioplayer.getCurrentAudioNumber();
        if (audioNumber != AUDIO_PLAYER_STATUS_ERROR) {
            M5.Display.fillRect(0, 40, 320, 20, BLACK);
            M5.Display.drawString("Current Audio Num:" + String(audioNumber), 0, 40);
        }
    }
}

void setup()
{
    auto cfg            = M5.config();
    cfg.serial_baudrate = 115200;
    M5.begin(cfg);
    M5.Power.setExtOutput(true);
    M5.Display.setTextSize(2);
    M5.Lcd.setTextFont(&fonts::efontCN_12);
    M5.Display.drawString("Unit AudioPlayer Test V0.1", 0, 0);

    int8_t port_a_pin1 = -1, port_a_pin2 = -1;
    port_a_pin1 = M5.getPin(m5::pin_name_t::port_a_pin1);
    port_a_pin2 = M5.getPin(m5::pin_name_t::port_a_pin2);
    Serial.printf("getPin: RX:%d TX:%d\n", port_a_pin1, port_a_pin2);
    while (!audioplayer.begin(&Serial1, port_a_pin1, port_a_pin2)) {
        M5.Display.setTextColor(RED);
        M5.Display.drawString("AudioPlayer or SD card", 0, 40);
        M5.Display.drawString("may not be connected", 0, 60);
        Serial.println("AudioPlayer or SD card may not be connected");
        delay(1000);
    }
    M5.Display.fillRect(0, 40, 320, 55, BLACK);
    M5.Display.setTextColor(WHITE);
    Serial.println("Unit AudioPlayer is ready");
    audioplayer.setVolume(30);
    audioplayer.selectAudioNum(1);
    audioplayer.setPlayMode(AUDIO_PLAYER_MODE_ALL_LOOP);
    M5.Display.drawString("Current Audio Num:1", 0, 40);
    audioplayer.playAudio();
    xTaskCreate(displayCurrentAudioNumber, "displayCurrentAudioNumber", 4096, NULL, 1, NULL);
}

void loop()
{
    M5.update();

    if (M5.BtnA.wasReleased()) {
        Serial.println("BtnA pressed");

    } else if (M5.BtnB.wasReleased()) {
        Serial.println("BtnB pressed");

    } else if (M5.BtnC.wasReleased()) {
        Serial.println("BtnC pressed");
    }
}
