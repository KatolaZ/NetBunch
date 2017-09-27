if HAVE_RONN

all-local: ronn

ronn:
	ronn -r5 --style=toc --manual="www.complex-networks.net" --organization="www.complex-networks.net" *.md

endif

docdir = $(prefix)/$(PACKAGE)-doc
dist_doc_DATA =  ba.1.html bb_fitness.1.html bbv.1.html bet_dependency.1.html \
betweenness.1.html clust.1.html clust_w.1.html cnm.1.html components.1.html \
conf_model_deg.1.html conf_model_deg_nocheck.1.html deg_seq.1.html \
deg_seq_w.1.html dijkstra.1.html dms.1.html er_A.1.html er_B.1.html \
f3m.1.html fitmle.1.html gn.1.html graph_info.1.html hv_net.1.html \
johnson_cycles.1.html knn.1.html knn_w.1.html kruskal.1.html \
label_prop.1.html largest_component.1.html modularity.1.html \
node_components.1.html pm.1.html power_law.1.html shortest.1.html \
shortest_avg_max_hist.1.html strong_conn.1.html ws.1.html \
ba.md bb_fitness.md bbv.md bet_dependency.md betweenness.md \
clust.md clust_w.md cnm.md components.md conf_model_deg.md \
conf_model_deg_nocheck.md deg_seq.md deg_seq_w.md dijkstra.md dms.md \
er_A.md er_B.md f3m.md fitmle.md gn.md graph_info.md hv_net.md \
johnson_cycles.md knn.md knn_w.md kruskal.md label_prop.md \
largest_component.md modularity.md node_components.md pm.md power_law.md \
shortest.md shortest_avg_max_hist.md strong_conn.md ws.md
###dist_doc_DATA = $(doc_DATA)


dist_man_MANS = ba.1 bb_fitness.1 bbv.1 bet_dependency.1 betweenness.1 \
clust.1 clust_w.1 cnm.1 components.1 conf_model_deg.1 \
conf_model_deg_nocheck.1 deg_seq.1 deg_seq_w.1 dijkstra.1 dms.1 \
er_A.1 er_B.1 f3m.1 fitmle.1 gn.1 graph_info.1 hv_net.1 \
johnson_cycles.1 knn.1 knn_w.1 kruskal.1 label_prop.1 \
largest_component.1 modularity.1 node_components.1 pm.1 power_law.1 \
shortest.1 shortest_avg_max_hist.1 strong_conn.1 ws.1

##dist_man_MANS = $(man_MANS)

