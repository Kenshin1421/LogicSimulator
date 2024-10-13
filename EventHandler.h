#pragma once
#include<SFML/Graphics.hpp>
#include "Objects.h"
#include<math.h>

class EventHandler{
public:
	EventHandler(sf::RenderWindow& p_window, std::vector<Node>& p_NodesArray, std::vector<Wire>& p_WiresArray, std::vector<Gate>& p_GatesArray,std::vector<GateButton>& p_GateButtons,float p_bordSize){
		window = &p_window;
		NodesArray = &p_NodesArray;
		WiresArray = &p_WiresArray;
		GatesArray = &p_GatesArray;
		GateButtons = &p_GateButtons;
		bordSize = p_bordSize;
	}
	void handle(sf::Event& event);
private:
	sf::RenderWindow* window;
	std::vector<Node>* NodesArray;
	std::vector<Wire>* WiresArray;
	std::vector<Gate>* GatesArray;
	std::vector<GateButton>* GateButtons;
	void UpdatePower();
	float bordSize;
	bool makingnode = false;
	bool makingwire = false;
	bool makinggate = false;
};

