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

//Helper function for positioning text.
void setText(Text &text, float x, float y) {
    FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(Vector2f(x, y));
}

//Class for creating artist graph.
class Graph {
private:
    //map for storing artists and their IDs...graph in the form of an adjacency list.
    unordered_map<string, json> artists;
    unordered_map<string, vector<string>> adjacencyList;

public:
    //adds artist nodes to the graph.
    void addArtist(const string& id, const json& artist) {
        artists[id] = artist;
        adjacencyList[id] = vector<string>();
    }

    //creates an edge between artists.
    void addEdge(const string& id1, const string& id2) {
        adjacencyList[id1].push_back(id2);
        adjacencyList[id2].push_back(id1);
    }

    //main breadth-first traversal function...takes in the starting node and returns recommended artists.
    vector<string> BFS(const string& startId) {
        queue<string> q;
        unordered_set<string> visited;
        vector<string> bfsResults;
        int count = 0;

        q.push(startId);
        visited.insert(startId);

        //Only gives at most 5 recommended artists.
        while (!q.empty() && count < 5) {
            string currentId = q.front();
            q.pop();
            json currentArtist = artists[currentId];

            //makes sure to only give niche artists with less than 0.5 familiarity score.
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

    //main depth-first traversal function with same input and output as BFS.
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

    //getter function that returns a vector of all artists' IDs in the graph.
    [[nodiscard]] vector<string> getIDs() const {
        vector<string> ids;
        ids.reserve(artists.size());
        for (const auto& pair : artists) {
            ids.push_back(pair.first);
        }
        return ids;
    }
};


//Creates the recommendations window and all properties.
//Takes in artist recs and time comparisons.
void displayRecommendations(const vector<string>& bfsResults, const vector<string>& dfsResults, double bfsTime, double dfsTime, bool &goBackToHome) {
    //sets font.
    Font font;
    font.loadFromFile("files/otherFont.ttf");

    //Opens window.
    RenderWindow recs(VideoMode(800, 600), "Recommendations", Style::Close);

    //creates border outline.
    float borderThickness = 15.0f;
    Color borderColor = sf::Color::Black;

    RectangleShape border;
    border.setSize(sf::Vector2f(recs.getSize().x - 2 * borderThickness, recs.getSize().y - 2 * borderThickness));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(borderThickness);
    border.setOutlineColor(borderColor);
    border.setPosition(borderThickness, borderThickness);

    //home button to return back to welcome window.
    Texture home;
    home.loadFromFile("files/home.png");

    Sprite homeButton(home);
    homeButton.setPosition(25, 525);

    //box outlines properties.
    float resultsBoxWidth = 350;
    float resultsBoxHeight = 400;
    float resultsBoxY = 120;

    RectangleShape bfsResultsBox;
    bfsResultsBox.setSize(Vector2f(resultsBoxWidth, resultsBoxHeight));
    bfsResultsBox.setFillColor(Color(80, 80, 80, 100));
    bfsResultsBox.setOutlineThickness(2);
    bfsResultsBox.setOutlineColor(Color::White);
    bfsResultsBox.setPosition(50, resultsBoxY);

    RectangleShape dfsResultsBox;
    dfsResultsBox.setSize(Vector2f(resultsBoxWidth, resultsBoxHeight));
    dfsResultsBox.setFillColor(Color(80, 80, 80, 100));
    dfsResultsBox.setOutlineThickness(2);
    dfsResultsBox.setOutlineColor(Color::White);
    dfsResultsBox.setPosition(400, resultsBoxY);

    //handles events of clicking the home button and closing the window.
    while (recs.isOpen()) {
        Event event{};
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

        //clearing and drawing window.
        recs.clear(Color(98, 122, 157));
        recs.draw(homeButton);
        recs.draw(border);
        recs.draw(bfsResultsBox);
        recs.draw(dfsResultsBox);

        //Creating window text.
        Text recTitle("Artist Recommendations", font, 40);
        recTitle.setFillColor(Color::White);
        setText(recTitle, 400, 65);
        recs.draw(recTitle);

        Text bfsTitle("BFS Results:", font, 30);
        bfsTitle.setFillColor(Color::White);
        bfsTitle.setStyle(Text::Underlined);
        setText(bfsTitle, 225, resultsBoxY + 20);
        recs.draw(bfsTitle);

        //Creates text for each artist in the BFS vector.
        float yBfs = resultsBoxY + 75;
        for (const auto& name : bfsResults) {
            Text artistName(name, font, 20);
            artistName.setFillColor(Color::White);
            setText(artistName, 225, yBfs);
            recs.draw(artistName);
            yBfs += 40;
        }

        Text dfsTitle("DFS Results:", font, 30);
        dfsTitle.setStyle(Text::Underlined);
        dfsTitle.setFillColor(Color::White);
        setText(dfsTitle, 575, resultsBoxY + 20);
        recs.draw(dfsTitle);

        float yDfs = resultsBoxY + 75;
        for (const auto& name : dfsResults) {
            Text artistName(name, font, 20);
            artistName.setFillColor(Color::White);
            setText(artistName, 575, yDfs);
            recs.draw(artistName);
            yDfs += 40;
        }

        //Displays traversal execution times.
        Text bfsTimeText("BFS Time: " + to_string(bfsTime) + " ms", font, 20);
        Text dfsTimeText("DFS Time: " + to_string(dfsTime) + " ms", font, 20);
        auto lightRed = Color(255,127,127);
        auto lightGreen = Color(144,238,144);

        //Sets the faster time to green and the slower time to red.
        if (bfsTime < dfsTime) {
            bfsTimeText.setFillColor(lightGreen);
            dfsTimeText.setFillColor(lightRed);
        } else {
            bfsTimeText.setFillColor(lightRed);
            dfsTimeText.setFillColor(lightGreen);
        }

        setText(bfsTimeText, 225, yBfs + 20);
        recs.draw(bfsTimeText);

        setText(dfsTimeText, 575, yDfs + 20);
        recs.draw(dfsTimeText);

        recs.display();
    }
}



//main function.
int main() {
    //Creates graph and conditionals.
    bool isRunning = true;
    bool goBackToHome = false;
    Graph graph;

    //Loading various textures and creating sprites.
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
    font.loadFromFile("files/otherFont.ttf");

    Text title("UNDERGROUND ARTISTS", font, 50);
    title.setFillColor(Color::White);
    title.setStyle(Text::Bold);
    setText(title, 400, (300) - 175);

    Text subtitle("Select a genre below for niche artist recommendations...", font, 18);
    subtitle.setFillColor(Color::White);
    subtitle.setStyle(Text::Bold);
    setText(subtitle, 400, 225);

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

    RenderWindow welcome(VideoMode(800, 600), "Underground Artists", Style::Close);

    welcome.setFramerateLimit(60);

    //Creates black border around window.
    float borderThickness = 15.0f;
    Color borderColor = sf::Color::Black;

    RectangleShape border;
    border.setSize(sf::Vector2f(welcome.getSize().x - 2 * borderThickness, welcome.getSize().y - 2 * borderThickness));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(borderThickness);
    border.setOutlineColor(borderColor);
    border.setPosition(borderThickness, borderThickness);

    //Main welcome window loop.
    while (isRunning) {
        while (welcome.isOpen()) {
            Event event{};
            while (welcome.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    welcome.close();
                    isRunning = false;
                }

                //Handles genre button events and assigns the selected genre.
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
                        //Parsing data set.
                        ifstream file("include/updated_music.json");
                        json jsonData;
                        file >> jsonData;
                        file.close();

                        //Set of unique artists.
                        graph = Graph();
                        unordered_set<string> seenArtists;

                        //Adds artist to graph if it has the selected genre (terms) and is unique.
                        for (const auto& item : jsonData) {
                            string genre = item["artist"]["terms"].get<string>();
                            string artistID = item["artist"]["id"];

                            if (genre == selectedGenre && seenArtists.find(artistID) == seenArtists.end()) {
                                graph.addArtist(artistID, item["artist"]);
                                seenArtists.insert(artistID);
                            }
                        }

                        //Adding edges.
                        vector<string> ids = graph.getIDs();
                        for (size_t i = 0; i < ids.size(); ++i) {
                            for (size_t j = i + 1; j < ids.size(); ++j) {
                                graph.addEdge(ids[i], ids[j]);
                            }
                        }

                        //Getting traversal times.
                        auto startBfs = chrono::high_resolution_clock::now();
                        vector<string> bfsResults = graph.BFS(ids[0]);
                        auto endBfs = chrono::high_resolution_clock::now();
                        double bfsTime = chrono::duration<double, milli>(endBfs - startBfs).count();

                        auto startDfs = chrono::high_resolution_clock::now();
                        vector<string> dfsResults = graph.DFS(ids[0]);
                        auto endDfs = chrono::high_resolution_clock::now();
                        double dfsTime = chrono::duration<double, milli>(endDfs - startDfs).count();

                        welcome.close();
                        displayRecommendations(bfsResults, dfsResults, bfsTime, dfsTime, goBackToHome);
                    }
                }
            }

            //Displaying and drawing.
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

        //Handles home button.
        if (goBackToHome) {
            goBackToHome = false;
            welcome.create(VideoMode(800, 600), "Underground Artists", Style::Close);
        }
    }

    return 0;
}

