#include <string>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

int main() {

  // Initializing variables to hold input data
  int count, power;
  cin >> count;
  cin >> power;

  // Creating an adjacency list to hold URL mappings
  unordered_map < string, vector < string > > urlMap;

  // Populating the adjacency list using input data
  for (int i = 0; i < count; i++) {
    string one, two;
    cin >> one;
    cin >> two;
    // If the second URL has no adjacent URLs, create an empty vector for it
    if (urlMap.count(two) == 0) {
      urlMap[two] = vector < string > ();
    }
    // Adding the second URL to the adjacency list of the first URL
    urlMap[one].push_back(two);
  }

  // Creating a list of all the urls (URLs) in the adjacency list
  vector < string > urls;
  urls.reserve(urlMap.size());
  for (auto url: urlMap) {
    urls.push_back(url.first);
  }
  int urlCount = urls.size();

  // Sort the urls alphabetically
  sort(urls.begin(), urls.end());

  // Creating an adjacency list to hold URLs with their corresponding weights
  unordered_map < string, vector < pair < string, double > > > urlWeights;

  // Populating the adjacency list with weights
  for (int i = 0; i < urlCount; i++) {
    vector < string > pointingNodes = urlMap[urls[i]];
    int outDeg = pointingNodes.size();

    for (int j = 0; j < outDeg; j++) {
      string pointingNode = pointingNodes[j];

      if (urlWeights.find(pointingNode) == urlWeights.end()) {
        vector < pair < string, double > > blank;
        urlWeights[pointingNode] = blank;
      }

      double weight = 1.0 / (double) outDeg;
      urlWeights[pointingNode].push_back(make_pair(urls[i], weight));
    }
  }

  int powerCounter = 1;
  vector < double > ranks(urlCount, 1.0 / (double) urlCount);
  vector < double > result(urlCount, 0.0);

  // If number of power iterations is greater than 1, perform the power iterations
  if (power != 1) {
    while (powerCounter < power) {
      // Calculate the new page rank for each URL based on the previous page rank values
      for (int i = 0; i < urlCount; ++i) {
        vector < pair < string, double > > pointingNodes = urlWeights[urls[i]];
        int outDeg = pointingNodes.size();

        for (int j = 0; j < outDeg; ++j) {
          int pointingIndex = distance(urls.begin(), find(urls.begin(), urls.end(), pointingNodes[j].first));
          result[i] += (pointingNodes[j].second * ranks[pointingIndex]);
        }

      }
      // Update the page rank values for each URL and reset the result vector
      ranks = result;
      fill(result.begin(), result.end(), 0.0);
      powerCounter++;
    }
  }

  // Print the page ranks for each URL in alphabetical order
  for (int i = 0; i < urlCount; i++) {
    cout << urls[i] << " " << setprecision(2) << fixed << ranks[i] << endl;
  }
}