
/*
engine_window

- Window creation and management
*/

typedef struct {
	int		id;
	char	*name;
	int		width, height;
} window_t;

// Window settings
#define RENDER_HEIGHT 500
#define INTERFACE_HEIGHT 100

#define DEFAULT_WIDTH 1200
#define DEFAULT_HEIGHT (RENDER_HEIGHT+INTERFACE_HEIGHT)

#define INTERFACE_WIDTH DEFAULT_WIDTH

void createWindow(window_t *window);