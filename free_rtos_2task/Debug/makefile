################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./source/can.obj" \
"./source/dabort.obj" \
"./source/esm.obj" \
"./source/gio.obj" \
"./source/notification.obj" \
"./source/os_croutine.obj" \
"./source/os_event_groups.obj" \
"./source/os_heap.obj" \
"./source/os_list.obj" \
"./source/os_mpu_wrappers.obj" \
"./source/os_port.obj" \
"./source/os_portasm.obj" \
"./source/os_queue.obj" \
"./source/os_tasks.obj" \
"./source/os_timer.obj" \
"./source/pinmux.obj" \
"./source/sci.obj" \
"./source/sys_core.obj" \
"./source/sys_dma.obj" \
"./source/sys_intvecs.obj" \
"./source/sys_main.obj" \
"./source/sys_mpu.obj" \
"./source/sys_pcr.obj" \
"./source/sys_phantom.obj" \
"./source/sys_pmm.obj" \
"./source/sys_pmu.obj" \
"./source/sys_selftest.obj" \
"./source/sys_startup.obj" \
"./source/sys_vim.obj" \
"./source/system.obj" \
"../source/sys_link.cmd" \
$(GEN_CMDS__FLAG) \
-l"rtsv7R4_T_be_v3D16_eabi.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include source/subdir_vars.mk
-include source/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
free_rtos_2task.out \

EXE_OUTPUTS__QUOTED += \
"free_rtos_2task.out" \

BIN_OUTPUTS += \
free_rtos_2task.hex \

BIN_OUTPUTS__QUOTED += \
"free_rtos_2task.hex" \


# All Target
all: free_rtos_2task.out

# Tool invocations
free_rtos_2task.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7R4 --code_state=32 --float_support=VFPv3D16 --abi=eabi -g --gcc --diag_wrap=off --diag_warning=225 --display_error_number --enum_type=packed -z -m"free_rtos_2task.map" --heap_size=0x800 --stack_size=0x800 -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/lib" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --reread_libs --display_error_number --diag_wrap=off --warn_sections --xml_link_info="free_rtos_2task_linkInfo.xml" --rom_model --be32 -o "free_rtos_2task.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

free_rtos_2task.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armhex"  -o "free_rtos_2task.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "source\can.pp" "source\esm.pp" "source\gio.pp" "source\notification.pp" "source\os_croutine.pp" "source\os_event_groups.pp" "source\os_heap.pp" "source\os_list.pp" "source\os_mpu_wrappers.pp" "source\os_port.pp" "source\os_queue.pp" "source\os_tasks.pp" "source\os_timer.pp" "source\pinmux.pp" "source\sci.pp" "source\sys_dma.pp" "source\sys_main.pp" "source\sys_pcr.pp" "source\sys_phantom.pp" "source\sys_pmm.pp" "source\sys_selftest.pp" "source\sys_startup.pp" "source\sys_vim.pp" "source\system.pp" 
	-$(RM) "source\can.obj" "source\dabort.obj" "source\esm.obj" "source\gio.obj" "source\notification.obj" "source\os_croutine.obj" "source\os_event_groups.obj" "source\os_heap.obj" "source\os_list.obj" "source\os_mpu_wrappers.obj" "source\os_port.obj" "source\os_portasm.obj" "source\os_queue.obj" "source\os_tasks.obj" "source\os_timer.obj" "source\pinmux.obj" "source\sci.obj" "source\sys_core.obj" "source\sys_dma.obj" "source\sys_intvecs.obj" "source\sys_main.obj" "source\sys_mpu.obj" "source\sys_pcr.obj" "source\sys_phantom.obj" "source\sys_pmm.obj" "source\sys_pmu.obj" "source\sys_selftest.obj" "source\sys_startup.obj" "source\sys_vim.obj" "source\system.obj" 
	-$(RM) "source\dabort.pp" "source\os_portasm.pp" "source\sys_core.pp" "source\sys_intvecs.pp" "source\sys_mpu.pp" "source\sys_pmu.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

