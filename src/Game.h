#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#include <random>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


#include "Cell.h"
/*
*
* Game engine
*
*
*/

int N = 200;

class Game {
    private:
        //vars
        sf::RenderWindow* window;
        sf::Event ev;
        sf::VideoMode videoMode;
        sf::Color bg_color;
        sf::View view;

        //Mouse
        sf::Vector2f mousePosWindow;
        
        //sf::VertexArray rays = sf::VertexArray(sf::LineStrip, 2);
        
        //Game objects
            std::vector<std::vector<Cell>> grid; 

        //drawing variables
            sf::CircleShape s = sf::CircleShape();

            
        //constants
        const float feed = 0.055f;
        const float kill = 0.062f;
        const float dA = 1.f;
        const float dB = 0.5f;
        const float timestep = 1.f;
        //initialise basics

        void initVariables(){
            this->window = nullptr;
            this->bg_color = sf::Color(0,0,0,255);
            this->videoMode.height = 800;
            this->videoMode.width = 800;



        }

        void initWindow(){
            this->window = new sf::RenderWindow(this->videoMode, "My first game", 
	        sf::Style::Titlebar | sf::Style::Close);
            this->window->setFramerateLimit(60);
            this->view = sf::View(sf::FloatRect(0,0,N,N));
            this->window->setView(this->view);
        }

        void initStructures(){
            for(int i = 0; i < N; i++){
                std::vector<Cell> row;
                for(int j = 0; j < N; j++){
                    Cell cell = Cell(1,0,j,i);
                    row.push_back(cell); 
                }
                grid.push_back(row);
            }

            for(int i = 50; i < 55; i++){
                for(int j = 50; j < 55; j++){
                   grid[i][j].setAmtB(1);
                }
                
 

            }


        }


        void initShapes(){
        }
    
    public:
        Game(){
            this->initVariables();
            this->initWindow();
            this->initStructures();
            this->initShapes();
        }

        virtual ~Game(){
            delete this->window;
        }

        //Accessors

        const bool running() const{
            return this->window->isOpen();
        }

        void updateMousePositions(){
            this->mousePosWindow = sf::Vector2f(sf::Mouse::getPosition(*this->window)); 
        }

        void drawGrid(){
            sf::Sprite sprite;
            sf::Texture texture;
            sf::Image image = sf::Image();
            image.create(this->videoMode.width,this->videoMode.height, sf::Color::Black);
            sf::Uint8 *pixels = new sf::Uint8[this->videoMode.width * this->videoMode.height * 4]; 
            for(int x = 0; x < N; x++){
                for(int y = 0; y < N; y++){
                    float valA = grid[y][x].getAmtA() *255;
                    float valB = grid[y][x].getAmtB() *255;
                    float c = std::max(0.f,std::min(255.f,valA - valB));
                    pixels[(x + y *this->videoMode.width)*4] = c; 
                    pixels[(x + y *this->videoMode.width)*4 + 1] = c;
                    pixels[(x + y *this->videoMode.width)*4 + 2] = c;
                    pixels[(x + y *this->videoMode.width)*4 + 3] = 255;
                   
                    /*
                    float alpha = grid[y][x].getAmtA();
                    if(alpha > 255){
                        alpha = 255;
                    }
                    */

                    //pixels[(x + y *this->videoMode.width)*4 + 3] = alpha;

                    
                    /*if(grid[y][x].getAmtA() > 0){
                        pixels[(x + y *this->videoMode.width)*4 + 3] = 255;

                    }else {
                        pixels[(x + y *this->videoMode.width)*4 + 3] = 0;

                    }*/

                }

            }
            image.create(this->videoMode.width, this->videoMode.height,pixels);
            texture.loadFromImage(image);
            sprite.setTexture(texture);
            this->window->draw(sprite);
            delete pixels;
        }

        std::pair<float,float> calculateLaplacianForCell(int x, int y){
            float sumA = -1 * grid[y][x].getAmtA();
            float sumB = -1 * grid[y][x].getAmtB();
            sumA += 0.05 * grid[y+1][x+1].getAmtA();
            sumB += 0.05 * grid[y+1][x+1].getAmtB();
            sumA += 0.05 * grid[y-1][x-1].getAmtA();
            sumB += 0.05 * grid[y-1][x-1].getAmtB();
            sumA += 0.05 * grid[y+1][x-1].getAmtA();
            sumB += 0.05 * grid[y+1][x-1].getAmtB();
            sumA += 0.05 * grid[y-1][x+1].getAmtA();
            sumB += 0.05 * grid[y-1][x+1].getAmtB();
            sumA += 0.2 * grid[y+1][x].getAmtA();
            sumB += 0.2 * grid[y+1][x].getAmtB();
            sumA += 0.2 * grid[y-1][x].getAmtA();
            sumB += 0.2 * grid[y-1][x].getAmtB();
            sumA += 0.2 * grid[y][x+1].getAmtA();
            sumB += 0.2 * grid[y][x+1].getAmtB();
            sumA += 0.2 * grid[y][x-1].getAmtA();
            sumB += 0.2 * grid[y][x-1].getAmtB();



            return std::make_pair(sumA,sumB);
        }



        void drawShapes(){
            //this->window->draw(this->s);
            drawGrid();            




            /*for(auto row : this->grid){
                for(auto cell : row){
                }
            }*/

        }



        void pollEvents(){
            while(this->window->pollEvent(this->ev)){
                
                switch(this->ev.type){
                    
                    case sf::Event::Closed:
                        this->window->close();
                        break;
                    
                    case sf::Event::KeyPressed:
                        if(this->ev.key.code == sf::Keyboard::Escape){
                            this->window->close();
                        }
                        break;
                    
                }
            }
        };

        void updateGridState(){
            
            auto grid_copy = grid;

            for(int y = 1; y < grid.size()-1; y++){
                for(int x = 1; x < grid[y].size()-1; x++){
                    


                    
                    grid_copy[y][x].changeAB(this->dA, this->dB, calculateLaplacianForCell(x,y), this->feed, this->kill, this->timestep);
                }
            } 
            grid = grid_copy;

        }



        //update and rendering

        void update(){
            this->pollEvents();
            this->updateGridState();
            
            //mouse pos
            this->updateMousePositions();
            //relative to the screen
            //this->s.setPosition(s.getPosition().x + 10, s.getPosition().y+10);

        };

        void render(){
            this->window->clear(this->bg_color);
            //draw game
            this->drawShapes();
            //----
            this->window->display();
        };


};