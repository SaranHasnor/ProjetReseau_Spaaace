#ifndef INTERFACE_H_DEFINED
#define INTERFACE_H_DEFINED

#include <utils.h>

typedef enum {
	FIELDTYPE_TEXT,
	FIELDTYPE_INT,
	FIELDTYPE_FLOAT,
	FIELDTYPE_SPECIAL,	// TODO: Take a specific set of characters
	FIELDTYPE_BINARY // All data is displayed as binary
} fieldType_t;

void initInterface(int window);

void setMenu(uint menu);
void addListEntry(uint menu, uint id, char *entry);
void clearList(uint menu, uint id);
void setTextFieldValue(uint menu, uint id, float min, float max, void *value, short editable);
void setSliderValue(uint menu, uint id, float min, float max, float *value);
void setCheckBoxValue(uint menu, uint id, short *value);
void setRadioButtonValue(uint menu, uint id, short *value);
uint getListSelectedIndex(uint menu, uint id);
void deleteObject(uint menu, uint index);
void clearMenu(uint menu);

void saveListSelectedIndex(uint menu, uint id);
void loadListSelectedIndex(uint menu, uint id);

int newMenu(int x, int y, void (*onEnter)(uint), void (*onExit)(uint));
int newButton(char *text, uint menu, uint x, uint y, uint w, uint h, void (*onClick)(void));
int newTextField(uint menu, uint x, uint y, uint w, uint h, fieldType_t type);
int newLabel(char *text, uint menu, uint x, uint y);
int newList(uint menu, uint x, uint y, uint w, uint h, void (*onSelect)(uint));
int newSlider(uint menu, uint x, uint y, uint length);
int newCheckBox(uint menu, uint x, uint y, uint size);
int newRadioButton(uint menu, uint x, uint y, uint radius, uint group);

#endif