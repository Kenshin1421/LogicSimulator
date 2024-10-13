#include "Renderer.h"

void Renderer::Render(){
	window->clear(bgColor);
	window->draw(border);
	for(int i = 0; i < WiresArray->size(); i++){
		window->draw((*WiresArray)[i].wire);
	}
	for(int i = 0; i < NodesArray->size(); i++){
		window->draw((*NodesArray)[i].node);
	}
	for(int i = 0; i < GatesArray->size(); i++){
		window->draw((*GatesArray)[i].gate);
		window->draw((*GatesArray)[i].title);
	}
	for(int i = 0; i < GateButtons->size(); i++){
		window->draw((*GateButtons)[i].button);
		window->draw((*GateButtons)[i].title);
	}
	window->display();
}

float Renderer::getBordSize(){
	return -1*border.getOutlineThickness();
}

void Renderer::loadGates(){
	bool first = true;
	for(const auto& dirEntry : std::filesystem::recursive_directory_iterator("Gates")){
		std::fstream myfile(dirEntry.path(), std::ios::in);
		if(myfile.is_open()){
			std::string title;
			std::string inp;
			std::string out;
			std::string temp;
			std::vector<std::string> data;
			std::getline(myfile, title);
			std::getline(myfile, inp);
			std::getline(myfile, out);
			while(getline(myfile, temp)){
				data.push_back(temp);
			}

			if(first){
				GateButton g(title, textFont, win_X, { getBordSize(), win_Y - getBordSize() / 2 }, std::stoi(inp), std::stoi(out), data);
				GateButtons->push_back(g);
				first = false;
			}
			else{
				float xPos = GateButtons->back().button.getPosition().x + (GateButtons->back().button.getSize().x*GateButtons->back().button.getScale().x)/2;
				GateButton g(title, textFont, win_X, {xPos + win_X/100, win_Y - getBordSize() / 2 }, std::stoi(inp), std::stoi(out), data);
				GateButtons->push_back(g);
			}
		}
	}
}