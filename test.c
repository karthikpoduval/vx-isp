#include <VX/vx.h>
#include "vx_lib_isp.h"
#include <VX/vx_lib_debug.h>
#include <VX/vx_helper.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bayer.h"

void vxLogCb(vx_context context,
	     vx_reference ref, vx_status status, const vx_char string[])
{
	size_t len = strlen(string);
	if (len > 0) {
		printf("%s", string);
		if (string[len - 1] != '\n')
			printf("\n");
		fflush(stdout);
	}
}

vx_status fillTestPattern(vx_image bayerImg)
{

	vx_imagepatch_addressing_t addr;
	vx_map_id map_id;
	vx_rectangle_t rect;
	vx_status status = VX_SUCCESS;
	void *base_ptr = NULL;
	vx_uint32 plane = 0, x, y;
	status = vxGetValidRegionImage(bayerImg, &rect);
	if (status != VX_SUCCESS) {
		printf("Could not get image rect\n");
		exit(-1);
		return status;
	}

	status = vxMapImagePatch(bayerImg, &rect, plane, &map_id,
				 &addr, &base_ptr,
				 VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST, 0);

	if (status != VX_SUCCESS) {
		printf("Could not map image\n");
		exit(-1);
		return status;
	}
	memcpy(base_ptr, bayer_rggb, 640 * 480 * 2);
	/* 2d addressing option */
	for (y = 0; y < addr.dim_y; y += addr.step_y) {
		for (x = 0; x < addr.dim_x; x += addr.step_x) {
			vx_uint16 *ptr = vxFormatImagePatchAddress2d(base_ptr,
								     x, y,
								     &addr);
			/* scale to RAW16 */
			*ptr = *ptr << 4;
		}
	}
	status = vxUnmapImagePatch(bayerImg, map_id);
	if (status != VX_SUCCESS) {
		printf("Could not unmap image\n");
		exit(-1);
		return status;
	}

	return status;
}

int main()
{
	vx_status status = VX_FAILURE;
	vx_context context = vxCreateContext();

	vxRegisterLogCallback(context, vxLogCb, vx_false_e);
	if (vxGetStatus((vx_reference) context) == VX_SUCCESS) {
		vx_uint32 i = 0;
		vx_uint32 w = 640, h = 480;
		vx_char ext_kernels[][VX_MAX_FILE_NAME] = {
			"openvx-isp",
			"openvx-debug"
		};
		vx_image images[] = {
			vxCreateImage(context, w, h, VX_DF_IMAGE_U16),	/* 0: u16 */
			vxCreateImage(context, w, h, VX_DF_IMAGE_RGB),	/* 1: rgb */
		};
		fillTestPattern(images[0]);
		status = VX_SUCCESS;
		for (i = 0; i < dimof(images); i++) {
			if (images[i] == 0) {
				status = VX_ERROR_NOT_SUFFICIENT;
			}
		}
		for (i = 0; i < dimof(ext_kernels); i++) {
			status |= vxLoadKernels(context, ext_kernels[i]);
			printf("load kernel=%s\n", ext_kernels[i]);
		}
		if (status == VX_SUCCESS) {
			vx_graph graph = vxCreateGraph(context);
			if (vxGetStatus((vx_reference) graph) == VX_SUCCESS) {
				vx_node nodes[] = {
					vxDemosaicNode(graph, images[0],
						       images[1]),
				};
				if (status == VX_SUCCESS) {
					status = vxVerifyGraph(graph);
					if (status == VX_SUCCESS) {
						status = vxProcessGraph(graph);
						printf
						    ("Graph run successfully\n");
					} else {
						printf
						    ("graph processing failed");
					}
					for (i = 0; i < dimof(nodes); i++) {
						vxReleaseNode(&nodes[i]);
					}
				}
				vxReleaseGraph(&graph);
			}
			vxuFWriteImage(context, images[1], "demosaic.rgb");
			for (i = 0; i < dimof(ext_kernels); i++) {
				status |=
				    vxUnloadKernels(context, ext_kernels[i]);
				printf("unload kernel=%s\n", ext_kernels[i]);
			}
		}
		for (i = 0; i < dimof(images); i++) {
			vxReleaseImage(&images[i]);
		}
		vxReleaseContext(&context);
	}

}
