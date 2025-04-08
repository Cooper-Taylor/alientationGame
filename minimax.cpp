#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <utility>
#include <set>
#include <climits>


/*
	GPT-Generated base Graph class for convenience
 */

using namespace std;


class Graph {
	public:
		// Struct to represent a vertex with a color attribute
		struct Vertex {
			int id;
			int color; // -1 means uncolored
			Vertex(int id) : id(id), color(-1) {}
		};

		// Default constructor
		Graph() {}

		// Copy constructor
		Graph(const Graph& other) {
			// Copy the vertices vector
			vertices = other.vertices;

			// Copy the adjacency list
			adjList = other.adjList;
		}


		// Define the equality operator
		bool operator==(const Graph& other) const {
			// Compare vertices and adjacency lists (both should be equal for the graphs to be equal)
			if (vertices.size() != other.vertices.size()) return false;

			// Compare adjacency lists (assuming both have the same structure)
			for (const auto& vertex : vertices) {
				if (adjList.at(vertex.id) != other.adjList.at(vertex.id)) {
					return false;
				}
			}
			return true;
		}




		// Constructor to create a graph with a given number of vertices
		Graph(int numVertices) {
			for (int i = 0; i < numVertices; ++i) {
				vertices.push_back(Vertex(i));
				adjList.push_back({});
			}
		}

		// Add an edge between two vertices
		void addEdge(int v1, int v2) {
			// Ensure vertices exist
			if (v1 >= 0 && v1 < vertices.size() && v2 >= 0 && v2 < vertices.size()) {
				adjList[v1].push_back(v2);
				adjList[v2].push_back(v1); // undirected edge
			} else {
				std::cout << "Invalid vertex ids!" << std::endl;
			}
		}

		// Set the color of a specific vertex
		void setColor(int vertexId, int color) {
			if (vertexId >= 0 && vertexId < vertices.size()) {
				vertices[vertexId].color = color;
			} else {
				std::cout << "Invalid vertex id!" << std::endl;
			}
		}

		// Get the color of a specific vertex
		int getColor(int vertexId) const {
			if (vertexId >= 0 && vertexId < vertices.size()) {
				return vertices[vertexId].color;
			}
			return -1; // Return -1 if the vertex is invalid
		}

		// Generic adjacency list get
		vector<vector<int>> getAdjList() const {
			return adjList;
		}

		// Get adjacency list of specific vertex
		vector<int> getAdjList(int vertexId) {
			if (vertexId >= 0 && vertexId < vertices.size()) {
				return adjList[vertexId];
			}
			// Return empty list if invalid
			return vector<int>();
		}

		// Get number of vertices
		int getNumVertices() const {
			return vertices.size();
		}

		vector<Vertex> getVertices() const {
			return vertices;
		}

		// Display the graph
		void displayGraph() const {
			for (const auto& vertex : vertices) {
				std::cout << "Vertex " << vertex.id << " (Color: " << vertex.color << ") is connected to: ";
				for (int neighbor : adjList.at(vertex.id)) {
					std::cout << neighbor << " ";
				}
				std::cout << std::endl;
			}
		}

	private:
		vector<Vertex> vertices; // List of vertices
		vector<vector<int>> adjList; // Adjacency list for edges
};

namespace std {
	template <typename T>
		struct hash<std::vector<T>> {
			std::size_t operator()(const std::vector<T>& vec) const {
				std::size_t seed = vec.size();
				for(auto x : vec) {
					x = ((x >> 16) ^ x) * 0x45d9f3b;
					x = ((x >> 16) ^ x) * 0x45d9f3b;
					x = (x >> 16) ^ x;
					seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
				}
				return seed;
			}
		};
}


bool isViable(Graph g) {
	// For each node, if node is not colored
	// check adjacency list to ensure that no other color exists

	for (int i = 0; i < g.getNumVertices(); i++) {
		if (g.getColor(i) != -1) {
			for (auto adj : g.getAdjList(i)) {
				if (g.getColor(i) != g.getColor(adj) && g.getColor(adj) != -1)
					return false;
			}
		}
	}

	return true;
}




class Color{
	public:
		vector<int> coloring = {};
		vector<Color> derivedColorings = {{}};
};


bool isViable(Graph g, Color c) {
	for (int i = 0; i < c.coloring.size(); i++) {
		if (c.coloring[i] != -1) {
			for (auto adj : g.getAdjList(i)) {
				if (c.coloring[i] != c.coloring[adj] && c.coloring[adj] != -1)
					return false;
			}
		}
	}

	return true;
};


int countDistinctColors(Graph g) {
	set<int> colorSet;
	for (auto v : g.getVertices()) {
		if (v.color != -1) {
			colorSet.insert(v.color);
		}
	}
	return colorSet.size();
}


int countDistinctColors(Color c) {
	set<int> colorSet;
	for (auto v : c.coloring) {
		if (v != -1) {
			colorSet.insert(v);
		}
	}
	return colorSet.size();
}

int countPlacements(Color c) {
	int count = 0;
	for (auto c : c.coloring) {
		if (c != -1) count++;
	}
	return count;
}

int countPlacements(Graph g) {
	int count = 0;
	for (auto v : g.getVertices()) {
		if (v.color != -1) {
			count++;
		}
	}
	return count;
}

Graph minGraph, maxGraph;
int minColors = INT_MAX;
int maxColors = 0;

Graph minPlacementGraph, maxPlacementGraph;
int minPlacements = INT_MAX;
int maxPlacements = 0;



// Translate into using just colorings
void generateDerivedGraphs(Graph &baseGraph, Color* currColor, unordered_map<vector<int>, bool>& knownGraphs) {

	// Start with empty graph
	// Color some node i
	// Recurse until all colored

	for (int v = 0; v < currColor->coloring.size(); v++) {
		if (currColor->coloring[v] == -1) {
			for (int c = 0; c < countDistinctColors(*currColor) + 1; c++) {
				Color newColor;
				newColor.coloring = currColor->coloring;
				int exchangedColor = newColor.coloring[v];

				newColor.coloring[v] = c;

				// Is it proper?
				if (knownGraphs.find(newColor.coloring) == knownGraphs.end()) {
					bool isValid = isViable(baseGraph, newColor);
					knownGraphs[newColor.coloring] = isValid;
				}

				// If valid
				if (knownGraphs[newColor.coloring]) {
					generateDerivedGraphs(baseGraph, &newColor, knownGraphs);
					currColor->derivedColorings.push_back(newColor);
				}
			}
		}	
	}
}

void displayDerivedColorings(Color* c) {
	for (int i : c->coloring)
		cout << i << " ";
	cout << endl;
	for (auto derived : c->derivedColorings)
		displayDerivedColorings(&derived);
}


int minimax(Graph& baseGraph, bool isMax, Color *currColor) {
	int best = isMax ? 0 : INT_MAX;

	// Already precomputed the colorings, so just run down them
	for (auto derived : currColor->derivedColorings) {
		// Recurse here
		int currPossible = minimax(baseGraph, !isMax, &derived);

		if ( (isMax && currPossible > best) || (!isMax && currPossible < best) )
			best = currPossible;
	}

	// Check if node has no derived colorings as base case
	if ( currColor->derivedColorings.size() != 0)
		return best;
	else {
		for (auto color : currColor->coloring)
			cout << color;
		cout << endl;
		return countPlacements(*currColor);
	}
}


int main() {
	// Create a graph with 5 vertices
	Graph g(4);


	// Add some edges to the graph
	g.addEdge(0,1);
	g.addEdge(1,2);
	g.addEdge(2,3);
	g.addEdge(3,0);

	// Display the graph
	g.displayGraph();

	// Is it viable?
	cout << ((isViable(g)) ? "yes" : "no") << endl << endl;

	unordered_map<vector<int>, bool> knownGraphs;
	Color baseColoring;

	for (int i = 0; i < g.getNumVertices(); i++)
		baseColoring.coloring.push_back(-1);

	knownGraphs[baseColoring.coloring] = true;	


	generateDerivedGraphs(g, &baseColoring, knownGraphs);



	/*
		generateDerivedGraphs(&rootNode, knownGraphs);

		cout << "Minimum Color Graph (Least Colors: " << minColors << "):\n";
		minGraph.displayGraph();
		cout << endl;

		cout << "Maximum Ccolor Graph (Most Colors: " << maxColors << "):\n";
		maxGraph.displayGraph();
		cout << endl;

		cout << "Minimum Placement Graph (not fully filled) (Least Placements: " << minPlacements << "):\n";
		minPlacementGraph.displayGraph();
		cout << endl;

		cout << "Maximum Placements Graph (Most Placements: " << maxPlacements << "):\n";
		maxPlacementGraph.displayGraph();
		cout << endl;

	*/

	cout << minimax(g, false, &baseColoring);
	return 0;
}

