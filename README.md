# UNDERGROUND ARTISTS: TRAVERSING AN UNKNOWN WORLD

## Project Overview

**Underground Artists: Traversing an Unknown World** is an application designed to help users discover lesser-known musical artists. The project leverages breadth-first search (BFS) and depth-first search (DFS) algorithms to traverse a graph representing musical artists, recommending niche artists with a familiarity score below 0.5. This tool provides a platform for users to explore new music beyond mainstream choices, focusing on underground genres.

## Features

- **Graph-Based Artist Representation**: Artists and their relationships are modeled as a graph, with nodes representing artists and edges representing connections or similarities between them.
- **Genre-Based Filtering**: Users can select from various genres to discover niche artists within specific musical styles.
- **Recommendation System**: BFS and DFS algorithms are used to recommend artists based on user-selected genres.
- **Interactive UI**: A graphical interface using SFML (Simple and Fast Multimedia Library) provides an intuitive and engaging experience for users to explore recommendations.

## Technologies Used

- **C++**: The core programming language used for implementing the application logic.
- **SFML**: A multimedia library used to create the graphical user interface.
- **nlohmann/json**: A C++ library for parsing and handling JSON data, used to manage artist information.
- **JSON**: Used for storing artist data, including genres, familiarity scores, and other relevant details.

## How It Works

1. **Data Loading**: The application reads artist data from a JSON file (`updated_music.json`). This file includes 100,000 musical artists and songs, but only the artists are used for this project. Each artist entry includes information such as the artist's name, genre, and familiarity score. 10,000 of these artists are real and the other 90,000 are randomly generated. 

2. **Graph Construction**: 
   - **Nodes**: The program reads through the entire dataset and adds artists to the graph who have less than a 0.5 familiarity score.
   - **Edges**: Connections between artists are established based on shared genres or other criteria.

3. **User Interaction**:
   - Upon launching the application, users are presented with a selection of genres.
   - Users can click on a genre to generate recommendations of niche artists in that genre.

4. **Recommendations**:
   - **BFS and DFS Algorithms**: The application utilizes BFS and DFS to traverse the graph and recommend up to five niche artists with a familiarity score of less than 0.5.
   - **Performance Comparison**: The execution times for BFS and DFS are displayed, with the faster time highlighted in green and the slower in red.

5. **Display Results**: These recommendations are then displayed on a new window which also shows the differences between the two algorithms mentioned above.

