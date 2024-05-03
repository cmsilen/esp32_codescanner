//
// Created by Carlo on 14/03/2024.
//

#ifndef _LIB_CODESCANNER_H_INCLUDED
#define _LIB_CODESCANNER_H_INCLUDED

#include "Arduino.h"
#include <HardwareSerial.h>
#include <driver/uart.h>

class CodeScanner{
private:
    bool configMode;
    bool verbose;
    int delayCommands;
    bool lowerCase;
    bool upperCase;
    bool digits;
    bool rawOutput;

    void verbosePrint();
    void verbosePrint(String str);
    bool sendCommand(String cmd, String verbmsg);
    bool sendQuery(char* query, char* response_data);
    bool verifyChar(char value);
public:
    enum Delay{DELAY_0MS, DELAY_100MS, DELAY_1000MS, DELAY_10000MS};
    enum Volume{VOLUME_0, VOLUME_20, VOLUME_40, VOLUME_60, VOLUME_80, VOLUME_100, VOLUME_ERR};
    enum Tone{TONE_1, TONE_2, TONE_3, TONE_4, TONE_5, TONE_ERR};

    CodeScanner();

    //impostazioni della libreria
    void setVerbose(bool isverbose);
    void setLowerCaseCharOutput(bool value);
    void setUpperCaseOutput(bool value);
    void setDigitsOutput(bool value);
    void setRawOutput(bool value);

    //impostazioni dello scanner
    bool setConfigMode();
    bool closeConfigMode();
    bool saveConfig();
    bool restoreConfig();
    bool setManualMode();
    bool setContinuousMode();
    bool setInductionMode();
    bool setContinuousSleep();
    bool setScanDelay(Delay delay);
    bool set1DCodesOn();
    bool set1DCodesOff();
    bool set2DCodesOn();
    bool set2DCodesOff();
    bool setStartupVolume(Volume volume);
    bool setDecodingVolume(Volume volume);
    bool setConfigVolume(Volume volume);
    bool setStartupTone(Tone tone);
    bool setDecodingTone(Tone tone);
    bool setConfigTone(Tone tone);
    bool setHorizontalMirroringOn();
    bool setHorizontalMirroringOff();
    bool setVerticalMirroringOn();
    bool setVerticalMirroringOff();
    bool enableCode39();
    bool disableCode39();
    bool enableCode128();
    bool disableCode128();
    bool enableUPC_EAN_JAN();
    bool disableUPC_EAN_JAN();
    bool enableCode93();
    bool disableCode93();
    bool enableInterleaved_2_of_5();
    bool disableInterleaved_2_of_5();
    bool enableCodabar();
    bool disableCodabar();
    bool enableCode11();
    bool disableCode11();
    bool enableMatrix_2_of_5();
    bool disableMatrix_2_of_5();
    bool enableMSI();
    bool disableMSI();
    bool enableIndustrial_2_of_5();
    bool disableIndustrial_2_of_5();
    bool enableGS1();
    bool disableGS1();
    bool enableISBN();
    bool disableISBN();
    bool enableISSN();
    bool disableISSN();
    bool enableCODE_32();
    bool disableCODE_32();
    bool enableQR();
    bool disableQR();

    bool setPreferredConfig();

    //informazioni dallo scanner
    int getBaudRate();
    Volume getStartupVolume();
    Volume getDecodingVolume();
    Volume getConfigVolume();
    Tone getStartupTone();
    Tone getDecodingTone();
    Tone getConfigTone();
    bool getHorizontalMirroring();
    bool getVerticalMirroring();
    bool getCode39();
    bool getCode128();
    bool getUPC_EAN_JAN();
    bool getCode93();
    bool getInterleaved_2_of_5();
    bool getCodabar();
    bool getCode11();
    bool getMatrix_2_of_5();
    bool getMSI();
    bool getIndustrial_2_of_5();
    bool getGS1();
    bool getISBN();
    bool getISSN();
    bool getCODE_32();
    bool getQR();

    //lettura codici
    int read(char* buffer);
    int read(char* buffer, int bytes);
};

#endif //_LIB_CODESCANNER_H_INCLUDED