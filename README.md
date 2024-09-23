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

## How to use

1. **Requirements**: This project requires SFML (2.5.1 or later) and is best used on CLion, with Cmake (3.10 minimum).

2. **Setup**: Begin with cloning this repository.
   - Open CLion and click on **Get from VCS**.
   - Choose **Git** as the VCS and enter the repo URL: https://github.com/Seb-Dion/FinalProject
   - Clone the project in whichever directory you desire.
3. **Building the Program**
   - When the project opens in CLion, you may be met with errors, which can be easily fixed.
   - To fix these, make sure you update the **CMakeLists.txt** by replacing everywhere it says "FinalProject" with whatever you named your directory (naming the directory FinalProject is recommended). Once this is done, reload the changes.
   - If errors are still occuring, make sure to clear the cache. This can be done by running `rm -rf /Path/To/Your/Directory/cmake-build-debug` on your terminal, or just simply deleting the `cmake-build-debug` directory on your program.
   - Once this is complete, `build` your project on CLion and make sure the working directory is correct by editing the configuration of your program.
   - All of this should ensure that the project is properly loaded onto CLion and then just click `run`!
4. **Running the Program**
   - When the project starts running, the user will be met with a welcome window that introduces them to the project and prompts them to choose from six popular musical genres.
   - When a genre is chosen, the window will switch to the artist recommendations page, where ten total artists are given. Five of them will be given by the BFS traversal and the other five by the DFS traversal. It is possible for there to be overlap between the artists each algorithm recommends, of course.
   - As mentioned above, artists given are determined to be 'niche' due to their familiarity being below 0.5. Familiarity is a `double` value attached to the artist which ranges from 0.0 to 1.0, with 1.0 being very familiar and 0.0 being very unknown.
   - The time to execution is also given for both algorithms to further allow for comparison between the two traversals.
   - **Note**: Since we had to generate 90,000 random artists to increase the dataset, most of the artist names may appear in the form of `Artist 04d3ae7f` or something similar to this. Even though it doesn't necessarily give recognizable names, the program still works as expected and will work exactly the same with a dataset of all real artists.
     
