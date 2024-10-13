#pragma once
#include<SFML/Graphics.hpp>
#include<filesystem>
#include<fstream>
#include<iostream>
#include "Objects.h"

class Renderer{
public:
	void loadGates();
	void Render();
	float getBordSize();

	Renderer(sf::RenderWindow& p_window, std::vector<Node>& p_NodesArray, std::vector<Wire>& p_WiresArray, std::vector<Gate>& p_GatesArray, std::vector<GateButton>& p_GateButtons){
		window = &p_window;
		NodesArray = &p_NodesArray;
		WiresArray = &p_WiresArray;
		GatesArray = &p_GatesArray;
		GateButtons = &p_GateButtons;
		win_X = window->getSize().x;
		win_Y = window->getSize().y;
		textFont.loadFromFile("font.ttf");
		border.setSize({float(win_X), float(win_Y)});
		border.setFillColor(bgColor);
		border.setOutlineColor(bordColor);
		border.setOutlineThickness(-1*float(win_X/18));
		loadGates();
	}
private:
	sf::RenderWindow* window;
	std::vector<Node>* NodesArray;
	std::vector<Wire>* WiresArray;
	std::vector<Gate>* GatesArray;
	std::vector<GateButton>* GateButtons;
	sf::Font textFont;
	sf::Color bgColor = sf::Color(223, 230, 233, 255);
	sf::Color bordColor = sf::Color(99, 110, 114, 255);
	sf::RectangleShape border;
	sf::RectangleShape TextBox;
	int win_X;
	int win_Y;
};