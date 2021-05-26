
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <bits/stdc++.h>

class Cell{

    private:
        float amtA;
        float amtB;
        float posX;
        float posY;

        //sf::RectangleShape shape = sf::RectangleShape();


    public:
        Cell(float amtA_l, float amtB_l, float x, float y){
            this->amtA = amtA_l;
            this->amtB = amtB_l;
            this->posX = x;
            this->posY = y;
            //this->shape.setFillColor(sf::Color::White);
            //this->shape.setPosition(sf::Vector2f(this->posX,this->posY));
        }


        float getAmtA(){
            return this->amtA;
        }
        float getAmtB(){
            return this->amtB;
        }


        void setAmtA(float val){
            this->amtA = val;
        }

        void setAmtB(float val){
            this->amtB = val;
        }

        void changeAB(float dA, float dB, std::pair<float,float> laplacian, float f, float k, float timestep){
            const float A = this->amtA;
            const float B = this->amtB;

            this->amtA = A + (dA * laplacian.first - A * B * B + f * (1-A)) * timestep;
            this->amtB = B + (dB * laplacian.second + A * B * B - (k+f) * B) * timestep;

            this->amtA = std::max(0.f,std::min(1.f,this->amtA));
            this->amtB = std::max(0.f,std::min(1.f,this->amtB));
        }
        

        /*
        sf::RectangleShape getShape(){
            return this->shape;
        }*/

};