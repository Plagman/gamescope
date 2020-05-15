// DRM output stuff

#pragma once

#include <xf86drm.h>
#include <xf86drmMode.h>
#include <assert.h>
#include <drm_fourcc.h>

#include <wlr/render/dmabuf.h>

#include "rendervulkan.hpp"

#ifndef C_SIDE

#include <unordered_map>
#include <utility>
#include <atomic>
#include <mutex>
#include <vector>

struct plane {
	drmModePlane *plane;
	drmModeObjectProperties *props;
	drmModePropertyRes **props_info;
};

struct crtc {
	drmModeCrtc *crtc;
	drmModeObjectProperties *props;
	drmModePropertyRes **props_info;
};

struct connector {
	drmModeConnector *connector;
	drmModeObjectProperties *props;
	drmModePropertyRes **props_info;
};

struct drm_t {
	int fd;
	
	/* only used for atomic: */
	struct plane *plane;
	struct crtc *crtc;
	struct connector *connector;
	int crtc_index;
	int kms_in_fence_fd;
	int kms_out_fence_fd;
	
	drmModeModeInfo *mode;
	uint32_t crtc_id;
	uint32_t connector_id;
	
	uint32_t plane_id;
	
	drmModeAtomicReq *req;
	uint32_t flags;
	
	struct liftoff_device *lo_device;
	struct liftoff_output *lo_output;
	struct liftoff_layer *lo_layers[ k_nMaxLayers ];
	
	std::vector < uint32_t > fbids_in_req;
	std::vector < uint32_t > fbids_on_screen;
	
	std::unordered_map< uint32_t, std::pair< bool, std::atomic< uint32_t > > > map_fbid_inflightflips;
	std::unordered_map< uint32_t, struct wlr_buffer * > map_fbid_buf;
	std::mutex free_queue_lock;
	std::vector< uint32_t > fbid_free_queue;
	
	std::mutex flip_lock;
	
	std::atomic < uint64_t > flipcount;
};
#endif

#ifndef C_SIDE
extern "C" {
#endif
	
#include "libliftoff.h"

extern struct drm_t g_DRM;

extern uint32_t g_nDRMFormat;

extern bool g_bUseLayers;
extern bool g_bRotated;
extern bool g_bDebugLayers;

int init_drm(struct drm_t *drm, const char *device, const char *mode_str, unsigned int vrefresh);
int drm_atomic_commit(struct drm_t *drm, struct Composite_t *pComposite, struct VulkanPipeline_t *pPipeline );
uint32_t drm_fbid_from_dmabuf( struct drm_t *drm, struct wlr_buffer *buf, struct wlr_dmabuf_attributes *dma_buf );
void drm_free_fbid( struct drm_t *drm, uint32_t fbid );
bool drm_can_avoid_composite( struct drm_t *drm, struct Composite_t *pComposite, struct VulkanPipeline_t *pPipeline );

#ifndef C_SIDE
}
#endif
