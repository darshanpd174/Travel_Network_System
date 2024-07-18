#include <bits/stdc++.h>

using namespace std;

class Location {
public:
    string name;
    unordered_map<string, int> neighbors; // Adjacency list of neighbors and corresponding travel times
};

// Graph class representing the travel network
class TravelNetwork {
public:
    unordered_map<string, Location> locations;

    void addLocation(const string& locationName) {

        
            Location location;
            location.name = locationName;
            locations[locationName] = location;
        
    }

    void addConnection(const string& loc1, const string& loc2, int travelTime) {
        locations[loc1].neighbors[loc2] = travelTime;
        locations[loc2].neighbors[loc1] = travelTime;
    }

    // Find the shortest travel route between two locations using Dijkstra's algorithm
    vector<string> findShortestRoute(const string& startLocation, const string& endLocation) {
        unordered_map<string, string> parent;
        unordered_map<string, int> distance;
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

        for (auto& loc : locations) {
            distance[loc.first] = INT_MAX;
        }
        distance[startLocation] = 0;
        pq.push(make_pair(0, startLocation));

        while (!pq.empty()) {
            string currentLocation = pq.top().second;
            pq.pop();

            for (const auto& neighbor : locations[currentLocation].neighbors) {
                string neighborLocation = neighbor.first;
                int travelTime = neighbor.second;
                int newDistance = distance[currentLocation] + travelTime;

                if (newDistance < distance[neighborLocation]) {
                    distance[neighborLocation] = newDistance;
                    parent[neighborLocation] = currentLocation;
                    pq.push(make_pair(newDistance, neighborLocation));
                }
            }
        }

        vector<string> shortestRoute;
        string loc = endLocation;
        while (loc != startLocation) {
            shortestRoute.push_back(loc);
            loc = parent[loc];
        }
        shortestRoute.push_back(startLocation);
        reverse(shortestRoute.begin(), shortestRoute.end());

        return shortestRoute;
    }

    // Find the minimum spanning tree of the travel network using Kruskal's algorithm
    vector<vector<string>> findMinimumTourPackage() {
        vector<vector<string>> minimumTourPackage;
        vector<pair<int, pair<string, string>>> edges;

        for (const auto& loc : locations) {
            string loc1 = loc.first;
            for (const auto& neighbor : loc.second.neighbors) {
                string loc2 = neighbor.first;
                int travelTime = neighbor.second;
                edges.push_back(make_pair(travelTime, make_pair(loc1, loc2)));
            }
        }

        sort(edges.begin(), edges.end());
        unordered_map<string, string> parent;

        for (const auto& loc : locations) {
            parent[loc.first] = loc.first;
        }

        int numEdges = 0;
        int mini=0;
        int numLocations = locations.size();
        for (const auto& edge : edges) {
            string loc1 = edge.second.first;
            string loc2 = edge.second.second;

            int wt =edge.first;

            string parent1 = findParent(parent, loc1);
            string parent2 = findParent(parent, loc2);

            if (parent1 != parent2) {
                minimumTourPackage.push_back({loc1, loc2});
                numEdges++;
                parent[parent1] = parent2;
                mini+=wt;
                if (numEdges == numLocations - 1) {
                    break;
                }
            }
        }

        cout << "\nMinimum time required is : " << mini << endl;
        

        return minimumTourPackage;
    }

    void displayTravelNetwork() {
        for (const auto& loc : locations) {
            cout << "Location: " << loc.first << endl;
            cout << "Neighbors: ";
            for (const auto& neighbor : loc.second.neighbors) {
                cout << neighbor.first << "(" << neighbor.second << ") ";
            }
            cout << endl;
        }
    }

    bool findplace(string& place){
        for(const auto& loc : locations){
            if(loc.first == place){
                return true;
            }
        }
        return false;
    }

private:
    string findParent(unordered_map<string, string>& parent, const string& loc) {
        if (parent[loc] != loc) {
            parent[loc] = findParent(parent, parent[loc]);
        }
        return parent[loc];
    }
};

//writing function for case4
void case4(TravelNetwork& travelNetwork){
    string place;
    cout << "Give the name of place: ";                
    cin >> place;

    if(travelNetwork.findplace(place)){
        cout << " Location already exist" << endl;
    }
    else{
        travelNetwork.addLocation(place);
        cout << "Add the neighbouring places: " << endl;
            while(true){
                string temp;
                cin >> temp;
                if(travelNetwork.findplace(temp)){
                    int num;
                    cout << "add travel time between them : ";
                    cin >> num;
                    travelNetwork.addConnection(place, temp, num);
                    cout << "Do you want to add more neighbours yes(1)/no(0) :";
                    bool check;
                    while(true){
                        cin>> check;
                        if(cin.fail()){
                            cin.clear();
					        cin.ignore(numeric_limits<streamsize>::max(), '\n');
					        cout << "Invalid input. Please enter a valid option." << endl;
                        }
                        else{
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
					        break;
                        }
                    }
                    if(check){
                        cout<< " Add the neighbour name: ";
                        continue;
                    }
                    else{
                        break;
                    }
                            
                }
                else{
                    cout << "No such Location exist in map" << endl;
                    cout << "Enter from available places: ";
                    continue;
                }
            }
    }
}

int main() {
    TravelNetwork travelNetwork;

    // Add locations (tourist attractions)
    travelNetwork.addLocation("Museum");
    travelNetwork.addLocation("Park");
    travelNetwork.addLocation("Zoo");
    travelNetwork.addLocation("Beach");
    travelNetwork.addLocation("Castle");

    // Add connections with travel times
    travelNetwork.addConnection("Museum", "Park", 10);
    travelNetwork.addConnection("Museum", "Zoo", 15);
    travelNetwork.addConnection("Park", "Zoo", 5);
    travelNetwork.addConnection("Park", "Beach", 20);
    travelNetwork.addConnection("Zoo", "Castle", 25);
    travelNetwork.addConnection("Beach", "Castle", 30);

    // Variables for menu selection
    int choice = -1;
    while (choice != 0) {
        // Display menu
        cout << "=== MENU ===" << endl;
        cout << "1. Find shortest route" << endl;
        cout << "2. Find minimum tour package" << endl;
        cout << "3. Display travel network" << endl;
        cout << "4. Add Extra places to go" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string startLocation, endLocation;
                cout << "Enter the start location: ";
                cin >> startLocation;
                cout << "Enter the end location: ";
                cin >> endLocation;

                vector<string> shortestRoute = travelNetwork.findShortestRoute(startLocation, endLocation);

                cout << "Shortest route: ";
                for (const auto& loc : shortestRoute) {
                    cout << loc << " ";
                }
                cout << endl;
                break;
            }
            case 2: {
                vector<vector<string>> minimumTourPackage = travelNetwork.findMinimumTourPackage();

                cout << "Minimum tour package: " << endl;
                for (const auto& edge : minimumTourPackage) {
                    cout << edge[0] << " - " << edge[1] << endl;
                }
                break;
            }
            case 3: {
                travelNetwork.displayTravelNetwork();
                break;
            }

            case 4:{
                case4(travelNetwork);
                break;
            }

            case 0:
                cout << " !!! Thank You !!!  " << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

        cout << endl;
    }

    return 0;
}
