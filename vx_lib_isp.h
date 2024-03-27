/*
 * Copyright (c) 2024 Karthik Poduval <karthik.poduval@gmail.com> 
 *
 * All rights reserved. This program and the accompanying materials 
 * are made available under the terms of the Apache License, Version 2.0 
 * which accompanies this distribution, and is available at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef _OPENVX_EXT_ISP_H_
#define _OPENVX_EXT_ISP_H_

#include <VX/vx.h>

#define VX_KERNEL_NAME_KHR_ISP_DEMOSAIC "org.isp.demosaic"
#define VX_LIBRARY_ISP (0x3)	// assigned from Khronos, vendors control their own

/*! \brief The list of ISP Kernels.
 * \ingroup group_xyz_ext
 */
enum vx_kernel_isp_e {
	/*! \brief The Example User Defined Kernel */
	VX_KERNEL_KHR_DEMOSAIC =
	    VX_KERNEL_BASE(VX_ID_DEFAULT, VX_LIBRARY_ISP) + 0x0,
	// up to 0xFFF kernel enums can be created.
};

enum vx_isp_demosaic_pattern_e {
	VX_DEMOSAIC_PATTERN_BGGR = 0x0,
	VX_DEMOSAIC_PATTERN_GBRG = 0x1,
	VX_DEMOSAIC_PATTERN_GRBG = 0x2,
	VX_DEMOSAIC_PATTERN_RGGB = 0x3,
};

enum vx_isp_bayer_pixel_e {
	VX_DEMOSAIC_PIXEL_B = 0x0,
	VX_DEMOSAIC_PIXEL_Gr = 0x1,
	VX_DEMOSAIC_PIXEL_Gb = 0x2,
	VX_DEMOSAIC_PIXEL_R = 0x3,
};

#ifdef __cplusplus
extern "C" {
#endif

	vx_node vxDemosaicNode(vx_graph graph, vx_image input, vx_image output);

	vx_status vxuDemosaic(vx_context context, vx_image input,
			      vx_uint32 value, vx_image output, vx_array temp);

#ifdef __cplusplus
}
#endif
#endif
