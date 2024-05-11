// d3d_frame.cpp
//

#include "d3d_local.h"

uint64_t            s_frame_count = 0;

static tr_fence* image_acquired_fence;
static tr_semaphore* image_acquired_semaphore;
static tr_semaphore* render_complete_semaphores;
static uint32_t swapchain_image_index;
static tr_render_target* render_target;
static uint32_t frameIdx;

void R_BeginFrame(float camera_separation) {
	frameIdx = s_frame_count % tr.m_renderer->settings.swapchain.image_count;

	image_acquired_fence = tr.m_renderer->image_acquired_fences[frameIdx];
	image_acquired_semaphore = tr.m_renderer->image_acquired_semaphores[frameIdx];
	render_complete_semaphores = tr.m_renderer->render_complete_semaphores[frameIdx];

	tr_acquire_next_image(tr.m_renderer, image_acquired_semaphore, image_acquired_fence);

	swapchain_image_index = tr.m_renderer->swapchain_image_index;
	render_target = tr.m_renderer->swapchain_render_targets[swapchain_image_index];

	tr.cmd = tr.m_cmds[frameIdx];

	tr_begin_cmd(tr.cmd);
	tr_cmd_render_target_transition(tr.cmd, render_target, tr_texture_usage_present, tr_texture_usage_color_attachment);
	tr_cmd_depth_stencil_transition(tr.cmd, render_target, tr_texture_usage_sampled_image, tr_texture_usage_depth_stencil_attachment);
	tr_cmd_set_viewport(tr.cmd, 0, 0, (float)tr.s_window_width, (float)tr.s_window_height, 0.0f, 1.0f);
	tr_cmd_set_scissor(tr.cmd, 0, 0, tr.s_window_width, tr.s_window_height);
	tr_cmd_begin_render(tr.cmd, render_target);
	tr_clear_value color_clear_value = { 0.0f, 0.0f, 0.0f, 0.0f };
	tr_cmd_clear_color_attachment(tr.cmd, 0, &color_clear_value);
	tr_clear_value depth_stencil_clear_value = { 0 };
	depth_stencil_clear_value.depth = 1.0f;
	depth_stencil_clear_value.stencil = 255;
	tr_cmd_clear_depth_stencil_attachment(tr.cmd, &depth_stencil_clear_value);

}

void R_EndFrame(void) {
	tr_cmd_end_render(tr.cmd);
	tr_cmd_render_target_transition(tr.cmd, render_target, tr_texture_usage_color_attachment, tr_texture_usage_present);
	tr_cmd_depth_stencil_transition(tr.cmd, render_target, tr_texture_usage_depth_stencil_attachment, tr_texture_usage_sampled_image);
	tr_end_cmd(tr.cmd);

	tr_queue_submit(tr.m_renderer->graphics_queue, 1, &tr.cmd, 1, &image_acquired_semaphore, 1, &render_complete_semaphores);
	tr_queue_present(tr.m_renderer->present_queue, 1, &render_complete_semaphores);

	tr_queue_wait_idle(tr.m_renderer->graphics_queue);
}

void R_RenderFrame(refdef_t* fd) {
	// Function body
}
