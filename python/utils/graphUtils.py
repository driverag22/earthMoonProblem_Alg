import networkx as nx


def isPlanar(G):
    is_planar, certificate = nx.check_planarity(G, counterexample=True)
    if not is_planar:
        # For partition 0 (where x[e] == 0), add constraints:
        #   - sum_{e in cert0} x[e] >= 1
        #     (at least one edge must be in partition 1)
        #   - sum_{e in cert0} x[e] <= |cert0| - 1
        #     (at least one edge must be in partition 0)
        kuratowski_edges = [tuple(sorted(e)) for e in certificate.edges()]
        return is_planar, kuratowski_edges
    return is_planar, None
