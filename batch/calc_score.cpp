#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include <queue>
using namespace std;

// cf. https://github.com/spaghetti-source/algorithm/blob/9cca6b826f19ed7e42dd326a4fbbb9f4d34f04d3/graph/maximum_flow_dinic.cc
template<typename flow_type>
struct dinic {
	struct edge {
		int src, dst;
		flow_type capacity, flow;
		size_t rev;
	};
	int n;
	vector<vector<edge>> adj;
	const flow_type EPS, INF;
	dinic(int n, flow_type EPS, flow_type INF) : n(n), adj(n), EPS(EPS), INF(INF) { }
	void add_edge(int src, int dst, flow_type capacity) {
		adj[src].push_back({src, dst, capacity, 0, adj[dst].size()});
		adj[dst].push_back({dst, src, 0, 0, adj[src].size()-1});
	}
	void add_both_edge(int src, int dst, flow_type capacity) {
		adj[src].push_back({src, dst, capacity, 0, adj[dst].size()});
		adj[dst].push_back({dst, src, capacity, 0, adj[src].size()-1});
	}
	flow_type max_flow(int s, int t) {
		vector<int> level(n), iter(n);
		function<int(void)> levelize = [&]() { // foward levelize
			level.assign(n, -1); level[s] = 0;
			queue<int> Q; Q.push(s);
			while (!Q.empty()) {
				int u = Q.front(); Q.pop();
				if (u == t) break;
				for (auto &e: adj[u]) {
					if (e.capacity - e.flow > EPS && level[e.dst] < 0) {
						Q.push(e.dst);
						level[e.dst] = level[u] + 1;
					}
				}
			}
			return level[t];
		};
		function<flow_type(int, flow_type)> augment = [&](int u, flow_type cur) {
			if (u == t) return cur;
			for (int &i = iter[u]; i < adj[u].size(); ++i) {
				edge &e = adj[u][i], &r = adj[e.dst][e.rev];
				if (e.capacity - e.flow > EPS && level[u] < level[e.dst]) {
					flow_type f = augment(e.dst, min(cur, e.capacity - e.flow));
					if (f > 0) {
						e.flow += f;
						r.flow -= f;
						return f;
					}
				}
			}
			return flow_type(0);
		};
		for (int u = 0; u < n; ++u) // initialize
			for (auto &e: adj[u]) e.flow = 0;

		flow_type flow = 0;
		while (levelize() >= 0) {
			fill(iter.begin(), iter.end(), 0);
			for (flow_type f; (f = augment(s, INF)) > 0; )
				flow += f;
		}
		return flow;
	}
};

int main(int argc, char** argv) {
	const int V = 400;
	const int E = 760;
	const double EPS = 1e-9;
	const double INF = 1e+15;

	vector<pair<int,int>> edges;
	for (int i = 0; ; ++ i) {
		for (int j = 0; j < 19; ++ j) edges.push_back(make_pair(i*20+j, i*20+j+1));
		if (i == 19) break;
		for (int j = 0; j < 20; ++ j) edges.push_back(make_pair(i*20+j, i*20+j+20));
	}
	if (edges.size() != E) throw 1;
	// for (auto p : edges) cout << p.first << " " << p.second << endl;

	ifstream fin(argv[1]);
	int n1, n2;
	fin >> n1 >> n2;
	vector<int> s(n1), t(n2);
	vector<double> capacities(E);
	for (int i = 0; i < n1; ++ i) fin >> s[i];
	for (int i = 0; i < n2; ++ i) fin >> t[i];
	for (int i = 0; i < E; ++ i) fin >> capacities[i];

	int n;
	while (cin >> n) {
		dinic<double> g(V+2, EPS, INF);
		for (int x : s) g.add_edge(V, x, INF);
		for (int x : t) g.add_edge(x, V+1, INF);
		for (int i = 0; i < n; ++ i) {
			int idx;
			double cnt;
			cin >> idx >> cnt;
			int u = edges[idx].first;
			int v = edges[idx].second;
			double cap = capacities[idx] / cnt;
			g.add_both_edge(u, v, cap);
		}
		printf("%.15f\n", g.max_flow(V, V+1));
		fflush(stdout);
	}
}
