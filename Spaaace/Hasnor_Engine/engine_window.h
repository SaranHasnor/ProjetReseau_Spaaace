
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
#define DEFAULT_WIDTH 1200
#define DEFAULT_HEIGHT 600

void createWindow(window_t *window);