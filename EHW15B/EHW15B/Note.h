#ifndef NOTE_H
#define NOTE_H
#include <myo/myo.hpp>
#include <SFML/Graphics.hpp>

class Note : public sf::Sprite
{
private:
	long time; //ms
	int column; //0123
	myo::Pose pose;
public:
	Note(long time, int column, myo::Pose pose, sf::Texture& texture)
	{
		this->time = time;
		this->column = column;
		this->pose = pose;
		this->setTexture(texture);
		this->setScale(sf::Vector2f(0.2, 0.2));
		this->setOrigin(500, 500);
	}
	
	long getTime()
	{
		return time;
	}

	int getColumn()
	{
		return column;
	}

	myo::Pose getPose()
	{
		return pose;
	}
};
#endif