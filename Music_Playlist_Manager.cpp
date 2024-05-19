#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Song {
public:
    string title;
    string artist;

    Song(string t, string a) : title(t), artist(a) {}
};

class Node {
public:
    Song song;
    Node* prev;
    Node* next;

    Node(Song s) : song(s), prev(nullptr), next(nullptr) {}
};

class Playlist {
private:
    Node* head;
    Node* tail;
    Node* lastPlayed;
    vector<Node*> recentlyPlayed;
    int songCount;

    void addToRecentlyPlayed(Node* node) {
        if (recentlyPlayed.size() == 5) {
            recentlyPlayed.erase(recentlyPlayed.begin());
        }
        recentlyPlayed.push_back(node);
    }

    void saveToFile() {
        ofstream file("playlist.txt");
        Node* current = head;
        while (current) {
            file << current->song.title << "," << current->song.artist << endl;
            current = current->next;
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file("playlist.txt");
        if (!file.is_open()) return;
        string line;
        while (getline(file, line)) {
            size_t commaPos = line.find(',');
            if (commaPos != string::npos) {
                string title = line.substr(0, commaPos);
                string artist = line.substr(commaPos + 1);
                addSong(title, artist);
            }
        }
        file.close();
    }

public:
    Playlist() : head(nullptr), tail(nullptr), lastPlayed(nullptr), songCount(0) {
        loadFromFile();
    }

    ~Playlist() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    void addSong(string title, string artist) {
        Song song(title, artist);
        Node* newNode = new Node(song);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        songCount++;
        saveToFile();
    }

    void deleteSongByTitle(string title) {
        Node* current = head;
        while (current) {
            if (current->song.title == title) {
                if (current == head) {
                    head = head->next;
                    if (head) {
                        head->prev = nullptr;
                    } else {
                        tail = nullptr;
                    }
                } else if (current == tail) {
                    tail = tail->prev;
                    tail->next = nullptr;
                } else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }
                delete current;
                songCount--;
                saveToFile();
                return;
            }
            current = current->next;
        }
        cout << "Song not found!" << endl;
    }

    void deleteSongByPosition(int position) {
        if (position < 1 || position > songCount) {
            cout << "Invalid position!" << endl;
            return;
        }

        Node* current = head;
        for (int i = 1; i < position; ++i) {
            current = current->next;
        }

        if (current == head) {
            head = head->next;
            if (head) {
                head->prev = nullptr;
            } else {
                tail = nullptr;
            }
        } else if (current == tail) {
            tail = tail->prev;
            tail->next = nullptr;
        } else {
            current->prev->next = current->next;
            current->next->prev = current->prev;
        }

        delete current;
        songCount--;
        saveToFile();
    }

    void displayPlaylist() const {
        Node* current = head;
        int index = 1;
        while (current) {
            cout << index << ". " << current->song.title << " by " << current->song.artist << endl;
            current = current->next;
            index++;
        }
    }

    void playSong(int position) {
        if (position < 1 || position > songCount) {
            cout << "Invalid position!" << endl;
            return;
        }

        Node* current = head;
        for (int i = 1; i < position; ++i) {
            current = current->next;
        }

        cout << "Playing: " << current->song.title << " by " << current->song.artist << endl;
        lastPlayed = current;
        addToRecentlyPlayed(current);
        saveToFile();
    }

    void retrieveLastPlayed() const {
        if (lastPlayed) {
            cout << "Last played: " << lastPlayed->song.title << " by " << lastPlayed->song.artist << endl;
        } else {
            cout << "No song has been played yet." << endl;
        }
    }

    void retrieveRecentlyPlayed() const {
        if (recentlyPlayed.empty()) {
            cout << "No recently played songs." << endl;
            return;
        }
        cout << "Recently played songs:" << endl;
        for (const auto& node : recentlyPlayed) {
            cout << node->song.title << " by " << node->song.artist << endl;
        }
    }

    int getTotalSongs() const {
        return songCount;
    }
};

void displayMenu() {
    cout << "Menu:" << endl;
    cout << "1. Add Song" << endl;
    cout << "2. Delete Song by Title" << endl;
    cout << "3. Delete Song by Position" << endl;
    cout << "4. Display Playlist" << endl;
    cout << "5. Play Song" << endl;
    cout << "6. Retrieve Last Played Song" << endl;
    cout << "7. Retrieve Recently Played Songs" << endl;
    cout << "8. Get Total Number of Songs" << endl;
    cout << "9. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    Playlist playlist;
    int choice;

    do {
        displayMenu();
        cin >> choice;
        cin.ignore(); // To ignore the newline character left in the input buffer
        string title, artist;
        int position;

        switch (choice) {
            case 1:
                cout << "Enter song title: ";
                getline(cin, title);
                cout << "Enter artist: ";
                getline(cin, artist);
                playlist.addSong(title, artist);
                break;
            case 2:
                cout << "Enter song title to delete: ";
                getline(cin, title);
                playlist.deleteSongByTitle(title);
                break;
            case 3:
                cout << "Enter position to delete: ";
                cin >> position;
                playlist.deleteSongByPosition(position);
                break;
            case 4:
                playlist.displayPlaylist();
                break;
            case 5:
                cout << "Enter position to play: ";
                cin >> position;
                playlist.playSong(position);
                break;
            case 6:
                playlist.retrieveLastPlayed();
                break;
            case 7:
                playlist.retrieveRecentlyPlayed();
                break;
            case 8:
                cout << "Total number of songs: " << playlist.getTotalSongs() << endl;
                break;
            case 9:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
                break;
        }
    } while (choice != 9);

    return 0;
}