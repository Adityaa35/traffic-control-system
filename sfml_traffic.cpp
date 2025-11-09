#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Traffic Control SFML");
    
    // Traffic data
    vector<string> dirs = {"North", "East", "South", "West"};
    vector<int> traffic = {15, 25, 10, 30};
    vector<int> green_times = {8, 12, 6, 14};
    
    // Colors
    sf::Color colors[] = {sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow};
    
    while (window.isOpen()) {
        for (auto event = window.pollEvent(); event.has_value(); event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
        window.clear(sf::Color::Black);
        
        // Draw traffic lanes (no text to avoid font issues)
        for (int i = 0; i < 4; i++) {
            sf::RectangleShape lane(sf::Vector2f(200, 40));
            lane.setPosition({100.f, 150.f + i * 80.f});
            lane.setFillColor(colors[i]);
            window.draw(lane);
        }
        
        window.display();
    }
    
    return 0;
}