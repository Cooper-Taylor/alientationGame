#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <utility>
#include <set>


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
		unordered_map<int, list<int>> getAdjList() const {
			return adjList;
		}

		// Get adjacency list of specific vertex
		list<int> getAdjList(int vertexId) {
			if (vertexId >= 0 && vertexId < vertices.size()) {
				return adjList[vertexId];
			}
			// Return empty list if invalid
			return list<int>();
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
		std::vector<Vertex> vertices; // List of vertices
		std::unordered_map<int, std::list<int>> adjList; // Adjacency list for edges
};


namespace std {
	template <>
		struct hash<Graph> {
			size_t operator()(const Graph& g) const {
				size_t hashValue = 0;

				// Hash based on the number of vertices and the adjacency list (could add more sophisticated hashing)
				for (const auto& vertex : g.getVertices()) {
					hashValue ^= std::hash<int>()(vertex.id) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
					// Hash the color as well if needed
					hashValue ^= std::hash<int>()(vertex.color) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
				}

				for (const auto& adj : g.getAdjList()) {
					hashValue ^= std::hash<int>()(adj.first) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
					for (const auto& neighbor : adj.second) {
						hashValue ^= std::hash<int>()(neighbor) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
					}
				}

				return hashValue;
			}
		};
}




/*
	Function that returns if a graph G violates any rule in game
args : A graph, g
returns : false if graph violates game rule, else true
 */
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


// Implement graph generation!!!
class graphNode {
	public:

		graphNode(Graph g) {
			parentGraph = g;
			derivedList = {};
			colorLimit = 0;

			// Calculate color limit
			for (int vertex = 0; vertex < parentGraph.getNumVertices(); vertex++) {
				if (parentGraph.getColor(vertex) != -1) {
					colorLimit++;	
				}
			}
		}

		void addDerived(graphNode n) {
			derivedList.push_back(n);
		}

		int getColorLimit() {
			return colorLimit;
		}

		Graph getGraph() {
			return parentGraph;
		}

		list<graphNode> getDerived() {
			return derivedList;
		}

		void displayNode() {
			for (auto i : derivedList) {
				Graph g = i.getGraph();
				g.displayGraph();
				cout << endl;
				i.displayNode();
			}
		}

	private:
		list<graphNode> derivedList;
		Graph parentGraph;
		int colorLimit;
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

Graph minGraph, maxGraph;
int minColors = INT_MAX, maxColors = 0;

void generateDerivedGraphs(graphNode* node, unordered_map<Graph, bool> &knownGraphs) {
	// Get parent graph
	// Change node, check if this is already in knownGraphs
	// If not in known graphs, check
	// Add to derived graph list
	Graph baseGraph = node->getGraph();

	for (int vertex = 0; vertex < baseGraph.getNumVertices(); vertex++) {
		if (baseGraph.getColor(vertex) == -1) {
			for (int color = 0; color <= node->getColorLimit(); color++) {
				// Change vertex color
				Graph derivedGraph = baseGraph;
				derivedGraph.setColor(vertex, color);

				// check if Graph is already in list of known graphs


				// check if Graph is valid if not in knownGraphs
				if (knownGraphs.find(derivedGraph) == knownGraphs.end()) {
					bool v = isViable(derivedGraph);
					knownGraphs[derivedGraph] = v;

					// Recurse on derived graph
					if (v) {
						graphNode derivedNode(derivedGraph);
						generateDerivedGraphs(&derivedNode, knownGraphs);
						node->addDerived(derivedNode);	

						int colorCount = countDistinctColors(derivedGraph);

                        if (colorCount < minColors) {
                            minColors = colorCount;
                            minGraph = derivedGraph;
                        }
                        if (colorCount > maxColors) {
                            maxColors = colorCount;
                            maxGraph = derivedGraph;
                        }
					}	
				}
			}
		}
	}
}



int main() {
	// Create a graph with 5 vertices
	Graph g(5);

	// Add some edges to the graph
	g.addEdge(0, 1);
	g.addEdge(1, 2);
	g.addEdge(2, 3);
	g.addEdge(3, 4);

	// Display the graph
	g.displayGraph();

	// Is it viable?
	cout << ((isViable(g)) ? "yes" : "no") << endl;

	// Generate derived graphs
	graphNode rootNode(g);
	unordered_map<Graph, bool> knownGraphs;
	knownGraphs[g] = true;	

	generateDerivedGraphs(&rootNode, knownGraphs);
	
	cout << "Minimum Graph (Least Colors: " << minColors << "):\n";
    minGraph.displayGraph();

    cout << "Maximum Graph (Most Colors: " << maxColors << "):\n";
    maxGraph.displayGraph();
	
	// Maybe find a better way to display this
	rootNode.displayNode();

	return 0;
}

