#include <SDL/SDL.h>
#include <GL/glew.h>
#include <mz/libs.h>
#include <mz/image.h>

static SDL_Surface *display_surface;
static GLuint gl_texture;

void mz_graphics_clear()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void mz_graphics_flush()
{
    SDL_GL_SwapBuffers();
}

void mz_graphics_init()
{
    int width = 640;
    int height = 480;

	SDL_Init(SDL_INIT_EVERYTHING);

    if((display_surface = SDL_SetVideoMode(width, height, 32, 
        SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL) {
            return;
    }

    glClearColor(0, 0, 0, 0);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 1, -1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &gl_texture);
}

static GLuint get_pixel_format(SDL_Surface *surface)
{
    GLuint format = 0;

	if(surface->format->BytesPerPixel == 4)
		format = (surface->format->Rmask==0x000000ff) ? GL_RGBA : GL_BGRA;
	else if(surface->format->BytesPerPixel == 3)
		format = (surface->format->Rmask==0x000000ff) ? GL_RGB : GL_BGR;
	else
		logI("wrong image format.");

    return format;
}

void mz_graphics_draw_texture(mz_image_t *image, int x, int y)
{
    float tex_max_x = (float)image->real_w / (float)image->w;
    float tex_max_y = (float)image->real_h / (float)image->h;
    int w = image->real_w;
    int h = image->real_h;

    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, gl_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, image->bytes_per_pixel, 
                                   image->w, image->h, 
                                   0, image->format,
                                   GL_UNSIGNED_BYTE, image->pixels);

    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        glTexCoord2f(0,         0);         glVertex3i(x,     y,     0);
        glTexCoord2f(tex_max_x, 0);         glVertex3i(x + w, y,     0);
        glTexCoord2f(tex_max_x, tex_max_y); glVertex3i(x + w, y + h, 0);
        glTexCoord2f(0,         tex_max_y); glVertex3i(x,     y + h, 0);
    glEnd();
}
