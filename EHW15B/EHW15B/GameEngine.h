#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include <myo/myo.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <queue>
#include <sstream>
#include <iostream>
#include "Note.h"

class GameEngine
{
private:
	sf::RenderWindow* window;
	std::queue <Note> map;
	std::vector<Note> onScreen;
	sf::Font font;
	sf::Text scoretext;
	sf::Music music;
	long time;
	sf::Texture bgtx, sbtx, tltx, cltx, sltx, sctx;
	sf::Sprite background, scorebar, thresholdline, columns, selected, songcomplete;
	std::string title;
	std::string songfile;
	sf::Texture ot[4];
	int activeColumn;
	int score = 0;
	myo::Pose currentPose = myo::Pose::rest;
public:
	
	GameEngine(sf::RenderWindow *window)
	{
		//window->clear(sf::Color(255, 255, 255));

		if (!bgtx.loadFromFile("Images/Play-Screen-Background.png")) {
			std::cerr << "Could not load bg" << std::endl;
		}
		background.setTexture(bgtx);

		if (!sbtx.loadFromFile("Images/Score-Background.png")) {
			std::cerr << "Could not load scorebg" << std::endl;
		}
		scorebar.setTexture(sbtx);

		if (!tltx.loadFromFile("Images/Threshold-Line.png")) {
			std::cerr << "Could not load thresholdline" << std::endl;
		}
		thresholdline.setTexture(tltx);

		if (!cltx.loadFromFile("Images/Columns-Deselected.png")) {
			std::cerr << "Could not load columns" << std::endl;
		}
		columns.setTexture(cltx);

		if (!sltx.loadFromFile("Images/Columns-Selected.png")) {
			std::cerr << "Could not load selected" << std::endl;
		}
		selected.setTexture(sltx);

		if (!sctx.loadFromFile("Images/Song-Complete-Screen.png")) {
			std::cerr << "Could not load songcomplete" << std::endl;
		}
		songcomplete.setTexture(sctx);

		if (!font.loadFromFile("GOTHAM-ROUNDED-BOOK.OTF"))
		{
			std::cerr << "Could not load font" << std::endl;
		}
		if (!music.openFromFile("EpicSong.wav"))
		{
			std::cerr << "Could not load song" << std::endl;
		}

		//music.play();

		scoretext.setFont(font);
		//scoretext.setString("Hello world");
		scoretext.setCharacterSize(60);
		scoretext.setColor(sf::Color::White);
		//scoretext.setStyle(sf::Text::Bold | sf::Text::Underlined);
		scoretext.setPosition(600, 10);

		this->window = window;
		
		/*ot[0].loadFromFile("Images/fist.png");
		system("pause");*/

		if (!ot[0].loadFromFile("Images/fist.png")) {
			std::cerr << "Could not load fist" << std::endl;
		}
		
		if (!ot[1].loadFromFile("Images/spread.png")) {
			std::cerr << "Could not load spread" << std::endl;
		}
		if (!ot[2].loadFromFile("Images/in.png")) {
			std::cerr << "Could not load in" << std::endl;
		}
		if (!ot[3].loadFromFile("Images/out.png")) {
			std::cerr << "Could not load out" << std::endl;
		}
	}

	void reset()
	{
		music.stop();
		loadMap("EpicSong.txt");
		score = 0;
	}
	void setTime(long time)
	{
		this->time = time;
	}

	void draw()
	{
		window->draw(background);
		window->draw(columns);
		selected.setPosition(sf::Vector2f(3 + activeColumn * 340, 101));
		window->draw(selected);
		window->draw(thresholdline);
		if (onScreen.size() > 0)
		{
			for (int i = 0; i <= onScreen.size() - 1; i++)
			{
				//std::cout << onScreen[i].getPosition().x << " | " << onScreen[i].getPosition().y << std::endl;
				window->draw(onScreen[i]);
			}
		}

		if (isFinished())
		{
			window->draw(songcomplete);
		}

		window->draw(scorebar);
		std::ostringstream ss; //string buffer to convert numbers to string
		ss << score;
		scoretext.setString(ss.str());
		//std::cout << std::to_string(score);
		window->draw(scoretext);
	}

	void killNotes()
	{
		if (onScreen.size() > 0 && time - 650 > onScreen[onScreen.size() - 1].getTime())
		{
			onScreen.pop_back();
		}
		//system("pause");
	}
	void loadNotes()
	{
		if (map.empty() == false && time + 2500 >= map.front().getTime())
		{
			auto it = onScreen.begin();
			it = onScreen.insert(it, map.front());
			map.pop();
			//std::cout << onScreen.size();
		}
	}

	void setNotePositions()
	{
		if (onScreen.size() > 0)
		{
			for (int i = 0; i <= onScreen.size() - 1; i++)
			{
				onScreen[i].setPosition(sf::Vector2f(173 + onScreen[i].getColumn() * 340, 667 - (onScreen[i].getTime() - time) / 3));
				onScreen[i].setScale((float)(0.1+0.2*(2000 - abs(time - onScreen[i].getTime())) / 4000), (float)(0.1+0.2*(2000 - abs(time - onScreen[i].getTime())) / 4000));
				//onScreen[i].setPosition(sf::Vector2f(0,0));
				//std::cout << onScreen[i].getPosition().x << std::endl;
			}
		}
	}

	Note getNextNote()
	{
		if (onScreen.size() > 0)
		{
			return onScreen.back();
		}
	}

	int getActiveColumn()
	{
		return activeColumn;
	}

	void setActiveColumn(DataCollector& collector, int cYaw)
	{
		if (abs((cYaw - 60) - collector.yaw_w) < 20 || abs((cYaw - 60 + 960) - collector.yaw_w) < 20)
		{
			activeColumn = 3;
		}
		else if (abs((cYaw - 20) - collector.yaw_w) < 20 || abs((cYaw - 20 + 960) - collector.yaw_w) < 20)
		{
			activeColumn = 2;
		}
		else if (abs((cYaw + 20) - collector.yaw_w) < 20 || abs((cYaw + 20 + 960) - collector.yaw_w) < 20)
		{
			activeColumn = 1;
		}
		else if (abs((cYaw + 60) - collector.yaw_w) < 20 || abs((cYaw + 60 + 960) - collector.yaw_w) < 20)
		{
			activeColumn = 0;
		}
	}

	int getScore()
	{
		return score;
	}

	void tryHitNext(DataCollector collector)
	{
		if (onScreen.size() > 0)
		{
			myo::Pose previousPose = currentPose;
			currentPose = collector.getPose();
			if (onScreen.back().state == 1)
			{
				if (currentPose != previousPose && currentPose != myo::Pose::rest)
				{
					//std::cout << currentPose.toString() << " " << previousPose.toString() << "";
					//std::cout << activeColumn << " " << onScreen.back().getColumn() << std::endl;
					if (currentPose == onScreen.back().getPose() && activeColumn == onScreen.back().getColumn())
					{
						
						if (abs(time - onScreen.back().getTime()) < 125)
						{
							score += 300;
							onScreen.back().setState(2);
							onScreen.back().setColor(sf::Color(144, 255, 144));
						}
						else if (abs(time - onScreen.back().getTime()) < 250)
						{
							score += 200;
							onScreen.back().setState(2);
							onScreen.back().setColor(sf::Color(144, 255, 144));
						}
						else if (abs(time - onScreen.back().getTime()) < 500)
						{
							score += 100;
							onScreen.back().setState(2);
							onScreen.back().setColor(sf::Color(144, 255, 144));
						}
						
						//std::cout << score << std::endl;
					}
					/*else
					{
					return 0;
					}*/
				}
			}
		}
		//std::cout << score << std::endl;
	}

	void setNoteStates()
	{
		if (onScreen.size() > 0)
		{
			for (int i = 0; i <= onScreen.size() - 1; i++)
			{
				if (abs(onScreen[i].getTime() - time) <= 700 && onScreen[i].state == 0)
				{
					onScreen[i].state = 1;
				}
				else if (onScreen[i].state == 1 && time - onScreen[i].getTime() > 500)
				{
					onScreen[i].state = 2;
					//onScreen[i].setColor(sf::Color(255, 128, 128));
				}
			}
		}
	}

	int checkDone(DataCollector& collector)
	{
		if (myo::Pose::fingersSpread == collector.getPose())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	void playMusic()
	{
		music.play();
	}

	bool isFinished()
	{
		return (map.empty() && onScreen.empty());
	}

	void update(long t)
	{
		setTime(t);
		killNotes();
		loadNotes();
		setNoteStates();
		setNotePositions();
	}
	
	void loadMap(std::string fileName)
	{
		std::ifstream mapReader(fileName);
		if (!mapReader) {
			std::cerr << "Could not load map" << std::endl;
		}
		getline(mapReader, title);
		getline(mapReader, songfile);
		long t;
		int c, p;
		myo::Pose pp;

		mapReader >> t >> c >> p;
		while (!mapReader.fail())
		{		
			if (p == 0) // fist, spread, in, out
			{
				pp = myo::Pose::fist;
			}
			else if (p == 1)
			{
				pp = myo::Pose::fingersSpread;
			}
			else if (p == 2)
			{
				pp = myo::Pose::waveIn;
			}
			else if (p == 3)
			{
				pp = myo::Pose::waveOut;
			}
			else
			{
				//die
			}
			Note newNote(t, c, pp, ot[p]);
			map.push(newNote);
			mapReader >> t >> c >> p;
		}
		mapReader.close();
	}
	std::string getTitle()
	{
		return title;
	}

	std::string getSongFile()
	{
		return songfile;
	}

	void spitOutMap()
	{
		while (!map.empty())
		{
			std::cout << map.front().getTime() << std::endl;
			map.pop();
		}
	}
};
#endif