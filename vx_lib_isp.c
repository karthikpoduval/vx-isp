/*
 * Copyright (c) 2024 Karthik Poduval <karthik.poduval@gmail.com> 
 *
 * All rights reserved. This program and the accompanying materials 
 * are made available under the terms of the Apache License, Version 2.0 
 * which accompanies this distribution, and is available at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>
#include "vx_lib_isp.h"

extern vx_kernel_description_t demosaic_kernel;

/*
 * list of kernels contained in this module
 * */
static vx_kernel_description_t *isp_kernels[] = {
	&demosaic_kernel,
};

static vx_uint32 num_kernels = dimof(isp_kernels);

vx_status VX_API_CALL vxPublishKernels(vx_context context)
{
	vx_uint32 p = 0;
	vx_uint32 k = 0;
	vx_status status = VX_FAILURE;

	for (k = 0; k < num_kernels; k++) {
		vx_kernel kernel = vxAddUserKernel(context,
						   isp_kernels[k]->name,
						   isp_kernels[k]->enumeration,
						   isp_kernels[k]->function,
						   isp_kernels[k]->numParams,
						   isp_kernels[k]->validate,
						   isp_kernels[k]->initialize,
						   isp_kernels[k]->
						   deinitialize);

		if (kernel) {
			status = VX_SUCCESS;	// temporary
			for (p = 0; p < isp_kernels[k]->numParams; p++) {
				status = vxAddParameterToKernel(kernel, p,
								isp_kernels[k]->
								parameters
								[p].direction,
								isp_kernels[k]->
								parameters
								[p].data_type,
								isp_kernels[k]->
								parameters
								[p].state);
				if (status != VX_SUCCESS) {
					vxAddLogEntry((vx_reference) context,
						      status,
						      "Failed to add parameter %d to kernel %s! (%d)\n",
						      p, isp_kernels[k]->name,
						      status);
					break;
				}
			}
			if (status == VX_SUCCESS) {
				status = vxFinalizeKernel(kernel);
				if (status != VX_SUCCESS) {
					vxAddLogEntry((vx_reference) context,
						      status,
						      "Failed to finalize kernel[%u]=%s\n",
						      k, isp_kernels[k]->name);
				}
			} else {
				status = vxRemoveKernel(kernel);
				if (status != VX_SUCCESS) {
					vxAddLogEntry((vx_reference) context,
						      status,
						      "Failed to remove kernel[%u]=%s\n",
						      k, isp_kernels[k]->name);
				}
			}
			/*! \todo should i continue with errors or fail and unwind? */
		} else {
			vxAddLogEntry((vx_reference) context, status,
				      "Failed to add kernel %s\n",
				      isp_kernels[k]->name);
		}
	}

	return status;
}

vx_status VX_API_CALL vxUnpublishKernels(vx_context context)
{
	vx_status status = VX_FAILURE;

	vx_uint32 k = 0;
	for (k = 0; k < num_kernels; k++) {
		vx_kernel kernel =
		    vxGetKernelByName(context, isp_kernels[k]->name);
		vx_kernel kernelcpy = kernel;

		if (kernel) {
			status = vxReleaseKernel(&kernelcpy);
			if (status != VX_SUCCESS) {
				vxAddLogEntry((vx_reference) context, status,
					      "Failed to release kernel[%u]=%s\n",
					      k, isp_kernels[k]->name);
			} else {
				kernelcpy = kernel;
				status = vxRemoveKernel(kernelcpy);
				if (status != VX_SUCCESS) {
					vxAddLogEntry((vx_reference) context,
						      status,
						      "Failed to remove kernel[%u]=%s\n",
						      k, isp_kernels[k]->name);
				}
			}
		} else {
			vxAddLogEntry((vx_reference) context, status,
				      "Failed to get added kernel %s\n",
				      isp_kernels[k]->name);
		}
	}

	return status;
}
