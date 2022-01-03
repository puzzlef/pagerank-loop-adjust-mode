Comparing the performance of **static**, **incremental**, and **dynamic**
PageRank; with loop-based dead end handling strategy ([pull], [CSR]).

`TODO after report!`

This experiment was for comparing the performance between the three modes of
PageRank computation: **static**, **incremental**, and **dynamic**. Loop-based
dead end handling strategy is used as a representative of non-teleport strategy.
This means self-loops were added to all dead ends (vertices with no outgoing
edges). This experiment was done on *temporal graphs* for *insertions* and
*deletions*, with *L1*, *L2*, and *L∞-norm* as the error measurement function
for convergence check.

The PageRank algorithm used here is the *standard power-iteration (pull)* based
PageRank. The rank of a vertex in an iteration is calculated as `c₀ + αΣrₙ/dₙ`,
where `c₀` is the *common* *teleport contribution*, `α` is the *damping factor*,
`rₙ` is the *previous rank of vertex* with an incoming edge, `dₙ` is the
*out-degree* of the incoming-edge vertex, and `N` is the *total number of
vertices* in the graph. The *common teleport contribution* `c₀`, calculated as
`(1-α)/N` , which only includes the *contribution due to a teleport from* *any
vertex* in the graph due to the damping factor `(1-α)/N`, as there are no
*dangling vertices* (dead ends). A *damping factor* of `α = 0.85`, and a
*tolerance* of  `τ = 10⁻⁶` was used.

It was observed that when using **L1-norm** for convergence check, the
**GM-RATIO** between *static*, *incremental*, and *dynamic* PageRank is
**1.00:0.75:0.58** for insertions, and **1.00:0.81:0.63** for deletions. For a
batch size of *1E+3*, the *GM-RATIO* is *1.00:0.65:0.52* for insertions, and
*1.00:0.69:0.55* for deletions. With respect to *GM-RATIO*, **incremental**
PageRank is observed to perform better than the *static* mode upto a batch size
of `1E+4` for insertions and deletions, while **dynamic** PageRank performs
better than the static mode upto a batch size of `5E+5` for insertions and
`1E+5` for deletions. Similarly, the **AM-RATIO** for insertions is
**1.00:0.84:0.75**, and for deletions is **1.00:0.94:0.84**. For a batch size of
*1E+3*, the *AM-RATIO* is *1.00:0.25:0.21* for insertions, and *1.00:0.29:0.25*
for deletions. With respect to *AM-RATIO*, **incremental** PageRank is observed
to perform better than the *static* mode upto a batch size of `1E+6` for
insertions and `5E+5` for deletions, while **dynamic** PageRank always
outperforms the static mode for insertions but only upto a batch size of `1E+6`
for deletions.

When using **L2-norm** for convergence check, the **GM-RATIO** between *static*,
*incremental*, and *dynamic* PageRank is **1.00:0.58:0.46** for insertions, and
**1.00:0.60:0.48** for deletions. For a batch size of *1E+3*, the *GM-RATIO* is
*1.00:0.50:0.40* for insertions, and *1.00:0.47:0.38* for deletions. With
respect to *GM-RATIO*, **incremental** PageRank is observed to perform better
than the *static* mode upto a batch size of `1E+6` for insertions and `5E+5` for
deletions, while **dynamic** PageRank always outperforms the static mode for
insertions but only upto a batch size of `1E+6` for deletions. Similarly, the
**AM-RATIO** for insertions is **1.00:0.72:0.63**, and for deletions is
**1.00:0.73:0.65**. For a batch size of *1E+3*, the *AM-RATIO* is
*1.00:0.18:0.15* for insertions, and *1.00:0.12:0.09* for deletions. With
respect to *AM-RATIO*, **incremental** PageRank always outperforms the static
mode for insertions but only upto a batch size of `1E+6` for deletions, while
**dynamic** PageRank always outperforms the static mode for insertions as well
as deletions.

When using **L∞-norm** for convergence check, the **GM-RATIO** between *static*,
*incremental*, and *dynamic* PageRank is **1.00:0.48:0.39** for insertions, and
**1.00:0.49:0.40** for deletions. For a batch size of *1E+3*, the *GM-RATIO* is
*1.00:0.49:0.40* for insertions, and *1.00:0.44:0.35* for deletions. With
respect to *GM-RATIO*, **incremental** and **dynamic** PageRank always
outperform the static mode for insertions as well as deletions. Similarly, the
**AM-RATIO** for insertions is **1.00:0.50:0.45**, and for deletions is
**1.00:0.50:0.46**. For a batch size of *1E+3*, the *AM-RATIO* is
*1.00:0.17:0.14* for insertions, and *1.00:0.11:0.09* for deletions. With
respect to *AM-RATIO*, **incremental** and **dynamic** PageRank again always
outperform the static mode for both insertions and deletions.

From the results it can be said that performance improvement provided by
**incremental**/**dynamic** PageRank computation is higher when using
**L∞-norm** for convergence check, when compared to **L2-norm** or **L1-norm**.
However, what can be said about **incremental** vs **dynamic** PageRank? When
does **incremental**/**dynamic** PageRank stop being better than the **static**
mode? How can this be explained? It should be noted that **L∞-norm** itself
converges much faster than **L2-norm** or **L1-norm**.

All outputs are saved in [out](out/) and a small part of the output is listed
here. Some [charts] are also included below, generated from [sheets]. The input
data used for this experiment is available at ["graphs"] (for small ones), and
the [SuiteSparse Matrix Collection]. This experiment was done with guidance
from [Prof. Dip Sankar Banerjee] and [Prof. Kishore Kothapalli].

<br>

```bash
$ g++ -std=c++17 -O3 main.cxx
$ ./a.out ~/data/CollegeMsg.txt
$ ./a.out ~/data/email-Eu-core-temporal.txt
$ ...

# ...
#
# Using graph /home/subhajit/data/sx-stackoverflow.txt ...
# Temporal edges: 63497051
#
# # Batch size 1e+01
# order: 747591 size: 6102013 {} [00527.108 ms; 029 iters.] [0.0000e+00 err.] I:pagerankMonolithicSeqL1Norm (static)
# order: 747591 size: 6102013 {} [00019.174 ms; 001 iters.] [1.6814e-06 err.] I:pagerankMonolithicSeqL1Norm (incremental)
# order: 747591 size: 6102013 {} [00016.747 ms; 001 iters.] [1.7174e-06 err.] I:pagerankMonolithicSeqL1Norm (dynamic)
# order: 747591 size: 6102013 {} [00312.101 ms; 017 iters.] [1.9536e-04 err.] I:pagerankMonolithicSeqL2Norm (static)
# order: 747591 size: 6102013 {} [00023.302 ms; 001 iters.] [1.6814e-06 err.] I:pagerankMonolithicSeqL2Norm (incremental)
# order: 747591 size: 6102013 {} [00020.400 ms; 001 iters.] [1.7174e-06 err.] I:pagerankMonolithicSeqL2Norm (dynamic)
# order: 747591 size: 6102013 {} [00254.736 ms; 012 iters.] [1.9268e-03 err.] I:pagerankMonolithicSeqLiNorm (static)
# order: 747591 size: 6102013 {} [00022.422 ms; 001 iters.] [1.6814e-06 err.] I:pagerankMonolithicSeqLiNorm (incremental)
# order: 747591 size: 6102013 {} [00019.032 ms; 001 iters.] [1.7174e-06 err.] I:pagerankMonolithicSeqLiNorm (dynamic)
# order: 747590 size: 6102004 {} [00549.579 ms; 029 iters.] [0.0000e+00 err.] D:pagerankMonolithicSeqL1Norm (static)
# order: 747590 size: 6102004 {} [00020.002 ms; 001 iters.] [1.6940e-06 err.] D:pagerankMonolithicSeqL1Norm (incremental)
# order: 747590 size: 6102004 {} [00017.212 ms; 001 iters.] [1.7295e-06 err.] D:pagerankMonolithicSeqL1Norm (dynamic)
# order: 747590 size: 6102004 {} [00321.375 ms; 017 iters.] [1.9535e-04 err.] D:pagerankMonolithicSeqL2Norm (static)
# order: 747590 size: 6102004 {} [00020.212 ms; 001 iters.] [1.6940e-06 err.] D:pagerankMonolithicSeqL2Norm (incremental)
# order: 747590 size: 6102004 {} [00017.232 ms; 001 iters.] [1.7295e-06 err.] D:pagerankMonolithicSeqL2Norm (dynamic)
# order: 747590 size: 6102004 {} [00228.498 ms; 012 iters.] [1.9268e-03 err.] D:pagerankMonolithicSeqLiNorm (static)
# order: 747590 size: 6102004 {} [00019.995 ms; 001 iters.] [1.6940e-06 err.] D:pagerankMonolithicSeqLiNorm (incremental)
# order: 747590 size: 6102004 {} [00017.214 ms; 001 iters.] [1.7295e-06 err.] D:pagerankMonolithicSeqLiNorm (dynamic)
# order: 1652959 size: 12339389 {} [01324.076 ms; 029 iters.] [0.0000e+00 err.] I:pagerankMonolithicSeqL1Norm (static)
# ...
```

[![](https://i.imgur.com/xWhEkfa.png)][sheetp]
[![](https://i.imgur.com/56fjSID.png)][sheetp]
[![](https://i.imgur.com/bPc7h5U.png)][sheetp]
[![](https://i.imgur.com/7bNooAH.png)][sheetp]

[![](https://i.imgur.com/1CmuvCq.png)][sheetp]
[![](https://i.imgur.com/TfQRyzD.png)][sheetp]
[![](https://i.imgur.com/MaQcs6F.png)][sheetp]
[![](https://i.imgur.com/39lAnny.png)][sheetp]

[![](https://i.imgur.com/1k3rcO2.gif)][sheetp]
[![](https://i.imgur.com/XA8R7pe.gif)][sheetp]
[![](https://i.imgur.com/JXkqned.gif)][sheetp]
[![](https://i.imgur.com/xfjYTPM.gif)][sheetp]

[![](https://i.imgur.com/8hXpeAd.gif)][sheetp]
[![](https://i.imgur.com/CbKCpbO.gif)][sheetp]
[![](https://i.imgur.com/MSkf759.gif)][sheetp]
[![](https://i.imgur.com/KlvWAiB.gif)][sheetp]

<br>
<br>


## References

- [How to check for Page Rank convergence?][L∞ norm]
- [L0 Norm, L1 Norm, L2 Norm & L-Infinity Norm](https://montjoile.medium.com/l0-norm-l1-norm-l2-norm-l-infinity-norm-7a7d18a4f40c)
- [PageRank Algorithm, Mining massive Datasets (CS246), Stanford University](https://www.youtube.com/watch?v=ke9g8hB0MEo)
- [Weighted Geometric Mean Selected for SPECviewperf® Composite Numbers](https://www.spec.org/gwpg/gpc.static/geometric.html)
- [SuiteSparse Matrix Collection]

<br>
<br>

[![](https://i.imgur.com/BnCiig7.jpg)](https://www.youtube.com/watch?v=04Uv44DRJAU)
[![DOI](https://zenodo.org/badge/372463776.svg)](https://zenodo.org/badge/latestdoi/372463776)

[Prof. Dip Sankar Banerjee]: https://sites.google.com/site/dipsankarban/
[Prof. Kishore Kothapalli]: https://cstar.iiit.ac.in/~kkishore/
[SuiteSparse Matrix Collection]: https://suitesparse-collection-website.herokuapp.com
["graphs"]: https://github.com/puzzlef/graphs
[nvGraph]: https://github.com/rapidsai/nvgraph
[pull]: https://github.com/puzzlef/pagerank-push-vs-pull
[CSR]: https://github.com/puzzlef/pagerank-class-vs-csr
[L1 norm]: https://github.com/rapidsai/nvgraph/blob/main/cpp/src/pagerank.cu#L154
[L2 norm]: https://github.com/rapidsai/nvgraph/blob/main/cpp/src/pagerank.cu#L149
[L∞ norm]: https://stackoverflow.com/a/29321153/1413259
[charts]: https://photos.app.goo.gl/XpQ4WnfixF2Tb1VS6
[sheets]: https://docs.google.com/spreadsheets/d/1Nbb3_znctUdgc-eSWWaqP0g7vZUUeS4I5uV0YNAru4s/edit?usp=sharing
[sheetp]: https://docs.google.com/spreadsheets/d/e/2PACX-1vQaPHkI4YwNqeZCvfkTvz_w6oJwHUp9CsevAfUF8xvBHupgQVv01RFIL7XI7qCQ6OisIA1sqvKllRB8/pubhtml
