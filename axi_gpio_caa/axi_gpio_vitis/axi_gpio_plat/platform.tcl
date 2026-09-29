# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct C:\Users\Char\Documents\ECE_520\Fall_26\axi_gpio_caa\axi_gpio_vitis\axi_gpio_plat\platform.tcl
# 
# OR launch xsct and run below command.
# source C:\Users\Char\Documents\ECE_520\Fall_26\axi_gpio_caa\axi_gpio_vitis\axi_gpio_plat\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {axi_gpio_plat}\
-hw {C:\Users\Char\Documents\ECE_520\Fall_26\axi_gpio_caa\axi_gpio_caa_wrapper.xsa}\
-proc {ps7_cortexa9_0} -os {standalone} -out {C:/Users/Char/Documents/ECE_520/Fall_26/axi_gpio_caa/axi_gpio_vitis}

platform write
platform generate -domains 
platform generate
