include ../common.mk 
bin_PROGRAMS = conf_model_deg conf_model_deg_nocheck
conf_model_deg_SOURCES = conf_model_deg.c ../utils/utils.c ../utils/iltree.c \
	../utils/iltree_double.c ../include/utils.h ../include/iltree.h \
../include/iltree_double.h ../include/utils.h
conf_model_deg_LDADD = -lm
conf_model_deg_nocheck_SOURCES = conf_model_deg_nocheck.c ../utils/utils.c \
	../utils/iltree.c ../utils/iltree_double.c ../include/iltree.h \
../include/iltree_double.h 
conf_model_deg_nocheck_LDADD = -lm
