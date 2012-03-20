#ifndef __MZ_EVENT_H__
#define __MZ_EVENT_H__

#include <mz/defs.h>

typedef struct mz_keyboard_t
{
	int	keycode;
	int	unichar;
} mz_keyboard_t;

typedef struct mz_event_t
{
	int		type;
	mz_keyboard_t 	keyboard;
} mz_event_t;

typedef struct mz_event_type_t
{
	int	update_frame;
	int	key_down;
	int	key_up;
} mz_event_type_t;

typedef struct mz_keys_t
{
	int	Escape;
	int	Enter;
	int	Space;

	int	Left;
	int	Right;
	int	Up;
	int	Down;

	int	Backspace;
	
} mz_keys_t;

typedef struct mz_constants_t
{
	mz_event_type_t	types;
	mz_keys_t	keys;
} mz_constants_t;

MZ_API const mz_constants_t mz;

#endif
