# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: C:\Users\Char\Documents\ECE_520\Fall_26\axi_gpio_caa\axi_gpio_vitis\axi_gpio_app_system\_ide\scripts\debugger_axi_gpio_app-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source C:\Users\Char\Documents\ECE_520\Fall_26\axi_gpio_caa\axi_gpio_vitis\axi_gpio_app_system\_ide\scripts\debugger_axi_gpio_app-default.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent Zybo Z7 210351BE7AA2A" && level==0 && jtag_device_ctx=="jsn-Zybo Z7-210351BE7AA2A-13722093-0"}
fpga -file C:/Users/Char/Documents/ECE_520/Fall_26/axi_gpio_caa/axi_gpio_vitis/axi_gpio_app/_ide/bitstream/axi_gpio_caa_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw C:/Users/Char/Documents/ECE_520/Fall_26/axi_gpio_caa/axi_gpio_vitis/axi_gpio_plat/export/axi_gpio_plat/hw/axi_gpio_caa_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source C:/Users/Char/Documents/ECE_520/Fall_26/axi_gpio_caa/axi_gpio_vitis/axi_gpio_app/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow C:/Users/Char/Documents/ECE_520/Fall_26/axi_gpio_caa/axi_gpio_vitis/axi_gpio_app/Debug/axi_gpio_app.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "*A9*#0"}
con
