#include <mcs51reg.h>
#include "t6963c.h"

unsigned char *_t6963c = (unsigned char __xdata *) 0x2000;

// Les deux bits qui indiquent que le contr�leur est pr�t pour une op�ration.
#define STA0_STA1 3

// Bits qui indique que le contr�leur est pr�t pour un auto-write
#define STA_3 8

// Pour transmettre une donn�e:
#define CD_DATA 0

// Pour transmettre une commande:
#define CD_COMMAND 1

/**
 * Attend que le contr�leur soit pr�t pour une nouvelle commande.
 */
void T6963C_statusCheck() {
	unsigned char status;
	do {
		status = _t6963c[CD_COMMAND];
		status &= STA0_STA1;
	} while (status != STA0_STA1);
}

/**
 * Transmet une commande qui sans arguments.
 * @param command Le code de commande.
 */
void T6963C_noDataCommand(unsigned char command) {
	T6963C_statusCheck();
	_t6963c[CD_COMMAND] = command;
}

/**
 * Transmet une commande qui prend un seul argument.
 * @param command Le code de commande.
 * @param data La valeur de l'argument.
 */
void T6963C_oneDataCommand(unsigned char command, unsigned char data1) {
	T6963C_statusCheck();
	_t6963c[CD_DATA] = data1;
	T6963C_statusCheck();
	_t6963c[CD_COMMAND] = command;
}

/**
 * Transmet une commande qui prend deux arguments.
 * @param command Le code de commande.
 * @param data1 La valeur du premier argument.
 * @param data2 La valeur du deuxi�me argument.
 */
void T6963C_twoDataCommand(unsigned char command, 
                           unsigned char data1,
                           unsigned char data2) {
	T6963C_statusCheck();
	_t6963c[CD_DATA] = data1;
	T6963C_statusCheck();
	_t6963c[CD_DATA] = data2;
	T6963C_statusCheck();
	_t6963c[CD_COMMAND] = command;
}

/**
 * Transmet une commande qui prend deux arguments.
 * @param command Le code de commande.
 * @param address L'adresse, sur 16 bits.
 */
void T6963C_addressCommand(unsigned char command, 
                           unsigned int address) {
	unsigned char *b = (unsigned char *) &address;
	T6963C_statusCheck();
	_t6963c[CD_DATA] = b[0];
	T6963C_statusCheck();
	_t6963c[CD_DATA] = b[1];
	T6963C_statusCheck();
	_t6963c[CD_COMMAND] = command;
}


/**
 * Pour �crire 1 byte � une adresse de la RAM.
 * @param address L'adresse.
 * @param data1 La donn�e.
 */
 void T6963C_dataWrite(unsigned int address,unsigned char data1) {
	T6963C_addressCommand(0x24,address);
	T6963C_oneDataCommand(0xC0,data1);
}

/**
 * Pour lire 1 byte � une adresse de la RAM.
 * @param address L'adresse.
 * @return La donn�e.
 */
unsigned char T6963C_dataRead(unsigned int address) {
	T6963C_addressCommand(0x24,address);
	T6963C_noDataCommand(0xC1);
	return _t6963c[CD_DATA];
}

/**
 * Status check sp�cial avant d'ex�cuter un auto-check.
 */
void T6963C_statusCheckForAutoWrite() {
	unsigned char status;
	do {
		status = _t6963c[CD_COMMAND];  // Pour activer C
		status &= STA_3;          // V�rifie STA3
	} while (status != STA_3);
}

/**
 * �crit un nombre variable de bytes � partir de l'adresse indiqu�e.
 * @param address L'adresse.
 * @param data Vecteur avec les bytes � �crire.
 * @param size Nombre de bytes � �crire.
 * @param L'adresse suivante au dernier byte. Ceci peut �tre
 * pratique pour les affichages constitu�s de plusieurs blocs.
 */
unsigned int T6963C_autoWrite(unsigned int address, 
                              unsigned char *data1,
                              unsigned int size) {
	unsigned int n;
	T6963C_addressCommand(0x24, address);
	T6963C_noDataCommand(0xB0);
	for(n=0 ; n<size ; n++) {
		address++;
		T6963C_statusCheckForAutoWrite();
		_t6963c[CD_DATA] = data1[n];
	}
	T6963C_noDataCommand(0xB2);
	return address;
}
/**
 * Affiche plusieurs fois le caract�re sp�cifi� � l'adresse indiqu�e.
 * @param address L'adresse.
 * @param data1 Caract�re � �crire.
 * @param size Le nombre de r�p�titions.
 * @param L'adresse suivante au dernier byte. Ceci peut �tre
 * pratique pour les affichages constitu�s de plusieurs blocs.
 */
unsigned int T6963C_autoRepeat(unsigned int address, 
                               unsigned char data1,
                               unsigned int repeats) {
	unsigned int n;
	T6963C_addressCommand(0x24, address);
	T6963C_noDataCommand(0xB0);
	for(n=0 ; n<repeats ; n++) {
		address++;
		T6963C_statusCheckForAutoWrite();
		_t6963c[CD_DATA] = data1;
	}
	T6963C_noDataCommand(0xB2);
	return address;
}

/**
 * Calcule l'adresse correspondante aux coordonn�es.
 * @param x Coordonn�e X.
 * @param y Coordonn�e Y.
 * @return l'adresse correspondante.
 */
unsigned int T6963C_calculateAddress(unsigned char x, 
									 unsigned char y) {
	return T6963C_ZONE_TEXTE + y * T6963C_COLONNES + x;
}

/**
 * Affiche un caract�re � la position indiqu�e.
 * @param x Coordonn�e X.
 * @param y Coordonn�e Y.
 * @param c Le caract�re � afficher.
 */
void T6963C_writeAt(unsigned char x, 
                    unsigned char y, 
                    unsigned char c) {
	unsigned int address = T6963C_calculateAddress(x, y);
	T6963C_dataWrite(address, c);
}

/**
 * Lit un caract�re de la position indiqu�e.
 * @param x Coordonn�e X.
 * @param y Coordonn�e Y.
 * @return Le caract�re � afficher.
 */
unsigned char T6963C_readFrom(unsigned char x, 
                              unsigned char y) {
	unsigned int address = T6963C_calculateAddress(x, y);
	return T6963C_dataRead(address);
}

/**
 * Initialise le contr�leur graphique.
 */
void T6963C_initialize() {
	T6963C_addressCommand(0x40, T6963C_ZONE_TEXTE);
	T6963C_addressCommand(0x42, T6963C_ZONE_GRAPHIQUE);
	T6963C_twoDataCommand(0x22, T6963C_ZONE_CG / 2048,0); 
	T6963C_twoDataCommand(0x41, T6963C_COLONNES,0);
	T6963C_twoDataCommand(0x43, T6963C_COLONNES,0);     
	
	// Pas de curseur, affichage du texte, affichage du graphique:
	T6963C_noDataCommand(0x9C);
	// Mode OR, CG Interne.
	T6963C_noDataCommand(0x80);
}
