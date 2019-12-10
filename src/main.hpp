#ifndef C_SIDE
extern "C" {
#endif
	
#include <SDL.h>
#include <SDL_vulkan.h>
	
extern SDL_Window *window;

#include "wlr/render/dmabuf.h"

void initOutput(void);
	
void startSteamCompMgr(void);


struct ResListEntry_t {
	struct wlr_surface *surf;
	struct wlr_dmabuf_attributes attribs;
};

void wayland_PushSurface(struct wlr_surface *surf, struct wlr_dmabuf_attributes *attribs);

int steamCompMgr_PullSurface( struct ResListEntry_t *pResEntry );

extern int g_nNestedWidth;
extern int g_nNestedHeight;
extern int g_nNestedRefresh;

extern uint32_t g_nOutputWidth;
extern uint32_t g_nOutputHeight;

int BIsNested( void );

#ifndef C_SIDE
}
#endif
