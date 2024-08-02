#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "include/json.hpp"

using namespace std;
using namespace sf;
using json = nlohmann::json;

void setText(Text &text, float x, float y) {
    FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(Vector2f(x, y));
}

class Graph {
private:
    unordered_map<string, json> artists;
    unordered_map<string, vector<string>> adjacencyList;

public:
    void addArtist(const string& id, const json& artist) {
        artists[id] = artist;
        adjacencyList[id] = vector<string>();
    }

    void addEdge(const string& id1, const string& id2) {
        adjacencyList[id1].push_back(id2);
        adjacencyList[id2].push_back(id1);
    }

    vector<string> BFS(const string& startId) {
        queue<string> q;
        unordered_set<string> visited;
        vector<string> bfsResults;
        int count = 0;

        q.push(startId);
        visited.insert(startId);

        while (!q.empty() && count < 5) {
            string currentId = q.front();
            q.pop();
            json currentArtist = artists[currentId];

            if (currentArtist["familiarity"].get<double>() < 0.5) {
                bfsResults.push_back(currentArtist["name"]);
                count++;
            }

            vector<string> neighbors = adjacencyList[currentId];

            for (const auto& neighborId : neighbors) {
                if (visited.find(neighborId) == visited.end()) {
                    q.push(neighborId);
                    visited.insert(neighborId);
                }
            }
        }
        return bfsResults;
    }

    vector<string> DFS(const string& startId) {
        stack<string> s;
        unordered_set<string> visited;
        vector<string> dfsResults;
        int count = 0;

        s.push(startId);
        visited.insert(startId);

        while (!s.empty() && count < 5) {
            string currentId = s.top();
            s.pop();
            json currentArtist = artists[currentId];

            if (currentArtist["familiarity"].get<double>() < 0.5) {
                dfsResults.push_back(currentArtist["name"]);
                count++;
            }

            vector<string> neighbors = adjacencyList[currentId];

            for (const auto& neighborId : neighbors) {
                if (visited.find(neighborId) == visited.end()) {
                    s.push(neighborId);
                    visited.insert(neighborId);
                }
            }
        }
        return dfsResults;
    }

    vector<string> getAllArtistIds() const {
        vector<string> ids;
        for (const auto& pair : artists) {
            ids.push_back(pair.first);
        }
        return ids;
    }
};

void displayRecommendations(const vector<string>& bfsResults, const vector<string>& dfsResults, double bfsTime, double dfsTime, bool &goBackToHome) {
    Font font;
    if (!font.loadFromFile("files/otherFont.ttf")) {
        cerr << "Error loading font" << endl;
        return;
    }

    RenderWindow recs(VideoMode(800, 600), "Recommendations", Style::Close);

    float borderThickness = 15.0f;
    Color borderColor = sf::Color::Black;

    RectangleShape border;
    border.setSize(sf::Vector2f(recs.getSize().x - 2 * borderThickness, recs.getSize().y - 2 * borderThickness));
    border.setFillColor(sf::Color::Transparent); // No fill, only outline
    border.setOutlineThickness(borderThickness);
    border.setOutlineColor(borderColor);
    border.setPosition(borderThickness, borderThickness);

    Texture home;
    home.loadFromFile("files/home.png");

    Sprite homeButton(home);
    homeButton.setPosition(25, 525);

    while (recs.isOpen()) {
        Event event;
        while (recs.pollEvent(event)) {
            if (event.type == Event::Closed) {
                recs.close();
            } else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f mousePosition = recs.mapPixelToCoords(Mouse::getPosition(recs));
                if (homeButton.getGlobalBounds().contains(mousePosition)) {
                    goBackToHome = true;
                    recs.close();
                }
            }
        }

        recs.clear(Color(98, 122, 157));

        recs.draw(homeButton);
        recs.draw(border);

        Text recTitle("Artist Recommendations", font, 40);
        recTitle.setFillColor(Color::White);
        setText(recTitle, 400, 50);
        recs.draw(recTitle);

        Text bfsTitle("BFS Results:", font, 30);
        bfsTitle.setFillColor(Color::White);
        bfsTitle.setStyle(Text::Underlined);
        setText(bfsTitle, 200, 150);
        recs.draw(bfsTitle);

        int yBfs = 180;
        int yDfs = 180;
        for (const auto& name : bfsResults) {
            Text artistName(name, font, 20);
            artistName.setFillColor(Color::White);
            setText(artistName, 200, yBfs);
            recs.draw(artistName);
            yBfs += 40;
        }

        Text dfsTitle("DFS Results:", font, 30);
        dfsTitle.setStyle(Text::Underlined);
        dfsTitle.setFillColor(Color::White);
        setText(dfsTitle, 600, 150);
        recs.draw(dfsTitle);

        for (const auto& name : dfsResults) {
            Text artistName(name, font, 20);
            artistName.setFillColor(Color::White);
            setText(artistName, 600, yDfs);
            recs.draw(artistName);
            yDfs += 40;
        }

        // Display time taken for BFS and DFS
        Text bfsTimeText("BFS Time: " + to_string(bfsTime) + " ms", font, 20);
        bfsTimeText.setFillColor(Color::White);
        setText(bfsTimeText, 200, yBfs + 20);
        recs.draw(bfsTimeText);

        Text dfsTimeText("DFS Time: " + to_string(dfsTime) + " ms", font, 20);
        dfsTimeText.setFillColor(Color::White);
        setText(dfsTimeText, 600, yDfs + 20);
        recs.draw(dfsTimeText);

        recs.display();
    }
}

int main() {
    bool isRunning = true;
    bool goBackToHome = false;
    Graph graph;

    Texture button;
    button.loadFromFile("files/buttondepth.png");

    Texture musicNote;
    musicNote.loadFromFile("files/mus.png");

    Sprite musicalNote(musicNote);
    musicalNote.setPosition(50, 50);
    musicalNote.setScale(0.05,0.05);

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

    Font font;
    if (!font.loadFromFile("files/otherFont.ttf")) {
        cerr << "Error loading font" << endl;
        return 1;
    }

    Text title("UNDERGROUND ARTISTS", font, 50);
    title.setFillColor(Color::White);
    title.setStyle(Text::Bold);
    setText(title, 800 / 2, (600 / 2) - 175);
    float animationSpeed = 0.5f;

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

    RenderWindow welcome(VideoMode(800, 600), "Musical Mystery", Style::Close);

    welcome.setFramerateLimit(60);

    float borderThickness = 15.0f;
    Color borderColor = sf::Color::Black;

    RectangleShape border;
    border.setSize(sf::Vector2f(welcome.getSize().x - 2 * borderThickness, welcome.getSize().y - 2 * borderThickness));
    border.setFillColor(sf::Color::Transparent); // No fill, only outline
    border.setOutlineThickness(borderThickness);
    border.setOutlineColor(borderColor);
    border.setPosition(borderThickness, borderThickness);

    while (isRunning) {
        while (welcome.isOpen()) {
            Event event;
            while (welcome.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    welcome.close();
                    isRunning = false;
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePosition = welcome.mapPixelToCoords(Mouse::getPosition(welcome));
                    string selectedGenre;

                    if (hipHopButton.getGlobalBounds().contains(mousePosition)) {
                        selectedGenre = "hip hop";
                    } else if (folkRockButton.getGlobalBounds().contains(mousePosition)) {
                        selectedGenre = "folk rock";
                    } else if (popRockButton.getGlobalBounds().contains(mousePosition)) {
                        selectedGenre = "pop rock";
                    } else if (orchestraButton.getGlobalBounds().contains(mousePosition)) {
                        selectedGenre = "orchestra";
                    } else if (rAndBButton.getGlobalBounds().contains(mousePosition)) {
                        selectedGenre = "r&b";
                    } else if (countryButton.getGlobalBounds().contains(mousePosition)) {
                        selectedGenre = "country";
                    }

                    if (!selectedGenre.empty()) {
                        ifstream file("include/music.json");
                        json jsonData;
                        file >> jsonData;
                        file.close();

                        graph = Graph();
                        unordered_set<string> seenArtists;

                        for (const auto& item : jsonData) {
                            string genre = item["artist"]["terms"].get<string>();
                            string artistID = item["artist"]["id"];

                            if (genre == selectedGenre && seenArtists.find(artistID) == seenArtists.end()) {
                                graph.addArtist(artistID, item["artist"]);
                                seenArtists.insert(artistID);
                            }
                        }

                        vector<string> ids = graph.getAllArtistIds();
                        for (size_t i = 0; i < ids.size(); ++i) {
                            for (size_t j = i + 1; j < ids.size(); ++j) {
                                graph.addEdge(ids[i], ids[j]);
                            }
                        }

                        // Measure BFS time
                        auto startBfs = chrono::high_resolution_clock::now();
                        vector<string> bfsResults = graph.BFS(ids[0]);
                        auto endBfs = chrono::high_resolution_clock::now();
                        double bfsTime = chrono::duration<double, milli>(endBfs - startBfs).count();

                        // Measure DFS time
                        auto startDfs = chrono::high_resolution_clock::now();
                        vector<string> dfsResults = graph.DFS(ids[0]);
                        auto endDfs = chrono::high_resolution_clock::now();
                        double dfsTime = chrono::duration<double, milli>(endDfs - startDfs).count();

                        welcome.close();
                        displayRecommendations(bfsResults, dfsResults, bfsTime, dfsTime, goBackToHome);
                    }
                }
            }

            welcome.clear(Color(98, 122, 157));
            welcome.draw(border);
            welcome.draw(title);
            welcome.draw(subtitle);
            welcome.draw(musicalNote);
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

        if (goBackToHome) {
            goBackToHome = false;
            welcome.create(VideoMode(800, 600), "Musical Mystery", Style::Close);
        }
    }

    return 0;
}

