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
#include <SFML/Window/Keyboard.hpp>
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
vector<json> BFS(const vector<json>& artists) {
    vector<json> artistsReturn;
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
            //cout << "BFS Artist Name: " << current["name"] << endl;
            artistsReturn.push_back(current);
            count++;
        }

        for (const auto& artist : artists) {
            if (visited.find(artist["id"]) == visited.end()) {
                q.push(artist);
                visited.insert(artist["id"]);
            }
        }
    }
    return artistsReturn;
}

// DFS function for artists
vector<json> DFS(const vector<json>& artists) {
    vector<json> artistsReturn;
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
            //cout << "DFS Artist Name: " << current["name"] << endl;
            artistsReturn.push_back(current);
            count++;
        }

        for (const auto& artist : artists) {
            if (visited.find(artist["id"]) == visited.end()) {
                s.push(artist);
                visited.insert(artist["id"]);
            }
        }
    }
    return artistsReturn;
}

vector<vector<json>> createVectorofArtist(string userGenre){
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
    vector<vector<json>> BFSandDFSvectors;
    //cout << "BFS results:" << endl;
    vector<json> bfs = BFS(matchingArtists);
    BFSandDFSvectors.push_back(bfs);
    //cout << "\nDFS results:" << endl;
    vector<json> dfs = DFS(matchingArtists);
    BFSandDFSvectors.push_back(dfs);
    return BFSandDFSvectors;
}

int main() {
    bool isRunning = true;

    // Load button texture
    Texture button;
    if (!button.loadFromFile("files/buttondepth.png")) {
        cerr << "Error loading button texture" << endl;
        return 1;
    }

    // Create button sprites
    Sprite hipHopButton(button);
    hipHopButton.setPosition(100, 280);
    hipHopButton.setScale(0.50, 0.50);

    Sprite folkRockButton(button);
    folkRockButton.setPosition(100, 380);
    folkRockButton.setScale(0.50, 0.50);

    Sprite popRockButton(button);
    popRockButton.setPosition(100, 480);
    popRockButton.setScale(0.50, 0.50);

    Sprite orchestraButton(button);
    orchestraButton.setPosition(500, 280);
    orchestraButton.setScale(0.5, 0.5);

    Sprite rAndBButton(button);
    rAndBButton.setPosition(500, 380);
    rAndBButton.setScale(0.5, 0.5);

    Sprite countryButton(button);
    countryButton.setPosition(500, 480);
    countryButton.setScale(0.5, 0.5);

    // Create and load font
    Font font;
    if (!font.loadFromFile("files/otherFont.ttf")) {
        cerr << "Error loading font" << endl;
        return 1;
    }

    // Create text elements
    Text title("A MUSICAL MYSTERY!", font, 50);
    title.setFillColor(Color::White);
    title.setStyle(Text::Bold);
    setText(title, 800 / 2, (600 / 2) - 175);

    Text subtitle("Select a genre below for niche artist recommendations...", font, 18);
    subtitle.setFillColor(Color::White);
    subtitle.setStyle(Text::Bold);
    setText(subtitle, 800 / 2, (600 / 2) - 75);

    Text hipHop("Hip Hop", font, 15);
    hipHop.setFillColor(Color::Black);
    hipHop.setStyle(Text::Bold);
    setText(hipHop, 195, 312);

    Text folkRock("Folk Rock", font, 15);
    folkRock.setFillColor(Color::Black);
    folkRock.setStyle(Text::Bold);
    setText(folkRock, 195, 412);

    Text popRock("Pop Rock", font, 15);
    popRock.setFillColor(Color::Black);
    popRock.setStyle(Text::Bold);
    setText(popRock, 195, 512);

    Text orchestra("Orchestra", font, 15);
    orchestra.setFillColor(Color::Black);
    orchestra.setStyle(Text::Bold);
    setText(orchestra, 595, 312);

    Text rAndB("R&B", font, 15);
    rAndB.setFillColor(Color::Black);
    rAndB.setStyle(Text::Bold);
    setText(rAndB, 595, 412);

    Text country("Country", font, 15);
    country.setFillColor(Color::Black);
    country.setStyle(Text::Bold);
    setText(country, 595, 512);

    // Create the welcome window
    RenderWindow welcome(VideoMode(800, 600), "Musical Mystery", Style::Close);

    while (isRunning) {
        // Handle events in the welcome window
        while (welcome.isOpen()) {
            Event event;
            while (welcome.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    welcome.close();
                    isRunning = false; // Set isRunning to false to exit the main loop
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePosition = welcome.mapPixelToCoords(Mouse::getPosition(welcome));

                    if (hipHopButton.getGlobalBounds().contains(mousePosition)) {
                        welcome.close();
                        RenderWindow recs(VideoMode(800, 600), "Recommendations", Style::Close);

                        // Handle events in the recommendations window
                        while (recs.isOpen()) {
                            Event event;
                            while (recs.pollEvent(event)) {
                                if (event.type == Event::Closed) {
                                    recs.close();
                                }
                            }

                            recs.clear(Color(98, 122, 157));
                            // Add drawing code for the new window here
                            recs.display();
                        }
                    }
                    else if (folkRockButton.getGlobalBounds().contains(mousePosition)){
                        welcome.close();
                        RenderWindow recs(VideoMode(800, 600), "Recommendations", Style::Close);
                        while (recs.isOpen()) {
                            Event event;
                            while (recs.pollEvent(event)) {
                                if (event.type == Event::Closed) {
                                    recs.close();
                                }
                            }

                            recs.clear(Color(98, 122, 157));
                            // Add drawing code for the new window here
                            recs.display();
                        }
                    }
                    else if (popRockButton.getGlobalBounds().contains(mousePosition)){
                        welcome.close();
                        RenderWindow recs(VideoMode(800, 600), "Recommendations", Style::Close);
                        while (recs.isOpen()) {
                            Event event;
                            while (recs.pollEvent(event)) {
                                if (event.type == Event::Closed) {
                                    recs.close();
                                }
                            }

                            recs.clear(Color(98, 122, 157));
                            // Add drawing code for the new window here
                            recs.display();
                        }
                    }
                }
            }

            // Render the welcome window
            welcome.clear(Color(98, 122, 157));
            welcome.draw(title);
            welcome.draw(subtitle);
            welcome.draw(hipHopButton);
            welcome.draw(hipHop);
            welcome.draw(folkRockButton);
            welcome.draw(folkRock);
            welcome.draw(popRockButton);
            welcome.draw(popRock);
            welcome.draw(orchestraButton);
            welcome.draw(orchestra);
            welcome.draw(rAndBButton);
            welcome.draw(rAndB);
            welcome.draw(countryButton);
            welcome.draw(country);
            welcome.display();
        }
    }

    /*string userGenre;
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
    DFS(matchingArtists);*/

    return 0;
}