#include <iostream>
#include <vector>
#include <cmath>
#include <climits>

using namespace std;

// Function to detect currency arbitrage using Bellman-Ford
bool detectArbitrage(int n, vector<vector<double>>& exchangeRates) {
    
    // Convert exchange rates to negative logarithms
    vector<vector<double>> graph(n, vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                graph[i][j] = -log(exchangeRates[i][j]);
            } else {
                graph[i][j] = 0; // No cost to stay in the same currency
            }
        }
    }

    // Bellman-Ford algorithm
    vector<double> dist(n, INT_MAX);
    vector<int> parent(n, -1); // To store the path
    dist[0] = 0; // Start from an arbitrary source (e.g., currency 0)

    // Relax edges |V|-1 times
    for (int k = 0; k < n - 1; ++k) {
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                if (dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                    dist[v] = dist[u] + graph[u][v];
                    parent[v] = u; // Track the path
                }
            }
        }
        // Print distances after each relaxation
        cout << "Distances after relaxation " << k + 1 << ": ";
        for (double d : dist) {
            cout << (d == INT_MAX ? "INF" : to_string(d)) << " ";
        }
        cout << endl;
    }

    // Final distances after all relaxations
    cout << "Final distances after all relaxations: ";
    for (double d : dist) {
        cout << (d == INT_MAX ? "INF" : to_string(d)) << " ";
    }
    cout << endl;

    // Check for negative weight cycles
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            if (dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                // Negative cycle detected, trace the cycle
                vector<int> cycle;
                int current = v;
                for (int i = 0; i < n; ++i) {
                    current = parent[current];
                }
                int start = current;
                cycle.push_back(start);
                current = parent[start];
                while (current != start) {
                    cycle.push_back(current);
                    current = parent[current];
                }
                cycle.push_back(start); // Complete the cycle

                cout << "Arbitrage opportunity detected!" << endl;
                cout << "Cycle: ";
                for (int i = cycle.size() - 1; i >= 0; --i) {
                    cout << cycle[i] << (i == 0 ? "\n" : " -> ");
                }

                // Calculate potential profit
                double money = 1.0; // Start with $1
                for (size_t i = cycle.size() - 1; i > 0; --i) {
                    int from = cycle[i];
                    int to = cycle[i - 1];
                    money *= exchangeRates[from][to];
                }

                cout << "Starting with $1, you can end with $" << money << endl;
                return true;
            }
        }
    }

    cout << "No arbitrage opportunity." << endl;
    return false;
}

int main() {
    int n = 3; // Number of currencies

    // Exchange rates matrix (USD, EUR, GBP)
    vector<vector<double>> exchangeRates = {
        {1.0, 0.9, 0.8},  // USD to USD, EUR, GBP
        {1.1, 1.0, 0.7},  // EUR to USD, EUR, GBP
        {1.25, 1.4, 1.0}  // GBP to USD, EUR, GBP
    };

    detectArbitrage(n, exchangeRates);

    return 0;
}
