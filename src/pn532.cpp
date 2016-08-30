#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <upm/pn532.h>

int main(void)
{
    // Instantiate an PN532 on I2C bus 0 (default) using gpio 3 for the
    // IRQ, and gpio 2 for the reset pin.
    upm::PN532 *nfc = new upm::PN532(3, 2);
    if (!nfc->init()) {
        printf("init() failed\n");
    }
    uint32_t vers = nfc->getFirmwareVersion();
    if (vers) {
        printf("Got firmware version: 0x%08x\n", vers);
    } else {
        printf("Could not identify PN532\n");
        return 1;
    }
    // Now scan and identify any cards that come in range (1 for now)
    // Retry forever
    nfc->setPassiveActivationRetries(0xff);
    nfc->SAMConfig();
    uint8_t uidSize;
    uint8_t uid[7];
    while (1) {
        memset(uid, 0, 7);
        if (nfc->readPassiveTargetID(nfc->BAUD_MIFARE_ISO14443A,
                    uid, &uidSize, 2000)) {
            // found a card
            printf("Found a card: UID len %d\n", uidSize);
            printf("UID: ");
            for (int i = 0; i < uidSize; i++)
                printf("%02x ", uid[i]);
            printf("\n");
            printf("SAK: 0x%02x\n", nfc->getSAK());
            printf("ATQA: 0x%04x\n\n", nfc->getATQA());
            sleep(1);
        } else {
            printf("Waiting for a card...\n");
        }
    }

    return 0;
}
