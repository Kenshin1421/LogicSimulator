#include<iostream>
#include "Renderer.h"
#include "EventHandler.h"
#include "Objects.h"

int main(){
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "App", sf::Style::Fullscreen);
	window.setFramerateLimit(120);

	std::vector<Node> NodesArray;
	std::vector<Wire> WiresArray;
	std::vector<Gate> GatesArray;
	std::vector<GateButton> GateButtons;

	Renderer renderer(window, NodesArray, WiresArray, GatesArray, GateButtons);
	EventHandler eventHandler(window, NodesArray, WiresArray, GatesArray,GateButtons,renderer.getBordSize());

	sf::Event ev;

	while(window.isOpen()){
		while(window.pollEvent(ev)){eventHandler.handle(ev);}
		renderer.Render();
	}
}