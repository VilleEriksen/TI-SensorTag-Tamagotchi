# invoke SourceDir generated makefile for empty.pem3
empty.pem3: .libraries,empty.pem3
.libraries,empty.pem3: package/cfg/empty_pem3.xdl
	$(MAKE) -f C:\Users\Ville\DOCUME~1\LINUXS~1\gateway/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\Ville\DOCUME~1\LINUXS~1\gateway/src/makefile.libs clean

