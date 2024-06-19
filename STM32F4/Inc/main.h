/*
 * main.h
 *
 *      Author: mthn_
 */

#ifndef MAIN_H_
#define MAIN_H_

#define SECTOR2_BASE_ADDR 0x08008000 // kullanıcı uygulaması sector2 den baslayacak

void bootmode();
void bootcheck();
void jump_to_user_app();

#endif /* MAIN_H_ */
