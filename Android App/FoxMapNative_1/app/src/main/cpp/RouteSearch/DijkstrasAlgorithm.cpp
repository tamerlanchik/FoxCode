#include "pch.h"
#include "DijkstrasAlgorithm.h"

/*
void BFS(const IGraph& graph, int vertex, void(*visit)(int)) {
	std::vector<bool> visited(graph.VerticesCount(), false);
	// Тут храним текуший фронт - ?
	std::queue<int> qu;
	// Кладем вершину, с которой наиснаем обход
	qu.push(vertex);
	visited[vertex] = true;

	while (qu.size() != 0) {
		int current = qu.front();
		qu.pop();
		visit(current);
		std::vector<int> adjacentVertices = graph.GetNextVertices(current);
		for (int i = 0; i < adjacentVertices.size(); ++i) {
			if (!visited[adjacentVertices[i]]) {
				qu.push(adjacentVertices[i]);
				visited[adjacentVertices[i]] = true;
			}
		}
	}
}

std::vector<int> r, pi;
void bfs(int s) {
	r.assign(n, INT_MAX);
	std::queue<int> q;
	q.push(s); r[s] = 0; pi[s] = -1;
	while (!q.empty()) {
		v = q.front(); q.pop();
		for (w : (v, w) ∈ E)
			if (r[w] > r[v] + 1) {
				r[w] = r[v] + 1;
				pi[w] = v;
				q.push(w);
			}

	}
}

bool Relax(u, v) {
	if (d[v] > d[u] + w(u, v)) {
		d[v] = d[u] + w(u, v);
		pi[v] = u;
		return true;
	}
	return false;
}

void Dijkstra(G, s) {
	pi[V] = -1;
	d[V] = INT_MAX;
	d[s] = 0;
	priority_queue<int> q; q.push(s);
	while (!q.empty()) {
		u = q.top(); q.pop();
		for ((u, v) : ребра из u) {
			if (d[v] == INT_MAX) {
				d[v] = d[u] + w(u, v);
				pi[v] = u;
				q.push(v);
			}
			else if (Relax(u, v)) {
				q.DecreaseKey(v, d[v]);

			}
		}
	}
}
*/
template <class T>
DijkstrasAlgorithm<T>::DijkstrasAlgorithm(const int start, const int end, const T**const matrix, const int matrixSize){

}

template <class T>
DijkstrasAlgorithm<T>::~DijkstrasAlgorithm(){

}
