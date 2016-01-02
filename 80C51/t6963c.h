#ifndef T6963C__H
#define T6963C__H

// Zone texte à partir de 0x0000 (occupe 30 x 16 = 480 bytes)
#define T6963C_ZONE_TEXTE 0x0000
// Zone graphique à partir de 0x0200 (occupe 30 x 128 = 3840 bytes)
#define T6963C_ZONE_GRAPHIQUE 0x0200
// Zone CG à partir de 3x2048 = 6144 = 0x1800.
#define T6963C_ZONE_CG 6144
// Nombre de colonnes de l'affichage:
#define T6963C_COLONNES 30

void T6963C_statusCheck();
void T6963C_noDataCommand(unsigned char command);
void T6963C_oneDataCommand(unsigned char command, unsigned char data1);
void T6963C_twoDataCommand(unsigned char command, 
                           unsigned char data1,
                           unsigned char data2);
void T6963C_addressCommand(unsigned char command, 
                           unsigned int address);
void T6963C_dataWrite(unsigned int address,unsigned char data1);
void T6963C_statusCheckForAutoWrite();
unsigned int T6963C_autoWrite(unsigned int address, 
                              unsigned char *data1,
                              unsigned int size);
unsigned int T6963C_autoRepeat(unsigned int address, 
                               unsigned char data1,
                               unsigned int repeats);
unsigned int T6963C_calculateAddress(unsigned char x, 
									 unsigned char y);
void T6963C_writeAt(unsigned char x, 
                    unsigned char y, 
					unsigned char c);
unsigned char T6963C_readFrom(unsigned char x, 
                              unsigned char y);
void T6963C_initialize();

#endif