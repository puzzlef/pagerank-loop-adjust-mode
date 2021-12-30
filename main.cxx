#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <utility>
#include "src/main.hxx"

using namespace std;




template <class G, class T>
void printRow(const G& x, const PagerankResult<T>& a, const PagerankResult<T>& b, const char *tec) {
  auto e = l1Norm(b.ranks, a.ranks);
  print(x); printf(" [%09.3f ms; %03d iters.] [%.4e err.] %s\n", b.time, b.iterations, e, tec);
}

void runPagerankBatch(const string& data, int repeat, int skip, int batch) {
  using T = float;
  using G = DiGraph<>;
  enum NormFunction { L0=0, L1=1, L2=2, Li=3 };
  vector<T> *init = nullptr;

  DiGraph<> xo;
  stringstream s(data);
  while (true) {
    // Skip some edges (to speed up execution)
    if (skip>0 && !readSnapTemporal(xo, s, skip)) break;
    auto x  = selfLoop(xo, [&](int u) { return isDeadEnd(xo, u); });
    auto xt = transposeWithDegree(x);
    auto ksOld = vertices(x);
    auto a0 = pagerankMonolithicSeq(x, xt, init, {repeat});
    auto Rx = a0.ranks;

    // Read edges for this batch.
    auto yo = copy(xo);
    if (!readSnapTemporal(yo, s, batch)) break;
    auto y  = selfLoop(yo, [&](int u) { return isDeadEnd(yo, u); });
    auto yt = transposeWithDegree(y);
    auto ks = vertices(y);
    vector<T> ry(y.span());
    int X = ksOld.size();
    int Y = ks.size();

    // INSERTIONS:
    // Adjust ranks for insertions.
    adjustRanks(ry, Rx, ksOld, ks, 0.0f, float(X)/(Y+1), 1.0f/(Y+1));

    // Find pagerank using L1-norm for convergence check.
    auto b0 = pagerankMonolithicSeq(y, yt, init, {repeat, L1});
    printRow(y, b0, b0, "I:pagerankMonolithicSeqL1Norm (static)");
    auto c0 = pagerankMonolithicSeq(y, yt, &ry, {repeat, L1});
    printRow(y, b0, c0, "I:pagerankMonolithicSeqL1Norm (incremental)");
    auto d0 = pagerankMonolithicSeqDynamic(x, xt, y, yt, &ry, {repeat, L1});
    printRow(y, b0, d0, "I:pagerankMonolithicSeqL1Norm (dynamic)");

    // Find pagerank using L2-norm for convergence check.
    auto e0 = pagerankMonolithicSeq(y, yt, init, {repeat, L2});
    printRow(y, b0, e0, "I:pagerankMonolithicSeqL2Norm (static)");
    auto f0 = pagerankMonolithicSeq(y, yt, &ry, {repeat, L2});
    printRow(y, b0, f0, "I:pagerankMonolithicSeqL2Norm (incremental)");
    auto g0 = pagerankMonolithicSeqDynamic(x, xt, y, yt, &ry, {repeat, L2});
    printRow(y, b0, g0, "I:pagerankMonolithicSeqL2Norm (dynamic)");

    // Find pagerank using Li-norm for convergence check.
    auto h0 = pagerankMonolithicSeq(y, yt, init, {repeat, Li});
    printRow(y, b0, h0, "I:pagerankMonolithicSeqLiNorm (static)");
    auto i0 = pagerankMonolithicSeq(y, yt, &ry, {repeat, Li});
    printRow(y, b0, i0, "I:pagerankMonolithicSeqLiNorm (incremental)");
    auto j0 = pagerankMonolithicSeqDynamic(x, xt, y, yt, &ry, {repeat, Li});
    printRow(y, b0, j0, "I:pagerankMonolithicSeqLiNorm (dynamic)");

    // DELETIONS:
    // Adjust ranks for deletions.
    auto Ry = b0.ranks;
    vector<T> rx(x.span());
    adjustRanks(rx, Ry, ks, ksOld, 0.0f, float(Y)/(X+1), 1.0f/(X+1));

    // Find pagerank using L1-norm for convergence check.
    auto b1 = pagerankMonolithicSeq(x, xt, init, {repeat, L1});
    printRow(x, b1, b1, "D:pagerankMonolithicSeqL1Norm (static)");
    auto c1 = pagerankMonolithicSeq(x, xt, &rx, {repeat, L1});
    printRow(x, b1, c1, "D:pagerankMonolithicSeqL1Norm (incremental)");
    auto d1 = pagerankMonolithicSeqDynamic(y, yt, x, xt, &rx, {repeat, L1});
    printRow(x, b1, d1, "D:pagerankMonolithicSeqL1Norm (dynamic)");

    // Find pagerank using L2-norm for convergence check.
    auto e1 = pagerankMonolithicSeq(x, xt, init, {repeat, L2});
    printRow(x, b1, e1, "D:pagerankMonolithicSeqL2Norm (static)");
    auto f1 = pagerankMonolithicSeq(x, xt, &rx, {repeat, L2});
    printRow(x, b1, f1, "D:pagerankMonolithicSeqL2Norm (incremental)");
    auto g1 = pagerankMonolithicSeqDynamic(y, yt, x, xt, &rx, {repeat, L2});
    printRow(x, b1, g1, "D:pagerankMonolithicSeqL2Norm (dynamic)");

    // Find pagerank using Li-norm for convergence check.
    auto h1 = pagerankMonolithicSeq(x, xt, init, {repeat, Li});
    printRow(x, b1, h1, "D:pagerankMonolithicSeqLiNorm (static)");
    auto i1 = pagerankMonolithicSeq(x, xt, &rx, {repeat, Li});
    printRow(x, b1, i1, "D:pagerankMonolithicSeqLiNorm (incremental)");
    auto j1 = pagerankMonolithicSeqDynamic(y, yt, x, xt, &rx, {repeat, Li});
    printRow(x, b1, j1, "D:pagerankMonolithicSeqLiNorm (dynamic)");

    // New graph is now old.
    xo = move(yo);
  }
}


void runPagerank(const string& data, int repeat) {
  int M = countLines(data), steps = 10;
  printf("Temporal edges: %d\n", M);
  for (int batch=10, i=0; batch<M; batch*=i&1? 2:5, i++) {
    int skip = max(M/steps - batch, 0);
    printf("\n# Batch size %.0e\n", (double) batch);
    runPagerankBatch(data, repeat, skip, batch);
  }
}


int main(int argc, char **argv) {
  char *file = argv[1];
  int repeat = argc>2? stoi(argv[2]) : 5;
  printf("Using graph %s ...\n", file);
  string d = readFile(file);
  runPagerank(d, repeat);
    printf("\n");
  return 0;
}
