mod_header_note.la: mod_header_note.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_header_note.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_header_note.la
