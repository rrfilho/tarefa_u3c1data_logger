#include "secure_digital_card.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hardware/rtc.h"
#include "pico/stdlib.h"

#include "ff.h"
#include "diskio.h"
#include "f_util.h"
#include "hw_config.h"
#include "my_debug.h"
#include "rtc.h"
#include "sd_card.h"

static sd_card_t* sd_get_by_name(const char *const name) {
    for (size_t i = 0; i < sd_get_num(); ++i)
        if (strcmp(sd_get_by_num(i)->pcName, name) == 0)
            return sd_get_by_num(i);
    return NULL;
}

static FATFS* sd_get_fs_by_name(const char *name) {
    for (size_t i = 0; i < sd_get_num(); ++i)
        if (strcmp(sd_get_by_num(i)->pcName, name) == 0)
            return &sd_get_by_num(i)->fatfs;
    return NULL;
}

void secure_digital_card_init(){
    time_init();
}

int secure_digital_card_mount() {
    const char *arg1 = strtok(NULL, " ");
    if (!arg1) arg1 = sd_get_by_num(0)->pcName;
    FATFS *p_fs = sd_get_fs_by_name(arg1);
    if (!p_fs) return -1;
    FRESULT fr = f_mount(p_fs, arg1, 1);
    if (FR_OK != fr) return -1;
    sd_card_t *pSD = sd_get_by_name(arg1);
    myASSERT(pSD);
    pSD->mounted = true;
    return 0;
}

int secure_digital_card_unmount() {
    const char *arg1 = strtok(NULL, " ");
    if (!arg1) arg1 = sd_get_by_num(0)->pcName;
    FATFS *p_fs = sd_get_fs_by_name(arg1);
    if (!p_fs) return -1;
    FRESULT fr = f_unmount(arg1);
    if (FR_OK != fr) return -1;
    sd_card_t *pSD = sd_get_by_name(arg1);
    pSD->mounted = false;
    pSD->m_Status |= STA_NOINIT;
    return 0;
}

int secure_digital_card_save_data(const char *filename, const char* content) {
    FIL file;
    FRESULT res = f_open(&file, filename, FA_OPEN_APPEND | FA_WRITE);
    if (res != FR_OK) return -1;
    UINT bw;
    res = f_write(&file, content, strlen(content), &bw);
    if (res != FR_OK) {
        f_close(&file);
        return -1;
    }
    f_close(&file);
    return 0;
}

void secure_digital_card_print_file(const char *filename) {
    FIL file;
    FRESULT res = f_open(&file, filename, FA_READ);
    if (res != FR_OK) return;
    char buffer[128];
    UINT br;
    printf("--- inicio %s ---\n", filename);
    while (f_read(&file, buffer, sizeof(buffer) - 1, &br) == FR_OK && br > 0) {
        buffer[br] = '\0';
        printf("%s\n", buffer);
    }
    f_close(&file);
    printf("--- fim %s ---\n\n", filename);
}
