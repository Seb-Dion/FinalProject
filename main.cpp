#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <stack>
#include <unordered_set>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "include/json.hpp"
using namespace std;
using namespace sf;

using json = nlohmann::json;

void setText(Text &text, float x, float y){
    FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(Vector2f(x, y));
}

// Function to compare artists by familiarity
bool compareByFamiliarity(const json& a, const json& b) {
    return a["familiarity"].get<double>() < b["familiarity"].get<double>();
}

// BFS function for artists
void BFS(const vector<json>& artists) {
    queue<json> q;
    unordered_set<string> visited;
    int count = 0;

    if (!artists.empty()) {
        q.push(artists[0]);
        visited.insert(artists[0]["id"]);
    }

    while (!q.empty() && count < 5) {
        json current = q.front();
        q.pop();

        if (current["familiarity"].get<double>() < 0.5) {
            cout << "BFS Artist Name: " << current["name"] << endl;
            count++;
        }

        for (const auto& artist : artists) {
            if (visited.find(artist["id"]) == visited.end()) {
                q.push(artist);
                visited.insert(artist["id"]);
            }
        }
    }
}

// DFS function for artists
void DFS(const vector<json>& artists) {
    stack<json> s;
    unordered_set<string> visited;
    int count = 0;

    if (!artists.empty()) {
        s.push(artists[0]);
        visited.insert(artists[0]["id"]);
    }

    while (!s.empty() && count < 5) {
        json current = s.top();
        s.pop();

        if (current["familiarity"].get<double>() < 0.5) {
            cout << "DFS Artist Name: " << current["name"] << endl;
            count++;
        }

        for (const auto& artist : artists) {
            if (visited.find(artist["id"]) == visited.end()) {
                s.push(artist);
                visited.insert(artist["id"]);
            }
        }
    }
}

int main() {
    // Prompt user for genre
    RenderWindow welcome(VideoMode(800, 600), "Musical Mystery", Style::Close);
    Font font;
    font.loadFromFile("files/font.ttf");

    Text title;
    title.setFont(font);
    title.setString("A MUSICAL MYSTERY!");
    title.setCharacterSize(50);
    title.setFillColor(Color::Black);
    title.setStyle(Text::Bold);
    title.setStyle(Text::Underlined);
    setText(title, 800 / 2, (600 / 2) - 150);

    while(welcome.isOpen()){
        Event event;
        while(welcome.pollEvent(event)){
            if(event.type == Event::Closed)
                welcome.close();
        }
        welcome.clear(Color::White);
        welcome.draw(title);
        welcome.display();
    }

    string userGenre;
    cout << "Hello! Welcome to a Musical Mystery" << endl;
    cout << "Enter the genre of music you like: ";
    getline(cin, userGenre);

    // Open and read the JSON file
    ifstream file("include/music.json");
    if (!file.is_open()) {
        cerr << "Unable to open file" << endl;
        return 1;
    }

    json jsonData;
    file >> jsonData;
    file.close();

    // Process the JSON data to find matching artists
    vector<json> matchingArtists;
    unordered_set<string> seenArtists;

    for (const auto& item : jsonData) {
        string genre = item["artist"]["terms"].get<string>();
        string artistID = item["artist"]["id"];

        // Check if the genre matches and the artist hasn't been added yet
        if (genre == userGenre && seenArtists.find(artistID) == seenArtists.end()) {
            matchingArtists.push_back(item["artist"]);
            seenArtists.insert(artistID);
        }
    }

    // Sort artists by familiarity (more unfamiliar first)
    sort(matchingArtists.begin(), matchingArtists.end(), compareByFamiliarity);

    // Output matching artists using BFS and DFS
    cout << "BFS results:" << endl;
    BFS(matchingArtists);
    cout << "\nDFS results:" << endl;
    DFS(matchingArtists);

    return 0;
}
