#pragma once
#include<SFML/Graphics.hpp>
#include<vector>
#include<unordered_map>
#include<string>
#include<iostream>

enum nodeType{
	input, mid, output, gate_in, gate_out
};

struct Node{
	int type;
	bool state;
	int gateid;
	sf::CircleShape node;

	std::vector<int> inputWires;
	std::vector<int> outputWires;

	void setColor(){
		if(state)
			node.setFillColor(on);
		else
			node.setFillColor(off);
	}
	void switchState(){state = !state;setColor();}
	void setState(bool p_state){state=p_state; setColor();}

	Node(int p_type, int win_X, sf::Vector2f pos , int p_gateid){
		type = p_type;
		state = false;
		gateid = p_gateid;
		setColor();
		if(p_type == input || p_type == output)
			node.setRadius(float(win_X/100));
		else
			node.setRadius(float(win_X/130));
		node.setOrigin({node.getRadius(), node.getRadius()});
		node.setPosition(pos);
		node.setOutlineColor(sf::Color::Black);
		if(p_type == input || p_type == output)
			node.setOutlineThickness(float(win_X/400));
		else
			node.setOutlineThickness(float(win_X/480));
	}
private:
	sf::Color off = sf::Color(45, 52, 54, 255);
	sf::Color on = sf::Color(231, 76, 60, 255);
};

struct Wire{
	bool state;
	sf::RectangleShape wire; 
	
	int fromNode;
	int toNode = -1;

	void setColor(){
		if(state)
			wire.setFillColor(on);
		else
			wire.setFillColor(off);
	}
	void switchState(){state = !state; setColor();}
	void setState(bool p_state){state = p_state;setColor();}

	Wire(bool initState, int from, int win_X, sf::Vector2f pos, float len, float angle){
		state = initState;
		setColor();
		fromNode = from;
		wire.setSize({len, float(win_X / 120)});
		wire.setOutlineThickness(float(win_X/480));
		wire.setOutlineColor(sf::Color::Black);
		wire.setOrigin(0.0f, float(win_X/240));
		wire.setPosition(pos);
		wire.setRotation(angle);
	}


private:
	sf::Color off = sf::Color(45, 52, 54, 255);
	sf::Color on = sf::Color(231, 76, 60, 255);
};

struct Gate{
	sf::Text title;
	sf::RectangleShape gate;

	std::vector<int> inputNodes;
	std::vector<int> outputNodes;

	std::unordered_map<int, int>* function;

	void calcOutput(std::vector<Node>* NodesArray){
		int inp = 0;
		for(int i : inputNodes){
			inp += (*NodesArray)[i].state;
			inp*=10;
		}
		inp/=10;
		int out = (*function)[inp];
		for(int i = outputNodes.size() - 1; i >= 0; i--){
			(*NodesArray)[outputNodes[i]].setState(bool(out%10));
			out/=10;
		}
	}

	void moveGate(sf::Vector2f pos, std::vector<Node>& NArray){
		gate.setPosition(pos);
		title.setPosition(pos);
		float buff = gate.getSize().y/inputNodes.size();
		for(int i = 0; i < inputNodes.size(); i++){
			NArray[inputNodes[i]].node.setPosition(pos.x - gate.getSize().x*gate.getScale().x/2, pos.y - gate.getSize().y / 2 + buff / 2 + (i * buff));
		}
		buff = gate.getSize().y / outputNodes.size();
		for(int i = 0; i < outputNodes.size(); i++){
			NArray[outputNodes[i]].node.setPosition(pos.x + gate.getSize().x * gate.getScale().x / 2, pos.y - gate.getSize().y/2 + buff/2+(i*buff));
		}
	}
	
	Gate(sf::Text& p_title, sf::RectangleShape& p_gate, sf::Vector2f pos , int win_X,int input, int output, int p_id,std::unordered_map<int, int>& func, std::vector<Node>& NArray){
		title = p_title;
		gate = p_gate;
		function = &func;

		//winx/75
		gate.setScale(p_gate.getScale().x, 1);
		sf::Vector2f size = {p_gate.getSize().x, p_gate.getSize().y};
		int max = (input>=output)*input + (output>input)*output;
		size.y = float(((win_X/130)+(win_X/480))*2*max) + float((max+1)*win_X/480);
		gate.setOrigin(size.x/2, size.y/2);
		//if(size.y < p_gate.getSize().y*p_gate.getScale().y){size.y = p_gate.getSize().y * p_gate.getScale().y;}
		gate.setSize(size);

		for(int i = 0; i < input; i++){
			Node n(gate_in, win_X, {gate.getPosition().x - gate.getSize().x/2, gate.getPosition().y}, p_id);
			NArray.push_back(n);
			inputNodes.push_back(NArray.size()-1);
		}
		for(int i = 0; i < output; i++){
			Node n(gate_out, win_X, { gate.getPosition().x + gate.getSize().x / 2, gate.getPosition().y }, p_id);
			NArray.push_back(n);
			outputNodes.push_back(NArray.size() - 1);
		}

		moveGate(pos, NArray);
	}
};

struct GateButton{
	sf::Text title;
	sf::RectangleShape button;
	int input = 0;
	int output = 0;
	std::unordered_map<int, int> function;

	GateButton(std::string text, sf::Font& font, int win_X, sf::Vector2f pos, int p_input, int p_output, std::vector<std::string>& p_data){
		title.setString(text);
		title.setFont(font);
		title.setCharacterSize(int(win_X/50));
		button.setFillColor(buttonColor);
		button.setSize(title.getGlobalBounds().getSize());
		button.setPosition(pos);
		title.setPosition(pos);
		button.setOrigin(button.getSize().x/2, button.getSize().y/2);
		title.setOrigin(button.getSize().x/2, button.getSize().y - win_X/500);
		button.setScale(1.5, 1.5);
		button.move(button.getSize().x/2 + button.getSize().x/4, 0);
		title.move(button.getSize().x/2 + +button.getSize().x / 4, 0);
		button.setOutlineColor(sf::Color::Black);
		button.setOutlineThickness(win_X/800);
		
		input = p_input;
		output = p_output;

		for(int i = 0; i < p_data.size(); i += 2){
			function[std::stoi(p_data[i])] = std::stoi(p_data[i+1]);
		}
	}

private:
	sf::Color buttonColor = sf::Color(45, 52, 54, 255);
};