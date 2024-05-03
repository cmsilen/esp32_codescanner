//
// Created by Carlo on 14/03/2024.
//
#include "CodeScanner.h"

#define ACK 6
#define RESPONSE_SIZE 9
#define RESPONSE_CMD_SIZE 4
#define RESPONSE_DATA_SIZE 4

void CodeScanner::verbosePrint() {
    if(verbose){
        Serial.println("ERRORE");
    }
}

void CodeScanner::verbosePrint(String str) {
    if(verbose){
        Serial.println(str);
    }
}

bool CodeScanner::sendCommand(String cmd, String verbmsg) {
    if(!Serial2){
        verbosePrint("ERRORE Serial2");
        return false;
    }
    if(!configMode){
        verbosePrint("Scanner non in modalità configurazione");
        return false;
    }

    Serial2.print(cmd);
    delay(delayCommands);
    if(Serial2.read() == ACK){
        verbosePrint(verbmsg);
        return true;
    }
    verbosePrint();
    return false;
}

bool CodeScanner::sendQuery(char* query, char* response_data) {
    char buffer[RESPONSE_SIZE + 1];
    char response_cmd[RESPONSE_CMD_SIZE];

    if(!Serial2){
        verbosePrint("ERRORE Serial2");
        return false;
    }
    if(!configMode){
        verbosePrint("Scanner non in modalità configurazione");
        return false;
    }

    Serial2.print(query);
    delay(delayCommands);
    read(buffer, RESPONSE_SIZE + 1);

    if(buffer[RESPONSE_SIZE - 1] != ACK){
        return false;
    }
    strncpy(response_cmd, buffer, RESPONSE_CMD_SIZE);
    strncpy(response_data, &buffer[RESPONSE_CMD_SIZE], RESPONSE_DATA_SIZE);
    if(strncmp(response_cmd, &query[2], RESPONSE_CMD_SIZE) != 0){
        return false;
    }
    return true;
}

bool CodeScanner::verifyChar(char value) {
    bool lowerCaseRes, upperCaseRes, digitsRes;

    if(rawOutput){
        return true;
    }

    lowerCaseRes = (lowerCase && (value >= 97 && value <= 122));
    upperCaseRes = (upperCase && (value >= 65 && value <= 90));
    digitsRes = (digits && (value >= 48 && value <= 57));
    return lowerCaseRes || upperCaseRes || digitsRes;
}

CodeScanner::CodeScanner() {
    configMode = false;
    verbose = false;
    delayCommands = 1000;
    rawOutput = true;
    lowerCase = false;
    upperCase = false;
    digits = false;
}

void CodeScanner::setLowerCaseCharOutput(bool value) {
    lowerCase = value;
    if(lowerCase){
        rawOutput = false;
    }
}

void CodeScanner::setUpperCaseOutput(bool value) {
    upperCase = value;
    if(upperCase){
        rawOutput = false;
    }
}

void CodeScanner::setDigitsOutput(bool value) {
    digits = value;
    if(digits){
        rawOutput = false;
    }
}

void CodeScanner::setRawOutput(bool value) {
    rawOutput = value;
    if(value){
        lowerCase = false;
        upperCase = false;
        digits = false;
    }
}

void CodeScanner::setVerbose(bool isverbose) {
    verbose = isverbose;
}

bool CodeScanner::setConfigMode() {
    if(configMode){
        return false;
    }
    configMode = true;
    configMode = sendCommand("~M00910001.", "Modalità configurazione attivata");
    return configMode;
}

bool CodeScanner::closeConfigMode() {
    if(!configMode){
        return false;
    }
    configMode = !sendCommand("~M00910000.", "Modalità configurazione disattivata");
    return configMode;
}

bool CodeScanner::saveConfig() {
    return sendCommand("~MA5F0506A.", "Configurazione salvata");
}

bool CodeScanner::restoreConfig() {
    return sendCommand("~MA5F08F37.", "Configurazione ripristinata");
}

bool CodeScanner::setManualMode() {
    return sendCommand("~M00210000.", "Modalità manuale attivata");
}

bool CodeScanner::setContinuousMode() {
    return sendCommand("~M00210001.", "Modalità continua attivata");
}

bool CodeScanner::setInductionMode() {
    return sendCommand("~M00210002.", "Modalità induzione attivata");
}

bool CodeScanner::setContinuousSleep() {
    return sendCommand("~M00220001.", "Sleep in modalità continua attivato");
}

bool CodeScanner::setScanDelay(CodeScanner::Delay delay) {
    switch (delay) {
        case DELAY_0MS: {
            return sendCommand("~M00B00000.", "Scan delay impostato a 0ms");
        }
        case DELAY_100MS: {
            return sendCommand("~M00B00001.", "Scan delay impostato a 100ms");
        }
        case DELAY_1000MS: {
            return sendCommand("~M00B0000A.", "Scan delay impostato a 1000ms");
        }
        case DELAY_10000MS: {
            return sendCommand("~M00B00064.", "Scan delay impostato a 10000ms");
        }
    }
    return false;
}

bool CodeScanner::set1DCodesOn() {
    return sendCommand("~M00010001.", "Attivati tutti i codici 1D");
}

bool CodeScanner::set1DCodesOff() {
    return sendCommand("~M00010000.", "Disattivati tutti i codici 1D");
}

bool CodeScanner::set2DCodesOn() {
    return sendCommand("~M00020001.", "Attivati tutti i codici 2D");
}

bool CodeScanner::set2DCodesOff() {
    return sendCommand("~M00020000.", "Disattivati tutti i codici 2D");
}

bool CodeScanner::setPreferredConfig() {
    bool done;

    done = setConfigMode();
    if(done){
        done = setInductionMode();
    }
    if(done){
        done = setContinuousSleep();
    }
    if(done){
        done = setScanDelay(DELAY_1000MS);
    }
    if(done){
        done = closeConfigMode();
    }
    return done;
}

bool CodeScanner::setStartupVolume(CodeScanner::Volume volume) {
    switch (volume) {
        case VOLUME_0: {
            return sendCommand("~M00FA0000.", "StartupVolume impostato a 0%");
        }
        case VOLUME_20: {
            return sendCommand("~M00FA0001.", "StartupVolume impostato a 20%");
        }
        case VOLUME_40: {
            return sendCommand("~M00FA0002.", "StartupVolume impostato a 40%");
        }
        case VOLUME_60: {
            return sendCommand("~M00FA0003.", "StartupVolume impostato a 60%");
        }
        case VOLUME_80: {
            return sendCommand("~M00FA0004.", "StartupVolume impostato a 80%");
        }
        case VOLUME_100: {
            return sendCommand("~M00FA0005.", "StartupVolume impostato a 100%");
        }
    }
    return false;
}

bool CodeScanner::setDecodingVolume(CodeScanner::Volume volume) {
    switch (volume) {
        case VOLUME_0: {
            return sendCommand("~M00FB0000.", "DecodingVolume impostato a 0%");
        }
        case VOLUME_20: {
            return sendCommand("~M00FB0001.", "DecodingVolume impostato a 20%");
        }
        case VOLUME_40: {
            return sendCommand("~M00FB0002.", "DecodingVolume impostato a 40%");
        }
        case VOLUME_60: {
            return sendCommand("~M00FB0003.", "DecodingVolume impostato a 60%");
        }
        case VOLUME_80: {
            return sendCommand("~M00FB0004.", "DecodingVolume impostato a 80%");
        }
        case VOLUME_100: {
            return sendCommand("~M00FB0005.", "DecodingVolume impostato a 100%");
        }
    }
    return false;
}

bool CodeScanner::setConfigVolume(CodeScanner::Volume volume) {
    switch (volume) {
        case VOLUME_0: {
            return sendCommand("~M00FC0000.", "ConfigVolume impostato a 0%");
        }
        case VOLUME_20: {
            return sendCommand("~M00FC0001.", "ConfigVolume impostato a 20%");
        }
        case VOLUME_40: {
            return sendCommand("~M00FC0002.", "ConfigVolume impostato a 40%");
        }
        case VOLUME_60: {
            return sendCommand("~M00FC0003.", "ConfigVolume impostato a 60%");
        }
        case VOLUME_80: {
            return sendCommand("~M00FC0004.", "ConfigVolume impostato a 80%");
        }
        case VOLUME_100: {
            return sendCommand("~M00FC0005.", "ConfigVolume impostato a 100%");
        }
    }
    return false;
}

bool CodeScanner::setStartupTone(CodeScanner::Tone tone) {
    switch (tone) {
        case TONE_1: {
            return sendCommand("~M00EA0000.", "Impostato lo StartupTone 1");
        }
        case TONE_2: {
            return sendCommand("~M00EA0001.", "Impostato lo StartupTone 2");
        }
        case TONE_3: {
            return sendCommand("~M00EA0002.", "Impostato lo StartupTone 3");
        }
        case TONE_4: {
            return sendCommand("~M00EA0003.", "Impostato lo StartupTone 4");
        }
        case TONE_5: {
            return sendCommand("~M00EA0004.", "Impostato lo StartupTone 5");
        }
    }
    return false;
}

bool CodeScanner::setDecodingTone(CodeScanner::Tone tone) {
    switch (tone) {
        case TONE_1: {
            return sendCommand("~M00EB0000.", "Impostato il DecodingTone 1");
        }
        case TONE_2: {
            return sendCommand("~M00EB0001.", "Impostato il DecodingTone 2");
        }
        case TONE_3: {
            return sendCommand("~M00EB0002.", "Impostato il DecodingTone 3");
        }
        case TONE_4: {
            return sendCommand("~M00EB0003.", "Impostato il DecodingTone 4");
        }
        case TONE_5: {
            return sendCommand("~M00EB0004.", "Impostato il DecodingTone 5");
        }
    }
    return false;
}

bool CodeScanner::setConfigTone(CodeScanner::Tone tone) {
    switch (tone) {
        case TONE_1: {
            return sendCommand("~M00EC0000.", "Impostato il ConfigTone 1");
        }
        case TONE_2: {
            return sendCommand("~M00EC0001.", "Impostato il ConfigTone 2");
        }
        case TONE_3: {
            return sendCommand("~M00EC0002.", "Impostato il ConfigTone 3");
        }
        case TONE_4: {
            return sendCommand("~M00EC0003.", "Impostato il ConfigTone 4");
        }
        case TONE_5: {
            return sendCommand("~M00EC0004.", "Impostato il ConfigTone 5");
        }
    }
    return false;
}

bool CodeScanner::setHorizontalMirroringOn() {
    return sendCommand("~M00240001.", "Horizontal Mirroring on");
}

bool CodeScanner::setHorizontalMirroringOff() {
    return sendCommand("~M00240000.", "Horizontal Mirroring off");
}

bool CodeScanner::setVerticalMirroringOn() {
    return sendCommand("~M00250001.", "Vertical Mirroring on");
}

bool CodeScanner::setVerticalMirroringOff() {
    return sendCommand("~M00250000.", "Vertical Mirroring off");
}

bool CodeScanner::enableCode39() {
    return sendCommand("~M01600001.", "Code39 abilitato");
}

bool CodeScanner::disableCode39() {
    return sendCommand("~M01600000.", "Code39 disabilitato");
}

bool CodeScanner::enableCode128() {
    return sendCommand("~M01500001.", "Code128 abilitato");
}

bool CodeScanner::disableCode128() {
    return sendCommand("~M01500000.", "Code128 disabilitato");
}

bool CodeScanner::enableUPC_EAN_JAN() {
    return sendCommand("~M01BA0001.", "UPC/EAN/JAN abilitato");
}

bool CodeScanner::disableUPC_EAN_JAN() {
    return sendCommand("~M01BA0000.", "UPC/EAN/JAN disabilitato");
}

bool CodeScanner::enableCode93() {
    return sendCommand("~M01C00001.", "Code39 abilitato");
}

bool CodeScanner::disableCode93() {
    return sendCommand("~M01C00000.", "Code39 disabilitato");
}

bool CodeScanner::enableInterleaved_2_of_5() {
    return sendCommand("~M01850001.", "Interleaved 2 of 5 abilitato");
}

bool CodeScanner::disableInterleaved_2_of_5() {
    return sendCommand("~M01850000.", "Interleaved 2 of 5 disabilitato");
}

bool CodeScanner::enableCodabar() {
    return sendCommand("~M01450001.", "Codabar abilitato");
}

bool CodeScanner::disableCodabar() {
    return sendCommand("~M01450000.", "Codabar disabilitato");
}

bool CodeScanner::enableCode11() {
    return sendCommand("~M10000001.", "Code11 abilitato");
}

bool CodeScanner::disableCode11() {
    return sendCommand("~M10000000.", "Code11 disabilitato");
}

bool CodeScanner::enableMatrix_2_of_5() {
    return sendCommand("~M02000001.", "Matrix 2 of 5 abilitato");
}

bool CodeScanner::disableMatrix_2_of_5() {
    return sendCommand("~M02000000.", "Matrix 2 of 5 disabilitato");
}

bool CodeScanner::enableMSI() {
    return sendCommand("~M11000001.", "MSI abilitato");
}

bool CodeScanner::disableMSI() {
    return sendCommand("~M11000000.", "MSI disabilitato");
}

bool CodeScanner::enableIndustrial_2_of_5() {
    return sendCommand("~M01E50001.", "Industrial 2 of 5 abilitato");
}

bool CodeScanner::disableIndustrial_2_of_5() {
    return sendCommand("~M01E50000.", "Industrial 2 of 5 disabilitato");
}

bool CodeScanner::enableGS1() {
    return sendCommand("~M12000001.", "GS1 Databar abilitato");
}

bool CodeScanner::disableGS1() {
    return sendCommand("~M12000000.", "GS1 Databar disabilitato");
}

bool CodeScanner::enableISBN() {
    return sendCommand("~M13000001.", "ISBN abilitato");
}

bool CodeScanner::disableISBN() {
    return sendCommand("~M13000000.", "ISBN disabilitato");
}

bool CodeScanner::enableISSN() {
    return sendCommand("~M13260001.", "ISSN abilitato");
}

bool CodeScanner::disableISSN() {
    return sendCommand("~M13260000.", "ISSN disabilitato");
}

bool CodeScanner::enableCODE_32() {
    return sendCommand("~M13530001.", "CODE 32 abilitato");
}

bool CodeScanner::disableCODE_32() {
    return sendCommand("~M13530000.", "CODE 32 disabilitato");
}

bool CodeScanner::enableQR() {
    return sendCommand("~M01B00001.", "QR abilitato");
}

bool CodeScanner::disableQR() {
    return sendCommand("~M01B00000.", "QR disabilitato");
}

int CodeScanner::getBaudRate() {
    char cmd[RESPONSE_CMD_SIZE];
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q00F5.", data)){
        return -1;
    }

    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        char ris = data[RESPONSE_DATA_SIZE - 1];

        switch (ris) {
            case '0': {
                return 1200;
            }
            case '1': {
                return 2400;
            }
            case '2': {
                return 4800;
            }
            case '3': {
                return 9600;
            }
            case '4': {
                return 19200;
            }
            case '5': {
                return 38400;
            }
            case '6': {
                return 57600;
            }
            case '7': {
                return 115200;
            }
        }
    }
    return -1;
}

CodeScanner::Volume CodeScanner::getStartupVolume() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q00FA.", data)){
        return VOLUME_ERR;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        char ris = data[RESPONSE_DATA_SIZE - 1];

        switch (ris) {
            case '0': {
                return VOLUME_0;
            }
            case '1': {
                return VOLUME_20;
            }
            case '2': {
                return VOLUME_40;
            }
            case '3': {
                return VOLUME_60;
            }
            case '4': {
                return VOLUME_80;
            }
            case '5': {
                return VOLUME_100;
            }
        }
    }
    return VOLUME_ERR;
}

CodeScanner::Volume CodeScanner::getDecodingVolume() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q00FB.", data)){
        return VOLUME_ERR;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        char ris = data[RESPONSE_DATA_SIZE - 1];

        switch (ris) {
            case '0': {
                return VOLUME_0;
            }
            case '1': {
                return VOLUME_20;
            }
            case '2': {
                return VOLUME_40;
            }
            case '3': {
                return VOLUME_60;
            }
            case '4': {
                return VOLUME_80;
            }
            case '5': {
                return VOLUME_100;
            }
        }
    }
    return VOLUME_ERR;
}

CodeScanner::Volume CodeScanner::getConfigVolume() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q00FC.", data)){
        return VOLUME_ERR;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        char ris = data[RESPONSE_DATA_SIZE - 1];

        switch (ris) {
            case '0': {
                return VOLUME_0;
            }
            case '1': {
                return VOLUME_20;
            }
            case '2': {
                return VOLUME_40;
            }
            case '3': {
                return VOLUME_60;
            }
            case '4': {
                return VOLUME_80;
            }
            case '5': {
                return VOLUME_100;
            }
        }
    }
    return VOLUME_ERR;
}

CodeScanner::Tone CodeScanner::getStartupTone() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q00EA.", data)){
        return TONE_ERR;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        char ris = data[RESPONSE_DATA_SIZE - 1];

        switch (ris) {
            case '0': {
                return TONE_1;
            }
            case '1': {
                return TONE_2;
            }
            case '2': {
                return TONE_3;
            }
            case '3': {
                return TONE_4;
            }
            case '4': {
                return TONE_5;
            }
        }
    }
    return TONE_ERR;
}

CodeScanner::Tone CodeScanner::getDecodingTone() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q00EB.", data)){
        return TONE_ERR;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        char ris = data[RESPONSE_DATA_SIZE - 1];

        switch (ris) {
            case '0': {
                return TONE_1;
            }
            case '1': {
                return TONE_2;
            }
            case '2': {
                return TONE_3;
            }
            case '3': {
                return TONE_4;
            }
            case '4': {
                return TONE_5;
            }
        }
    }
    return TONE_ERR;
}

CodeScanner::Tone CodeScanner::getConfigTone() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q00EC.", data)){
        return TONE_ERR;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        char ris = data[RESPONSE_DATA_SIZE - 1];

        switch (ris) {
            case '0': {
                return TONE_1;
            }
            case '1': {
                return TONE_2;
            }
            case '2': {
                return TONE_3;
            }
            case '3': {
                return TONE_4;
            }
            case '4': {
                return TONE_5;
            }
        }
    }
    return TONE_ERR;
}

bool CodeScanner::getHorizontalMirroring() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q0024.", data)){
        return false;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        return (data[RESPONSE_DATA_SIZE - 1] == '1');
    }
    return false;
}

bool CodeScanner::getVerticalMirroring() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q0025.", data)){
        return false;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        return (data[RESPONSE_DATA_SIZE - 1] == '1');
    }
    return false;
}

bool CodeScanner::getCode39() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q0160.", data)){
        return false;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        return (data[RESPONSE_DATA_SIZE - 1] == '1');
    }
    return false;
}

bool CodeScanner::getCode128() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q0150.", data)){
        return false;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        return (data[RESPONSE_DATA_SIZE - 1] == '1');
    }
    return false;
}

bool CodeScanner::getUPC_EAN_JAN() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q01BA.", data)){
        return false;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        return (data[RESPONSE_DATA_SIZE - 1] == '1');
    }
    return false;
}

bool CodeScanner::getCode93() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q01C0.", data)){
        return false;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        return (data[RESPONSE_DATA_SIZE - 1] == '1');
    }
    return false;
}

bool CodeScanner::getInterleaved_2_of_5() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q0185.", data)){
        return false;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        return (data[RESPONSE_DATA_SIZE - 1] == '1');
    }
    return false;
}

bool CodeScanner::getCodabar() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q0145.", data)){
        return false;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        return (data[RESPONSE_DATA_SIZE - 1] == '1');
    }
    return false;
}

bool CodeScanner::getCode11() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q1000.", data)){
        return false;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        return (data[RESPONSE_DATA_SIZE - 1] == '1');
    }
    return false;
}

bool CodeScanner::getMatrix_2_of_5() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q0200.", data)){
        return false;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        return (data[RESPONSE_DATA_SIZE - 1] == '1');
    }
    return false;
}

bool CodeScanner::getMSI() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q1100.", data)){
        return false;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        return (data[RESPONSE_DATA_SIZE - 1] == '1');
    }
    return false;
}

bool CodeScanner::getIndustrial_2_of_5() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q01E5.", data)){
        return false;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        return (data[RESPONSE_DATA_SIZE - 1] == '1');
    }
    return false;
}

bool CodeScanner::getGS1() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q1200.", data)){
        return false;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        return (data[RESPONSE_DATA_SIZE - 1] == '1');
    }
    return false;
}

bool CodeScanner::getISBN() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q1300.", data)){
        return false;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        return (data[RESPONSE_DATA_SIZE - 1] == '1');
    }
    return false;
}

bool CodeScanner::getISSN() {
    return getISBN();   //le specifiche indicano lo stesso codice di ISBN
}

bool CodeScanner::getCODE_32() {
    return getISBN();   //le specifiche indicano lo stesso codice di ISBN
}

bool CodeScanner::getQR() {
    char data[RESPONSE_DATA_SIZE];

    if(!sendQuery("~Q01B0.", data)){
        return false;
    }
    if(strncmp(data, "000", RESPONSE_DATA_SIZE - 1) == 0){
        return (data[RESPONSE_DATA_SIZE - 1] == '1');
    }
    return false;
}

int CodeScanner::read(char *buffer) {
    int nbytes = -1, bytesWritten = 0;

    nbytes = Serial2.available();
    while(nbytes <= 0){
        delay(100);
        nbytes = Serial2.available();
    }
    delay(100);
    memset(buffer, 0, sizeof(buffer));
    nbytes = Serial2.available();
    for(int i = 0; i < nbytes; i++){
        char tmp;

        Serial2.read(&tmp, 1);
        if(verifyChar(tmp)){
            buffer[bytesWritten] = tmp;
            bytesWritten++;
        }
    }
    return bytesWritten;
}

int CodeScanner::read(char *buffer, int bytes) {
    int bytesWritten = 0;

    if(bytes <= 0){
        return -1;
    }
    while(Serial2.available() < bytes){
        delay(100);
    }

    for(int i = 0; i < bytes; i++){
        char tmp;

        Serial2.read(&tmp, 1);
        if(verifyChar(tmp)){
            buffer[bytesWritten] = tmp;
            bytesWritten++;
        }
    }
    return bytesWritten;
}