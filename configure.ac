AC_INIT([netbunch], [1.0], [v.nicosia@qmul.ac.uk])
AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])
: ${CFLAGS=""}
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile src/ba/Makefile src/er/Makefile src/graph_info/Makefile \
                 src/dms/Makefile src/bb_fitness/Makefile src/bbv/Makefile src/conf/Makefile \
                 src/deg_seq/Makefile src/power_law/Makefile src/shortest/Makefile\
                 src/components/Makefile src/kruskal/Makefile src/clust/Makefile \
                 src/fitmle/Makefile src/dijkstra/Makefile src/f3m/Makefile \
                 src/betweenness/Makefile src/label_prop/Makefile src/cnm/Makefile \
                 src/johnson/Makefile src/gn/Makefile src/modularity/Makefile \
                 src/pm/Makefile src/knn/Makefile src/hidden/Makefile \
                 src/ws/Makefile doc/Makefile examples/Makefile])
AC_CHECK_PROGS([RONN], [ronn])
if test -z "$RONN"; then
   AC_MSG_WARN([ronn not found -- continuing without document rebuild support])
fi
AM_CONDITIONAL([HAVE_RONN], [test -n "$RONN"])
AC_OUTPUT
