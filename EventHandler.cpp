#include "EventHandler.h"
#include<iostream>
#include<unordered_set>


float calcDist(float x1, float y1, float x2, float y2){ return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));}

float calcAngle(float x1, float y1, float x2, float y2){
	//x1,y1 are origin cords
	//std::cout << x1 << y1 << x2 << y2 <<std::endl;
	float factor = 180/3.1415; //Radians to degrees
	if((x2 - x1 >= 0 && y2 - y1 <= 0) || (x2 - x1 > 0 && y2 - y1 > 0)){ //1st and 4th quad
		return atan2f(y1-y2, x2-x1)*factor*-1;
	}
	else if(x2 - x1 < 0 && y2 - y1 < 0){ //2nd quad
		return (-1*atan2f(y1-y2, x2-x1))*factor;
	}
	else{ // 3rd quad
		return (-1 * atan2f(y1 - y2, x2 - x1)) * factor;
	}
}

int nodeClick(std::vector<Node>* NArray, sf::Vector2i& m){
	for(int i = 0; i < NArray->size(); i++){
		sf::Vector2f temp = (*NArray)[i].node.getPosition();
		float r = (*NArray)[i].node.getRadius();
		if(calcDist(temp.x, temp.y, float(m.x), float(m.y)) < r){
			return i;
		}
	}
	return -1;
}

int gateButtonClick(std::vector<GateButton>* Buttons, sf::Vector2i& m){
	for(int i = 0; i < Buttons->size(); i++){
		sf::Vector2f pos = (*Buttons)[i].button.getPosition();
		sf::Vector2f size = (*Buttons)[i].button.getSize();
		sf::Vector2f scale= (*Buttons)[i].button.getScale();
		if(m.x > pos.x - size.x*scale.x / 2 && m.x < pos.x + size.x*scale.x / 2 && m.y > pos.y - size.y*scale.y / 2 && m.y < pos.y + size.y*scale.y / 2){
			return i;
		}
	}
	return -1;
}

void EventHandler::handle(sf::Event& event){
	sf::Vector2i mousepos = sf::Mouse::getPosition(*window);
	switch(event.type){
		case sf::Event::Closed:
			window->close();
		break;
		case sf::Event::KeyPressed:
			if(event.key.code == sf::Keyboard::I && makingnode == false){
				makingnode = true;
				Node n(nodeType::input, window->getSize().x, {bordSize, float(mousepos.y)}, -1);
				NodesArray->push_back(n);
			}
			if(event.key.code == sf::Keyboard::O && makingnode == false){
				makingnode = true;
				Node n(nodeType::output, window->getSize().x, { window->getSize().x-bordSize, float(mousepos.y) }, -1);
				NodesArray->push_back(n);
			}
		break;
		case sf::Event::MouseMoved:
			if(makingnode && mousepos.y > float(bordSize) && float(mousepos.y) < window->getSize().y - bordSize){
				NodesArray->back().node.setPosition({NodesArray->back().node.getPosition().x, float(mousepos.y)});
			}
			if(makingwire){
				sf::Vector2f pos = WiresArray->back().wire.getPosition();
				float angle = calcAngle(pos.x, pos.y, float(mousepos.x), float(mousepos.y));
				float width = WiresArray->back().wire.getSize().y;
				WiresArray->back().wire.setSize({calcDist(pos.x, pos.y, float(mousepos.x), float(mousepos.y)), width});
				WiresArray->back().wire.setRotation(angle);
			}
			if(makinggate){
				GatesArray->back().moveGate({float(mousepos.x), float(mousepos.y)}, *NodesArray);
			}
		break;
		case sf::Event::MouseButtonPressed:
			if(event.mouseButton.button == sf::Mouse::Left){
				if(makingnode){makingnode = false;break;}
				if(makinggate){makinggate=false;break;}
				
				int clicked = nodeClick(NodesArray, mousepos);
				if(clicked != -1 && (*NodesArray)[clicked].type == input){
					(*NodesArray)[clicked].switchState();
					UpdatePower();
					break;
				}

				clicked = gateButtonClick(GateButtons, mousepos);
				if(clicked != -1){
					GateButton& button = (*GateButtons)[clicked];
					Gate g(button.title, button.button, {float(mousepos.x), float(mousepos.y)}, window->getSize().x, button.input, button.output, GatesArray->size(), button.function, (*NodesArray));
					GatesArray->push_back(g);
					makinggate = true;
				}
				
			}
			else if(event.mouseButton.button == sf::Mouse::Middle){
				Node n(nodeType::mid, window->getSize().x, {float(mousepos.x), float(mousepos.y)}, -1);
				NodesArray->push_back(n);
			}
			else if(event.mouseButton.button == sf::Mouse::Right){
				int clicked = nodeClick(NodesArray, mousepos);
				if(clicked != -1 && !makingnode && !makingwire){
					if((*NodesArray)[clicked].type != output){
						makingwire = true;
						sf::Vector2f sp = (*NodesArray)[clicked].node.getPosition(); // Start position
						float angle = calcAngle(sp.x, sp.y, float(mousepos.x), float(mousepos.y));
						Wire w((*NodesArray)[clicked].state, clicked, window->getSize().x, sp, calcDist(sp.x, sp.y, mousepos.x, mousepos.y), angle);
						WiresArray->push_back(w);
						(*NodesArray)[clicked].outputWires.push_back(WiresArray->size()-1);
					}
				}
				else if(clicked != -1 && !makingnode && makingwire){
					if((*NodesArray)[clicked].type != input){
						makingwire = false;
						WiresArray->back().toNode = clicked;
						(*NodesArray)[clicked].inputWires.push_back(WiresArray->size()-1);
						sf::Vector2f pos = WiresArray->back().wire.getPosition();
						sf::Vector2f pos2 = (*NodesArray)[clicked].node.getPosition();
						float angle = calcAngle(pos.x, pos.y, pos2.x, pos2.y);
						float width = WiresArray->back().wire.getSize().y;
						WiresArray->back().wire.setSize({ calcDist(pos.x, pos.y, pos2.x, pos2.y), width });
						WiresArray->back().wire.setRotation(angle);
						UpdatePower();
					}
				}
			}
		break;
	}
}

void EventHandler::UpdatePower(){
	//Turn off everything except input Nodes

	for(int i = 0; i < WiresArray->size(); i++){
		(*WiresArray)[i].setState(false);
	}
	for(int i = 0; i < NodesArray->size(); i++){
		if((*NodesArray)[i].type != input){
			(*NodesArray)[i].setState(false);
		}
	}

	//This can be improved upon but it is what it is
	std::unordered_map<int, std::unordered_set<int>> graph;
	for(int i = 0; i < WiresArray->size(); i++){
		int from = (*WiresArray)[i].fromNode;
		int to = (*WiresArray)[i].toNode;
		graph[from].insert(to);
		graph[to].insert(from);
	}
	bool changed = false;

	do{
		changed = false;
		for(int i = 0; i < NodesArray->size(); i++){
			Node& curr = (*NodesArray)[i];
			if(curr.type == input){continue;}
			if(curr.type == gate_out){ continue; }

			if(curr.type == gate_in){
				bool prev = curr.state;
				curr.state = false;
				for(int neighbour : graph[i]){
					if((*NodesArray)[neighbour].type == output || (*NodesArray)[neighbour].type == gate_in){ continue; }
					curr.state = curr.state || (*NodesArray)[neighbour].state;
				}
				if(prev != curr.state){changed = true;curr.setState(curr.state);}
				std::vector<bool> prevStates;
				for(int j = 0; j < (*GatesArray)[curr.gateid].outputNodes.size(); j++){
					prevStates.push_back((*NodesArray)[(*GatesArray)[curr.gateid].outputNodes[j]].state);
				}
				(*GatesArray)[curr.gateid].calcOutput(NodesArray);
				for(int j = 0; j < (*GatesArray)[curr.gateid].outputNodes.size(); j++){
					if(prevStates[j] != ((*NodesArray)[(*GatesArray)[curr.gateid].outputNodes[j]].state)){changed=true;}
				}
				continue;
			}

			bool newstate = false;
			for(int neighbour : graph[i]){
				if((*NodesArray)[neighbour].type == output || (*NodesArray)[neighbour].type == gate_in){continue;}
				newstate = newstate || (*NodesArray)[neighbour].state;
			}
			if(curr.state != newstate){
				curr.setState(newstate);
				changed = true;
			}
			
		}
	}while(changed);

	//Update wires

	for(int i = 0; i < WiresArray->size(); i++){
		int from = (*WiresArray)[i].fromNode;
		(*WiresArray)[i].setState((*NodesArray)[from].state);
	}
}