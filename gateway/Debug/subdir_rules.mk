################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"J:/css1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=none -me --include_path="C:/Users/Ville/Documents/Linux Shared/gateway" --include_path="C:/Users/Ville/Documents/Linux Shared/gateway" --include_path="J:/rtos/tirtos_cc13xx_cc26xx_2_21_00_06/products/cc26xxware_2_24_03_17272" --include_path="J:/css1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1067269937:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1067269937-inproc

build-1067269937-inproc: ../empty.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"J:/rtos/xdctools_3_32_00_06_core/xs" --xdcpath="J:/rtos/tirtos_cc13xx_cc26xx_2_21_00_06/packages;J:/rtos/tirtos_cc13xx_cc26xx_2_21_00_06/products/tidrivers_cc13xx_cc26xx_2_21_00_04/packages;J:/rtos/tirtos_cc13xx_cc26xx_2_21_00_06/products/bios_6_46_01_37/packages;J:/rtos/tirtos_cc13xx_cc26xx_2_21_00_06/products/uia_2_01_00_01/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.simplelink:CC2650F128 -r release -c "J:/css1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS" --compileOptions "-mv7M3 --code_state=16 --float_support=none -me --include_path=\"C:/Users/Ville/Documents/Linux Shared/gateway\" --include_path=\"C:/Users/Ville/Documents/Linux Shared/gateway\" --include_path=\"J:/rtos/tirtos_cc13xx_cc26xx_2_21_00_06/products/cc26xxware_2_24_03_17272\" --include_path=\"J:/css1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include\" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi  " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-1067269937 ../empty.cfg
configPkg/compiler.opt: build-1067269937
configPkg/: build-1067269937


