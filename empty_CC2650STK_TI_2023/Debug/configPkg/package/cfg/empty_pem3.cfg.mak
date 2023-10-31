# invoke SourceDir generated makefile for empty.pem3
empty.pem3: .libraries,empty.pem3
.libraries,empty.pem3: package/cfg/empty_pem3.xdl
	$(MAKE) -f C:\Users\Ville\Documents\LinuxShared\TI-SensorTag-Tamagotchi\empty_CC2650STK_TI_2023/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\Ville\Documents\LinuxShared\TI-SensorTag-Tamagotchi\empty_CC2650STK_TI_2023/src/makefile.libs clean

