#pragma once

void secure_digital_card_init();
int secure_digital_card_mount();
int secure_digital_card_unmount();
int secure_digital_card_save_data(const char *filename, const char* content);
void secure_digital_card_print_file(const char *filename);