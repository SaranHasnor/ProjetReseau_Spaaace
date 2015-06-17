#include <GL/glut.h>
#include <windows.h>
#include "engine_interface.h"
#include "engine_window.h"
#include <utils.h>
#include <utils_bytestream.h>
#include <utils_string.h>

/*
interface.c

- Interface creation tools
- Interface management and rendering
*/

typedef enum {
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT
} alignment_t;

typedef struct label_s {
	char		*text;
	alignment_t	textAlignment;
} label_t;

typedef struct button_s {
	char	*text;
	void	(*onClick)(void);
} button_t;

typedef struct textField_s {
	void		**value;
	fieldType_t	type;
	float		min,max;
	short		editable;
	alignment_t	textAlignment;
} textField_t;

typedef struct slider_s {
	float	percentage;
	float	*value;
	float	min,max;
} slider_t;

typedef struct list_s {
	char		**list;
	uint		listLength;
	uint		selectIndex;
	uint		startIndex;			// Index of the first entry to display
	uint		maxEntries;
	alignment_t	textAlignment;
	void		(*onSelect)(uint);	// Args: Index
} list_t;

typedef struct checkBox_s {
	short			*value;
	void			(*onToggle)(short);	// Args: Value
} checkBox_t;

typedef struct radio_s {
	uint	group;
	short			*value;
} radio_t;

typedef enum {
	OBJ_LABEL,
	OBJ_BUTTON,
	OBJ_TEXTFIELD,
	OBJ_LIST,
	OBJ_SLIDER,
	OBJ_CHECKBOX,
	OBJ_RADIOBUTTON
} objectType_t;

typedef union object_u {
	slider_t	*slider;
	button_t	*button;
	list_t		*list;
	label_t		*label;
	textField_t	*textField;
	checkBox_t	*checkBox;
	radio_t		*radioButton;
} gen_object_t;

typedef struct object_s {
	uint	x,y;
	uint	w,h;
	gen_object_t	object;
	objectType_t	type;
} object_t;

typedef struct menu_s {
	int				x,y;
	object_t		*objects;
	uint	nbObjects;

	// Events
	void			(*onEnter)(uint);	// Args: menu we came from
	void			(*onExit)(uint);	// Args: menu we're switching to
} menu_t;

typedef struct interface_s {
	// Current view pos
	int				x,y;

	// Transition data
	int				transTime;
	int				targX, targY;

	// Menus
	menu_t			*menus;
	uint	nbMenus;

} interface_t;

interface_t _interface;

object_t *_curObject = NULL;	// Current object we're working on

void *clipboard;
char *renameString;
uint renameStringCursor;

/*
====================================================
				CREATION FUNCTIONS
====================================================
*/

int newMenu(int x, int y, void (*onEnter)(uint), void (*onExit)(uint))
{ // Creates a menu at the given coordinates
	if (_interface.nbMenus == 0)
	{
		_interface.menus = (menu_t*)mem_alloc(sizeof(menu_t));
	}
	else
	{
		_interface.menus = (menu_t*)mem_realloc(_interface.menus,sizeof(menu_t)*(_interface.nbMenus+1));
	}

	_interface.menus[_interface.nbMenus].x = x;
	_interface.menus[_interface.nbMenus].y = y;
	_interface.menus[_interface.nbMenus].nbObjects = 0;
	_interface.menus[_interface.nbMenus].objects = NULL;

	_interface.menus[_interface.nbMenus].onEnter = onEnter;
	_interface.menus[_interface.nbMenus].onExit = onExit;

	return _interface.nbMenus++;
}

void deleteObject(uint menu, uint index)
{ // Deletes the selected object
	object_t *object;
	if (menu >= _interface.nbMenus)
	{
		return;
	}

	if (index >= _interface.menus[menu].nbObjects)
	{
		return;
	}

	object = &_interface.menus[menu].objects[index];

	switch (object->type)
	{
	case OBJ_LABEL:
		mem_free(object->object.label);
		break;
	case OBJ_BUTTON:
		mem_free(object->object.button);
		break;
	case OBJ_TEXTFIELD:
		mem_free(object->object.textField->value);
		mem_free(object->object.textField);
		break;
	case OBJ_LIST:
		mem_free(object->object.list);
		break;
	case OBJ_SLIDER:
		mem_free(object->object.slider);
		break;
	case OBJ_CHECKBOX:
		mem_free(object->object.checkBox);
		break;
	case OBJ_RADIOBUTTON:
		mem_free(object->object.radioButton);
		break;
	default:
		break;
	}

	_interface.menus[menu].nbObjects--;
}

void clearMenu(uint menu)
{ // Removes all objects from the selected menu
	uint i;
	uint size;
	if (menu >= _interface.nbMenus)
	{
		return;
	}

	size = _interface.menus[menu].nbObjects;

	// Kind of messy but okay for now
	if (!size)
	{
		return;
	}

	for (i=size-1; i>0; i--)
	{
		deleteObject(menu, i);
	}

	deleteObject(menu, 0);
}

int addObject(uint menu, objectType_t type, void *object, uint x, uint y, uint w, uint h)
{ // Adds the given object to a menu
	if (menu >= _interface.nbMenus)
	{
		return -1;
	}
	
	if (_interface.menus[menu].nbObjects == 0)
	{
		_interface.menus[menu].objects = (object_t*)mem_alloc(sizeof(object_t));
	}
	else
	{
		_interface.menus[menu].objects = (object_t*)mem_realloc(_interface.menus[menu].objects,sizeof(object_t)*(_interface.menus[menu].nbObjects+1));
	}

	_interface.menus[menu].objects[_interface.menus[menu].nbObjects].x = x;
	_interface.menus[menu].objects[_interface.menus[menu].nbObjects].y = y;
	_interface.menus[menu].objects[_interface.menus[menu].nbObjects].w = w;
	_interface.menus[menu].objects[_interface.menus[menu].nbObjects].h = h;

	_interface.menus[menu].objects[_interface.menus[menu].nbObjects].type = type;

	switch (type)
	{
	case OBJ_LABEL:
		_interface.menus[menu].objects[_interface.menus[menu].nbObjects].object.label = (label_t*)object;
		break;
	case OBJ_BUTTON:
		_interface.menus[menu].objects[_interface.menus[menu].nbObjects].object.button = (button_t*)object;
		break;
	case OBJ_TEXTFIELD:
		_interface.menus[menu].objects[_interface.menus[menu].nbObjects].object.textField = (textField_t*)object;
		break;
	case OBJ_LIST:
		_interface.menus[menu].objects[_interface.menus[menu].nbObjects].object.list = (list_t*)object;
		break;
	case OBJ_SLIDER:
		_interface.menus[menu].objects[_interface.menus[menu].nbObjects].object.slider = (slider_t*)object;
		break;
	case OBJ_CHECKBOX:
		_interface.menus[menu].objects[_interface.menus[menu].nbObjects].object.checkBox = (checkBox_t*)object;
		break;
	case OBJ_RADIOBUTTON:
		_interface.menus[menu].objects[_interface.menus[menu].nbObjects].object.radioButton = (radio_t*)object;
		break;
	default:
		printf("Error: unknown object type: %i\n", type);
		break;
	}
	
	return _interface.menus[menu].nbObjects++;
}

int newLabel(char *text, uint menu, uint x, uint y)
{
	label_t *label = (label_t*)mem_alloc(sizeof(label_t));
	int res;

	label->text = text;

	res = addObject(menu, OBJ_LABEL, (void*)label, x, y, 50, 15);	// FIXME: Calculate w and h dynamically

	if (res == -1)
	{
		mem_free(label);
	}

	return res;
}

int newList(uint menu, uint x, uint y, uint w, uint h, void (*onSelect)(uint))
{
	list_t *list = (list_t*)mem_alloc(sizeof(list_t));
	int res;

	list->listLength = 0;
	list->list = NULL;
	list->selectIndex = 0;
	list->startIndex = 0;
	list->maxEntries = (int)(((float)h-40.0)/15.0);
	list->onSelect = onSelect;
	
	res = addObject(menu, OBJ_LIST, (void*)list, x, y, w, h);

	if (res == -1)
	{
		mem_free(list);
	}

	return res;
}

int newButton(char *text, uint menu, uint x, uint y, uint w, uint h, void (*onClick)(void))
{
	button_t *button = (button_t*)mem_alloc(sizeof(button_t));
	int res;

	button->text = text;
	button->onClick = onClick;
	
	res = addObject(menu, OBJ_BUTTON, (void*)button, x, y, w, h);

	if (res == -1)
	{
		mem_free(button);
	}

	return res;
}

int newSlider(uint menu, uint x, uint y, uint length)
{
	slider_t *slider = (slider_t*)mem_alloc(sizeof(slider_t));
	int res;

	slider->percentage = 0.0;
	slider->min = 0.0;
	slider->max = 0.0;
	slider->value = NULL;
	
	res = addObject(menu, OBJ_SLIDER, (void*)slider, x, y, length, 10);

	if (res == -1)
	{
		mem_free(slider);
	}

	return res;
}

int newTextField(uint menu, uint x, uint y, uint w, uint h, fieldType_t type)
{
	textField_t *field = (textField_t*)mem_alloc(sizeof(textField_t));
	int res;

	field->min = 0;
	field->max = 0;
	field->value = (void**)mem_alloc(sizeof(void*));
	*field->value = NULL;
	field->editable = 0;
	field->type = type;
	
	res = addObject(menu, OBJ_TEXTFIELD, (void*)field, x, y, w, h);

	if (res == -1)
	{
		mem_free(field->value);
		mem_free(field);
	}

	return res;
}

int newCheckBox(uint menu, uint x, uint y, uint size)
{
	checkBox_t *checkBox = (checkBox_t*)mem_alloc(sizeof(checkBox_t));
	int res;

	checkBox->value = NULL;
	
	res = addObject(menu, OBJ_CHECKBOX, (void*)checkBox, x, y, size, size);

	if (res == -1)
	{
		mem_free(checkBox);
	}

	return res;
}

int newRadioButton(uint menu, uint x, uint y, uint radius, uint group)
{
	radio_t *rButton = (radio_t*)mem_alloc(sizeof(radio_t));
	int res;

	rButton->group = group;
	rButton->value = NULL;
	
	res = addObject(menu, OBJ_RADIOBUTTON, (void*)rButton, x, y, radius, radius);

	if (res == -1)
	{
		mem_free(rButton);
	}

	return res;
}

/*
====================================================
				UTILITY FUNCTIONS
====================================================
*/

void renderString(char *s, int x, int y, short showRaster/*, float maxWidth*/)
{ // Renders a string character by character to make sure it will be at least partially rendered TODO: Support various alignments
	uint i=0;
	int _x = x, _y = y;
	while (s[i] != '\0')
	{
		glRasterPos2i(x,y);
		if (s[i] == '\n')// || x > maxWidth)
		{
			y += 13;
			x = _x;
			
			if (s[i] != '\n')
			{
				i--;
			}
		}
		else
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[i]);
			x += 8;
		}
		i++;
	}

	if (showRaster /*&& (glutGet(GLUT_ELAPSED_TIME)%1000) < 500*/) // Would be better like that but it requires constant redraws
	{ // Temporary
		glRasterPos2i(x,y);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '_');
	}
}

void renderStringV2(char *s, uint x, uint y, uint width, alignment_t textAlignment, int rasterPos)
{
	uint i=0;
	int _x = x;
	int _y = y + 14;

	while (s[i] != '\0')
	{
		glRasterPos2i(x,y);
		if (s[i] == '\n' || x > width)
		{
			y += 13;
			x = _x;
			
			if (s[i] != '\n')
			{
				i--;
			}
		}
		else
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[i]);
			if (rasterPos == i)
			{
				glRasterPos2i(x,y);
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '_');
			}
			x += 8;
		}
		i++;
	}
}

void drawInterface(void);

// Didn't want to do this buuuuuut
uint prevMenu = 0;
uint nextMenu = 0;

void doTransition(int value)
{
	float percentage = 1.0f - (float)(value-glutGet(GLUT_ELAPSED_TIME))/(float)(value-_interface.transTime);

	if (percentage < 1.0f && ((_interface.targX != _interface.x) || (_interface.targY != _interface.y)))
	{
		_interface.x = _interface.x + atLeastOne(percentage*(_interface.targX - _interface.x));
		_interface.y = _interface.y + atLeastOne(percentage*(_interface.targY - _interface.y));

		glutTimerFunc(10, doTransition, value);
	}
	else
	{

		if (_interface.menus[prevMenu].onExit)
		{
			_interface.menus[prevMenu].onExit(nextMenu);
		}
		
		prevMenu = nextMenu;

		_interface.x = _interface.targX;
		_interface.y = _interface.targY;
		_interface.transTime = 0;
	}
	
	glutSetWindow(2);
	glutPostRedisplay();
	//drawInterface();
	glutSetWindow(1);
}

void setMenu(uint menu)
{
	//static uint prevMenu = -1;

	if (menu > _interface.nbMenus)
	{
		return;
	}

	if (_interface.transTime)
	{ // Already switching menus
		return;
	}

	nextMenu = menu;

	if (_interface.menus[nextMenu].onEnter)
	{
		_interface.menus[nextMenu].onEnter(prevMenu);
	}

	_interface.targX = _interface.menus[menu].x;
	_interface.targY = _interface.menus[menu].y;
	_interface.transTime = glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc(10, doTransition, _interface.transTime + 500);
}

void listSelect(list_t *list, uint index)
{ // Utility function for internal use
	if (index >= list->listLength)
	{
		if (list->listLength)
		{ // Just lower it...
			index = list->listLength-1;
		}
		else
		{
			return;
		}
	}

	if (list->onSelect)
	{
		list->onSelect(index);
	}
	list->selectIndex = index;
}

char *getFieldValue(textField_t *field)
{ // Utility function for internal use
	char *res;
	if (field->type == FIELDTYPE_INT)
	{
		res = strFromInt(*((int*)(*field->value)));
	}
	else if (field->type == FIELDTYPE_FLOAT)
	{
		res = strFromFloat(*((float*)(*field->value)));
	}
	else if (field->type == FIELDTYPE_BINARY)
	{
		res = strFromBinary((byte*)*field->value);
	}
	else
	{
		res = newString((char*)(*field->value));
	}
	return res;
}

void unselectRadioGroup(uint menu, uint group)
{ // Utility function for internal use
	uint i;

	if (menu >= _interface.nbMenus)
		return;

	for (i=0; i<_interface.menus[menu].nbObjects; i++)
	{
		if (_interface.menus[menu].objects[i].type == OBJ_RADIOBUTTON)
		{
			radio_t *rButton = _interface.menus[menu].objects[i].object.radioButton;

			if (rButton->group == group && rButton->value)
			{
				*rButton->value = 0;
			}
		}
	}
}

void addListEntry(uint menu, uint id, char *entry)
{
	list_t *list = _interface.menus[menu].objects[id].object.list;
	
	if (!list->list)
	{
		list->list = (char**)mem_alloc(sizeof(char*));
	}
	else
	{
		list->list = (char**)mem_realloc(list->list,sizeof(char*)*(list->listLength+1));
	}

	list->list[list->listLength++] = entry;
	
	if (list->listLength == 1)
	{ // Got our first entry, select it
		list->startIndex = 0;
		listSelect(list, 0);
	}
}

void clearList(uint menu, uint id)
{
	list_t *list = _interface.menus[menu].objects[id].object.list;
	
	if (!list->list)
	{
		return;
	}

	else
	{
		mem_free(list->list);
		list->list = NULL;
		list->listLength = 0;
	}
}

void setSliderValue(uint menu, uint id, float min, float max, float *value)
{
	slider_t *slider = _interface.menus[menu].objects[id].object.slider;
	slider->percentage = (*value-min)/(max-min);
	slider->value = value;
	slider->min = min;
	slider->max = max;
}

void setTextFieldValue(uint menu, uint id, float min, float max, void *value, short editable)
{
	textField_t *field = _interface.menus[menu].objects[id].object.textField;
	*field->value = value;
	field->min = min;
	field->max = max;
	field->editable = editable;
}

void setCheckBoxValue(uint menu, uint id, short *value)
{
	checkBox_t *checkBox = _interface.menus[menu].objects[id].object.checkBox;
	checkBox->value = value;
}

void setRadioButtonValue(uint menu, uint id, short *value)
{
	radio_t *rButton = _interface.menus[menu].objects[id].object.radioButton;
	rButton->value = value;
}

uint getListSelectedIndex(uint menu, uint id)
{
	return _interface.menus[menu].objects[id].object.list->selectIndex;
}

uint _listID = 0;
void saveListSelectedIndex(uint menu, uint id)
{
	_listID = _interface.menus[menu].objects[id].object.list->selectIndex;
}

void loadListSelectedIndex(uint menu, uint id)
{
	listSelect(_interface.menus[menu].objects[id].object.list, _listID);
	_listID = 0;
}

/*
====================================================
				INPUT/OUTPUT FUNCTIONS
====================================================
*/

void drawLabel(label_t *label, uint x, uint y)
{
	// Text
	glColor3f(1.0f,1.0f,1.0f);
	renderString(label->text, x, y, 0);
}

void drawButton(button_t *button, uint x, uint y, uint w, uint h)
{
	// Border
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x+w, y);
	glVertex2i(x+w, y+h);
	glVertex2i(x, y+h);
	glEnd();
	// Text
	glColor3f(1.0f,1.0f,1.0f);
	renderString(button->text, x+(w-(strlen(button->text)*8))/2,y+(h/2+4), 0);
}

void drawTextField(textField_t *field, uint x, uint y, uint w, uint h)
{
	char *value = NULL;
	short selected = 0;

	// Border
	if (field->editable)
		glColor3f(1.0f, 1.0f, 1.0f);
	else
		glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x+w, y);
	glVertex2i(x+w, y+h);
	glVertex2i(x, y+h);
	glEnd();
	// Value
	glColor3f(0.0f,0.0f,0.0f);
	if (_curObject && _curObject->type == OBJ_TEXTFIELD && _curObject->object.textField == field)
	{ // Being typed
		value = newString(renameString);
		selected = 1;
	}
	else
	{
		if (*field->value)
		{
			value = getFieldValue(field);
		}
	}
	if (value)
	{
		int len = strlen(value);
		renderString(value, ((int)x+((int)w-(len*8))/2), y+14, selected);
		mem_free(value);
	}
}

void drawList(list_t *list, uint x, uint y, uint w, uint h)
{
	uint i;
	// Border
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x+w, y);
	glVertex2i(x+w, y+h);
	glVertex2i(x, y+h);
	glEnd();
	// Buttons
	// - Top
	if (list->startIndex > 0)
		glColor3f(0.0f, 0.0f, 0.0f);
	else
		glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x+w, y);
	glVertex2i(x+w, y+20);
	glVertex2i(x, y+20);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2i(x+(w-8)/2,y+14);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '^'); // TODO: Draw an actual arrow
	// - Bottom
	if (list->startIndex+list->maxEntries < list->listLength)
		glColor3f(0.0f, 0.0f, 0.0f);
	else
		glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	glVertex2i(x, y+h-20);
	glVertex2i(x+w, y+h-20);
	glVertex2i(x+w, y+h);
	glVertex2i(x, y+h);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2i(x+(w-8)/2,y+h-9);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'v'); // TODO: Draw an actual arrow
	// List
	for (i=list->startIndex; i<list->listLength && i-list->startIndex<list->maxEntries; i++)
	{
		int y2 = y + 20 + 15 * ((i-list->startIndex)+1);
		int j=0;
		if (i == list->selectIndex)
		{ // Selected option
			glColor3f(0.0f,0.0f,0.75f);
			glBegin(GL_QUADS);
			glVertex2i(x+2, y2);
			glVertex2i(x+w-2, y2);
			glVertex2i(x+w-2, y2-15);
			glVertex2i(x+2, y2-15);
			glEnd();
			glColor3f(1.0f,1.0f,1.0f);
		}
		else
		{
			glColor3f(0.0f,0.0f,0.0f);
		}
		if (i == list->selectIndex && 
			_curObject && _curObject->type == OBJ_LIST && _curObject->object.list == list)
		{ // Option being renamed
			glRasterPos2i(x+3, y2-2);
			while (renameString[j] != '\0')
			{
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13, renameString[j]); 
				j++;
			}
			if (j < 15)
			{ // Raster
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '_');
			}
		}
		else
		{
			if (!list->list[i])
			{
				renderString("Unnamed entry", x+3, y2-2, 0);
			}
			else if (strlen(list->list[i]))
			{
				renderString(list->list[i], x+3, y2-2, 0);
			}
			else
			{
				renderString("(empty)", x+3, y2-2, 0);
			}
		}
	}
}

void drawSlider(slider_t *slider, uint x, uint y, uint length)
{
	glBegin(GL_LINES);
	// Axis
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2i(x, y-5);
	glVertex2i(x, y+5);

	glVertex2i(x, y);
	glVertex2i(x+length, y);

	glVertex2i(x+length, y-5);
	glVertex2i(x+length, y+5);

	// Cursor
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2i(x+(slider->percentage*length), y-5);
	glVertex2i(x+(slider->percentage*length), y+5);
	glEnd();
}

void drawCheckBox(checkBox_t *checkBox, uint x, uint y, uint size)
{
	// Border
	glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x+size, y);
	glVertex2i(x+size, y+size);
	glVertex2i(x, y+size);
	glEnd();
	if (checkBox->value && *checkBox->value)
	{ // Check sign
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		glVertex2i(x, y);
		glVertex2i(x+size, y+size);

		glVertex2i(x+size, y);
		glVertex2i(x, y+size);
		glEnd();
	}
}

void drawRadioButton(radio_t *rButton, uint x, uint y, uint radius)
{
	// Border FIXME: MAKE THIS A CIRCLE
	glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x+radius, y);
	glVertex2i(x+radius, y+radius);
	glVertex2i(x, y+radius);
	glEnd();
	if (rButton->value && *rButton->value)
	{ // Check sign
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		glVertex2i(x, y);
		glVertex2i(x+radius, y+radius);

		glVertex2i(x+radius, y);
		glVertex2i(x, y+radius);
		glEnd();
	}
}

void drawInterface(void)
{
	uint i,j;

	// Background
#if 1
	glColor3f(0.2f, 0.2f, 0.1f);
#else
	glColor3f((float)rand()/(float)RAND_MAX, (float)rand()/(float)RAND_MAX, (float)rand()/(float)RAND_MAX);
#endif
	glBegin(GL_POINTS);
	for (i=0; i<INTERFACE_WIDTH; i++)
		for (j=0; j<INTERFACE_HEIGHT; j++)
			glVertex2i(i,j);
	glEnd();

	for (i=0; i<_interface.nbMenus; i++)
	{
		for (j=0; j<_interface.menus[i].nbObjects; j++)
		{
			object_t *object = &_interface.menus[i].objects[j];
			int x, y;

			x = _interface.menus[i].x + object->x - _interface.x;
			y = _interface.menus[i].y + object->y - _interface.y;

			switch (_interface.menus[i].objects[j].type)
			{
			case OBJ_LABEL:
				drawLabel(object->object.label, x, y);
				break;
			case OBJ_BUTTON:
				drawButton(object->object.button, x, y, object->w, object->h);
				break;
			case OBJ_TEXTFIELD:
				drawTextField(object->object.textField, x, y, object->w, object->h);
				break;
			case OBJ_LIST:
				drawList(object->object.list, x, y, object->w, object->h);
				break;
			case OBJ_SLIDER:
				drawSlider(object->object.slider, x, y, object->w);
				break;
			case OBJ_CHECKBOX:
				drawCheckBox(object->object.checkBox, x, y, object->w);
				break;
			case OBJ_RADIOBUTTON:
				drawRadioButton(object->object.radioButton, x, y, object->w);
				break;
			default:
				break;
			}
		}
	}
}

void updateSliderValue(slider_t *slider)
{
	if (slider->value)
	{
		*slider->value = slider->min+slider->percentage*(float)(slider->max-slider->min);
	}

	glutSetWindow(1);
	//drawScene(); // Force the update because the window is busy
	glutPostRedisplay(); // This will have to do for now, but it does nothing
	glutSetWindow(2);
}

void interfaceMouse(int button, int state, int x, int y)	// I want to make these unsigned but I caaaan't :(
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{ // Check for interactions with components (in opposite order compared to render)
		uint i=0,j;
		int actualX, actualY;

		actualX = _interface.x + x;
		actualY = _interface.y + y;

		if (renameString)
		{ // Cancel it FIXME: Don't do that if we clicked on the object?
			mem_free(renameString);
			renameString = NULL;
			renameStringCursor = 0;
			_curObject = NULL;
			drawInterface();
			glutSetWindow(1);
			return;
		}

		while (i<_interface.nbMenus)
		{
			j=0;
			while (j<_interface.menus[i].nbObjects)
			{
				object_t *object = &_interface.menus[i].objects[j];
				int objectX, objectY;
				objectX = _interface.menus[i].x + object->x;
				objectY = _interface.menus[i].y + object->y;

				switch (object->type)
				{
				case OBJ_BUTTON:
					{
						button_t *button = object->object.button;
						if (isInRange(objectX, actualX, objectX+object->w)
							&& isInRange(objectY, actualY, objectY+object->h)
							&& button->onClick)
						{
							button->onClick();
							drawInterface();
							glutSetWindow(1);
							return;
						}
					}
					break;
				case OBJ_TEXTFIELD:
					{
						textField_t *field = object->object.textField;
						if (isInRange(objectX, actualX, objectX+object->w)
							&& isInRange(objectY, actualY, objectY+object->h)
							&& field->editable)
						{
							_curObject = &_interface.menus[i].objects[j];
							renameString = getFieldValue(field);
							drawInterface();
							glutSetWindow(1);
							return;
						}
					}
					break;
				case OBJ_LIST:
					{
						list_t *list = object->object.list;
						if (isInRange(objectX, actualX, objectX+object->w)
							&& isInRange(objectY, actualY, objectY+object->h))
						{
							if (y < (int)(object->y+20))
							{
								if (list->startIndex > 0)
									list->startIndex--;
							}
							else if (y > (int)(object->y+object->h-20))
							{
								if (list->startIndex+list->maxEntries < list->listLength)
									list->startIndex++;
							}
							else
							{
								listSelect(list, list->startIndex+(y-object->y-20)/15);
							}
							drawInterface();
							glutSetWindow(1);
							return;
						}
					}
					break;
				case OBJ_SLIDER:
					{
						slider_t *slider = object->object.slider;
						if (isInRange(objectX, actualX, objectX+object->w)
							&& isInRange(objectY-5, actualY, objectY+5))
						{
							slider->percentage = (float)(x-object->x)/(float)object->w;
							updateSliderValue(slider);
							_curObject = &_interface.menus[i].objects[j];
							drawInterface();
							glutSetWindow(1);
							return;
						}
					}
					break;
				case OBJ_CHECKBOX:
					{
						checkBox_t *checkBox = object->object.checkBox;
						if (isInRange(objectX, actualX, objectX+object->w)
							&& isInRange(objectY, actualY, objectY+object->h))
						{
							if (checkBox->value)
							{
								*checkBox->value = !*checkBox->value;
							}
							drawInterface();
							glutSetWindow(1);
							glutPostRedisplay(); // Temporary... maybe?
							return;
						}
					}
					break;
				case OBJ_RADIOBUTTON:
					{
						radio_t *rButton = object->object.radioButton;
						if (isInRange(objectX, actualX, objectX+object->w)
							&& isInRange(objectY, actualY, objectY+object->h))
						{
							if (rButton->value && !*rButton->value)
							{
								unselectRadioGroup(i, rButton->group);
								*rButton->value = 1;
							}
							drawInterface();
							glutSetWindow(1);
							return;
						}
					}
					break;
				default:
					break;
				}
				j++;
			}
			i++;
		}
	}
	else
	{ // Nothing, just go back to the previous window
		if (_curObject && _curObject->type == OBJ_SLIDER)
			_curObject = NULL;

		glutSetWindow(1);
	}
}

void interfaceMotion(int x, int y)
{
	slider_t *slider;

	if (!_curObject || _curObject->type != OBJ_SLIDER)
		return;

	slider = _curObject->object.slider;
	slider->percentage = (float)(x-_curObject->x)/(float)_curObject->w;
	if (slider->percentage < 0)
		slider->percentage = 0;
	if (slider->percentage > 1)
		slider->percentage = 1;
	updateSliderValue(slider);

	drawInterface();
	glutSetWindow(1);
}

short isAcceptedSpecialChar(char c)
{
	return (c == '_' ||
		c == '-' ||
		c == '+' ||
		c == '/' ||
		c == '*' ||
		c == '[' ||
		c == ']' ||
		c == '(' ||
		c == ')' ||
		c == '{' ||
		c == '}' ||
		c == '|' ||
		c == ',' ||
		c == ':' ||
		c == ';' ||
		c == '.' ||
		c == '!' ||
		c == '?' ||
		c == '<' ||
		c == '>' ||
		c == '^' ||
		c == '&' ||
		c == '#' ||
		c == '$' ||
		c == '%');
}


void interfaceKeyboard(unsigned char key, int x, int y)
{
	uint pos;

	if (!renameString)
		return;

	if (!_curObject)
		return;

	pos = strlen(renameString);

	if (key == 8)
	{ // Backspace
		if (pos < 1)
		{
			return;
		}
		renameString[pos-1] = '\0';
		renameString = (char*)mem_realloc(renameString, sizeof(char)*(pos));
	}
	else if (key == 13)
	{ // Enter
		if (pos < 1)
		{
			return;
		}
		if (_curObject->type == OBJ_TEXTFIELD)
		{
			textField_t *field = _curObject->object.textField;
			if (field->type == FIELDTYPE_INT)
			{
				int value = atoi(renameString);
				if (value < field->min)
					value = field->min;
				if (value > field->max)
					value = field->max;
				*((int*)(*field->value)) = value;
			}
			else if (field->type == FIELDTYPE_FLOAT)
			{
				float value = (float)atof(renameString);
				if (value < field->min)
					value = field->min;
				if (value > field->max)
					value = field->max;
				*((float*)(*field->value)) = value;
			}
			else if (field->type == FIELDTYPE_BINARY)
			{ // Compile the binary data (we expect that the size of the container did not change...)
				uint i = 0;
				byte counter = 0;
				uint size = mem_size(*field->value);
				if (!size) size = 1;
				while (i < 9*size) // Make sure we don't go out of bounds
				{
					if (renameString[i] == ' ' || renameString[i] == '\0')
					{
						((byte*)*field->value)[i/9] = counter;
						counter = 0;
						if (renameString[i] == '\0')
						{
							break;
						}
					}
					else
					{
						counter = counter * 2;
						if (renameString[i] == '1') counter += 1;
					}
					i++;
				}
			}
			else
			{
				strcpy_safe((char*)(*field->value), renameString);
			}
			mem_free(renameString);
			_curObject = NULL;

			glutSetWindow(1);
			glutPostRedisplay();
			glutSetWindow(2);
		}
		else if (_curObject->type == OBJ_LIST)
		{
			list_t *list = _curObject->object.list;
			int index = list->selectIndex;
			strcpy_safe(list->list[index], renameString);
			mem_free(renameString);
			list->onSelect(index); // Update it	
		}
		renameString = NULL;
	}
	else if (key == 27)
	{ // Escape
		mem_free(renameString);
		renameString = NULL;
		_curObject = NULL;
	}
	else
	{ // Normal input
		if (_curObject->type == OBJ_TEXTFIELD && _curObject->object.textField->type != FIELDTYPE_TEXT)
		{ // Only take numbers

			if (_curObject->object.textField->type == FIELDTYPE_BINARY)
			{ // Only two choices here really
				if (key != '0' && key != '1')
				{
					return;
				}
			}
			else
			{
				if (key < '0' || key > '9')
				{
					if (key == '.' && _curObject->object.textField->type != FIELDTYPE_FLOAT)
					{
						return;
					}
				}

				if ((_curObject->object.textField->max > 0 && pos >= nbDigits(_curObject->object.textField->max))
					|| pos >= 8)
				{
					return;
				}
			}
		}
		else
		{ // Take most things
			if ((key < 'a' || key > 'z') &&
				(key < 'A' || key > 'Z') &&
				(key < '0' || key > '9') &&
				!isAcceptedSpecialChar(key) &&
				((key != ' ') || pos == 0)) // Can't start with a space
			{
				return;
			}

			if (pos >= 15)
			{
				return;
			}
		}
		
		renameString = (char*)mem_realloc(renameString, sizeof(char)*(pos+2));
		renameString[pos] = key;
		renameString[pos+1] = '\0';
	}

	drawInterface();
	glutSetWindow(1);
}

void reshapeInterface(int w, int h)
{
	glViewport(0, 0,(GLsizei) w, (GLsizei) h);
	drawInterface();
	glutSetWindow(1);
	//glutPositionWindow(0,0);
}

void initInterface(int window)
{
	//clipboard = (keyFrame_t*)mem_alloc(sizeof(keyFrame_t));
	//clipboard->name = NULL;

	renameString = NULL;

	_interface.menus = NULL;
	_interface.nbMenus = 0;
	_interface.targX = 0;
	_interface.targY = 0;
	_interface.x = 0;
	_interface.y = 0;
	_interface.transTime = 0;

	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutCreateSubWindow(window, 0, RENDER_HEIGHT, INTERFACE_WIDTH, INTERFACE_HEIGHT);

    glDisable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	gluOrtho2D(0.0, INTERFACE_WIDTH, INTERFACE_HEIGHT, 0.0);
	glPointSize(1.0f);

	glutDisplayFunc(drawInterface);
	glutMouseFunc(interfaceMouse);
	glutReshapeFunc(reshapeInterface);
	glutMotionFunc(interfaceMotion);
	glutKeyboardFunc(interfaceKeyboard);
}