# Python Based Complex Civilization Model

This project demonstrates how a Python script can control multiple entities in a virtual environment and set up an advanced communication protocol to build a structured civilization with responsibilities and hierarchy to win a survival game. The game is designed as a board game where entities have limited visibility and interaction range, requiring them to communicate and strategize.

## Quick Start

### Building the Project

To build the project, you need to compile both the engine (C/C++) and the model (Python) components. The Makefile automates this process.

```bash
# Build the entire project
make all
```

### Running the Project

After building, you will have three main executables:

1. **Server** (`./engine_server`): This is the main game server.
   - **Usage:**
     ```bash
     ./engine_server -p <port> -x <width> -y <height> -n <name1> <name2> ... -c <clientNb> -f <freq>
     ```
   - Replace placeholders with:
     - `<port>`: Port number for server communication.
     - `<width>` and `<height>`: Size of the board.
     - `<name1> <name2> ...`: Names of the teams.
     - `<clientNb>`: Number of clients (players).
     - `<freq>`: Game frequency.

2. **Model** (`./civilization`): This is the AI component controlling the entities.
   - **Usage:**
     ```bash
     ./civilization -p <port> -n <team_name> -h <host>
     ```
   - Replace placeholders with:
     - `<port>`: Port number (must match the server).
     - `<team_name>`: The name of your team.
     - `<host>`: Hostname or IP of the server (default is "localhost").

3. **GUI** (`./engine_gui_<your architecture>`): This provides a visual representation of the game.
   - **Usage:**
    You have to run GUI executables from the `engine/gui` directory. As the GUI is architecture-dependent, you need to select the appropriate executable based on your system.

    You'll need libsfml version 2.5.x installed on your system to run the GUI.

     ```bash
     ./engine_gui -p <port> -h <machine>
     ```
   - Replace placeholders with:
     - `<port>`: Port number (must match the server).
     - `<machine>`: Hostname of the server.

### Cleaning the Build

To clean up the built files and reset the environment:

```bash
# Clean temporary files
make clean

# Fully clean and reset
make fclean
```

## Project Parts

This project is composed of two main parts:

- **Engine**: Written in C/C++, the engine is a server with a GUI that allows the Python model to connect and control entities.
- **Model**: Written in Python, the model handles the civilization system, including secure server connections and role management.

## Leader Role

The civilization is guided by a Leader, who has the primary role of orchestrating the game:

- Assigns roles to entities.
- Sets strategic steps for the civilization.
- Does not physically move on the board, relying on other entities to supply food and resources.

## Phases of Civilization

The civilization progresses through distinct phases based on the number of entities and their positioning on the map. 

### Phases Description

1. **Regroupement**: 
   - The first entity on the map becomes the Leader and broadcasts its location, prompting other entities to join and establish a base.

2. **Growth**: 
   - Focus on foundational growth:
     - **Poule**: Continuously forks (creates new entities).
     - **Marthyr**: Sacrifices itself, dropping all food to build a solid base.

3. **Exploration**: 
   - Resource gathering intensifies with various roles assigned to find resources and avoid death.

4. **Leveling Up**: 
   - A strict focus on resource accumulation and leveling up the civilization.

## Roles

### Roles Distribution

Roles evolve as the civilization advances, ensuring a dynamic approach to growth and survival.

**Phase One Roles:**
- **Poule**: Creates new entities until death.
- **Marthyr**: Sacrifices itself, providing essential resources.

**Phase Two Roles:**
- **Gatherer**: Gathers resources, focusing on survival.
- **Court**: Stay in the base, helps in forking and make the team level up when enough ressources are in the base.
- **Concubine**: Forks to increase population without succumbing to starvation.
- **Marthyr**: Continues to support by sacrificing itself.
- **Snail**: Use a snail shape pattern to cross all the map and pick up food and ressources.

**Phase Three Roles:**
- **Gatherer**: Advanced resource collection with specialized movement patterns.
- **Court**: Level up as much as possible to reach the maximum level.
- **Concubine**: Focuses on increasing the entity count without dying.
- **Marthyr**: Plays a critical support role, sacrificing itself when needed.
- **Snail**: Continues targeted resource collection.

### Leader’s Responsibilities

- Assigns the base location.
- Guides other entities (trantors).
- Assigns roles based on civilization needs.
- Defines and manages encryption keys for secure communication.

## Communication Protocol

Entities communicate using a strict protocol to maintain security and organization:

- **"B:<xor key>"**: The leader provides an XOR encryption key for secure communication.
- **"C:"**: Indicates that the message is ciphered using the provided key, with content encoded in Base64.
- **"R:<id>"**: A request broadcast from a new entity asking for a role.
- **"A:<id>:<ROLE>"**: The leader’s response assigning a role, where `<id>` matches the requesting entity's UUID.

## Licensing

This project is licensed under the MIT License.