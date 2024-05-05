# vx-isp
Open VX extension user kernel for ISP (Image Signal Processor)

First build OpenVX reference implementaion on Linux

* Git Clone project with a recursive flag to get submodules

      git clone --recursive https://github.com/karthikpoduval/OpenVX-sample-impl.git

* Use Build.py script

      cd OpenVX-sample-impl/
      sudo python3 Build.py --os=Linux --machine_install
      
      
* build vx-isp
    
        meson build
        ninja -C build

* run test: This Test Converts a build in VGA Bayer Test Pattern to RGB using VX ISP Demosaic Node

        build/test_vx_isp

* convert the RGB image and scaled image into a png for viewing

        convert -depth 8 -size 640x480 rgb:demosaic.rgb pic.png
	convert -depth 8 -size 1280x960 rgb:scaled.rgb scaled.png	
