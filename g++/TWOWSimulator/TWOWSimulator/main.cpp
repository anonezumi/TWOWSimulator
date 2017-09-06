//made by anonymouse#1643, read readme.md for more info and howto
#include <iostream> //cout
#include <random> //normal_distribution
#include <chrono> //system_clock
#include <string> //better than c-style, or so says SO
#include <fstream> //read from contestantdata.txt
#include <stdlib.h> //system()
#include <math.h> //round()

enum Twist {NONE, JS3};

void Say(std::string msg, std::ofstream &file, int delay = 0)
{
    std::cout << msg;
	if (delay == 0) file << msg;
    else
	{
		std::string zeroes = "";
		if(delay % 1000 == delay)
		{
			if(delay % 100 == delay)
			{
				if(delay % 10 == delay) zeroes = "000";
				else zeroes = "00";
			}
			else zeroes = "0";
		}
		file << zeroes << delay << " " << msg;
	}
}

struct Contestant
{
    double avgRS;
    double stDev;
    double score;
    bool alive;
    bool prized;
    int wins;
    std::string name;
    int *rounds;
    int numRounds;
	int lives;
	int points;
    Contestant() //this default should never be in the array for long
    {
        avgRS = 50;
        stDev = 10;
        name = "DefaultContestant";
        score = 0;
        wins = 0;
        alive = true;
        prized = false;
        rounds = NULL;
		lives = 2;
		points = 0;
    }
    Contestant(double pavgRS, double pstDev, int pnumRounds, std::string pname) //this is the only constructor I call
    {
        avgRS = pavgRS;
        stDev = pstDev;
        name = pname;
        score = 0;
        wins = 0;
        alive = true;
        prized = false;
        numRounds = pnumRounds;
        rounds = new int[pnumRounds];
        for (int i = 0; i < pnumRounds; ++i)
        {
            rounds[i] = 0;
        }
		lives = 2;
		points = 0;
    }
    void Randomize() //randomize the score. should probably move the cout to main but im lazy
    {
        std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
        std::normal_distribution<double> normDist(avgRS, stDev);
        score = normDist(generator);
    }
    void Reset() //between games
    {
        alive = true;
        score = 0;
		points = 0;
		lives = 2;
    }
};

Contestant *GetWinner(Contestant *contestants, int contestantNum) //if there's only one contestant where alive=true, return a pointer to it
{
    Contestant *winner = NULL;
    for (int i = 0; i < contestantNum; ++i)
    {
        if (contestants[i].alive)
        {
            if (winner == NULL) winner = &contestants[i];
            else return NULL;
        }
    }
    return winner;
}

void ContestantBubbleSort(Contestant *contestants, int contestantNum) //sorts contestants by score, ascending
{
    bool changed = true;
    Contestant temp;
    while (changed)
    {
        changed = false;
        for (int i = 0; i < contestantNum - 1; ++i)
        {
            if (contestants[i].score > contestants[i + 1].score)
            {
                temp = contestants[i];
                contestants[i] = contestants[i + 1];
                contestants[i + 1] = temp;
                changed = true;
            }
        }
    }
}

Contestant *ParseContestantData(int contestantNum, int numRounds) //reads contestant data from file
{
    Contestant *contestants = new Contestant[contestantNum];
    std::ifstream data;
    data.open("contestantdata.txt");
    for (int i = 0; i < contestantNum; ++i)
    {
        std::string name;
        std::getline(data, name);
        std::string temp;
        std::getline(data, temp);
        double avgRS = 50;
		avgRS = std::stod(temp);
        std::getline(data, temp);
        double stDev = 15;
		stDev = std::stod(temp);
        contestants[i] = Contestant(avgRS, stDev, numRounds, name);
        std::getline(data, temp);
    }
    data.close();
    return contestants;
}

int main(void)
{
    std::ofstream file;
    file.open("output.txt");
    bool singleMode = false;
	int PS2Mode = 0;
    int sims = 1;
    int simsS = 1;
    int contestantNum = 10;
    int currentRoundS = 1;
    int currentRound = 0;
    int numRounds = 0;
	int lbMode = 0;
	Twist twist = NONE;
    std::cout << "Type the current round number.\n"; //maybe remove the requirement of input outside of contestantdata.txt? we'll see
    try
    {
        std::cin >> currentRoundS;
    }
    catch (int e)
    {
        std::cout << "There was an error. Let's just assume it's round 1.\n";
        currentRoundS = 1;
    }
    std::cout << "Type the number of contestants. This should not exceed the amount of contestants stored in contestantdata.txt.\n";
    try
    {
        std::cin >> contestantNum;
    }
    catch (int e)
    {
        std::cout << "There was an error. Let's just assume it's 10.\n";
        contestantNum = 1;
    }
    std::cout << "Type the number of simulations to perform.\n";
    try
    {
		std::cin >> sims;
    }
    catch (int e)
    {
        std::cout << "There was an error. Let's just assume it's 1.\n";
        sims = 1;
    }
	int temp = 0;
    std::cout << "What twist should the TWOW have?\n0 - No Twist\n1 - JPomz S3: Lives\n";
    try
    {
        std::cin >> temp;
		twist = static_cast<Twist>(temp);
    }
    catch (int e)
    {
        std::cout << "There was an error. Let's just assume it's no twist.\n";
        twist = NONE;
    }
    std::cout << "Format the results in the file for PS2Bot syntax? [l/y/n]\n";
	char yn = 'n';
    try
    {
        std::cin >> yn;
    }
    catch (int e)
    {
        std::cout << "There was an error. Let's just assume it's no.\n";
    }
	if(yn == 'y') PS2Mode = 1;
	else if(yn == 'l')
	{
		PS2Mode = 1;
		lbMode = 1;
	}
    simsS = sims;
    if (sims <= 5) singleMode = true; //i might make the user be able to toggle this
	switch(twist)
	{
	case NONE:
		for (double i = contestantNum; i > 1; ++numRounds) //how many rounds are there?
		{
			int kill = round(i / 5);
			if (kill == 0) kill = 1;
			i -= kill;
		}
		break;
	case JS3: numRounds = ceil((double)(contestantNum + 16) / 2);
	}
		
    Contestant *contestants = ParseContestantData(contestantNum, numRounds);
    Contestant *winner = NULL;
    for (; sims > 0; --sims) //you think this is bad? I could have used while(sims-->0). be thankful.
    {
        winner = NULL;
        while (winner == NULL)
        {
            if (singleMode)
			{
				Say("--------------------\n", file, PS2Mode * 1000);
				Say("Round " + std::to_string(currentRound + currentRoundS) + " start!\n", file, PS2Mode * 500);
			}
            int alivenum = 0;
            for (int i = 0; i < contestantNum; ++i)
            {
                if (contestants[i].alive)
                {
                    ++alivenum;
                }
            }
            for (int i = 0; i < contestantNum; ++i)
            {
                if (contestants[i].alive)
                {
                    contestants[i].Randomize();
					if (singleMode) Say(contestants[i].name + " got " + std::to_string(contestants[i].score) + "%.\n", file, PS2Mode * 20000/alivenum);
                    if (contestants[i].prized) //this is for double response prizes. should probably clean this up a bit
                    {
                        double temp = contestants[i].score;
                        if (singleMode) Say(contestants[i].name + " also had a DRP!\n", file, PS2Mode * 10000/alivenum);
						contestants[i].Randomize();
						if (singleMode) Say(contestants[i].name + " also got " + std::to_string(contestants[i].score) + "%.\n", file, PS2Mode * 20000/alivenum);
                        if (temp > contestants[i].score) contestants[i].score = temp;
                        contestants[i].prized = false;
                    }
                }
            }
            ContestantBubbleSort(contestants, contestantNum);
			int killnum = 1;
			switch(twist)
			{
			case NONE:
				killnum = round((double)(alivenum) / 5);
				if (killnum == 0) killnum = 1;
				for (int i = 0; killnum > 0; ++i)
				{
					if (contestants[i].alive)
					{
						contestants[i].alive = false;
						if (singleMode) Say(contestants[i].name + " has died.\n", file, PS2Mode * 10000/alivenum);
						--killnum;
					}
				}
				break;
			case JS3:
				killnum = round((double)(alivenum) * 1/3); //lose 1 life for 1/3- NR.
				if (killnum == 0) killnum = 1;
				for (int i = 0; killnum > 0; ++i)
				{
					if (contestants[i].alive)
					{
						--contestants[i].lives;
						if (singleMode) Say(contestants[i].name + " has lost a life. They have " + std::to_string(contestants[i].lives) + " lives left.\n", file, PS2Mode * 10000/alivenum);
						--killnum;
					}
				}
				killnum = round((double)(alivenum) * 1/6); //lose 2 lives for 1/6- NR.
				if (killnum == 0) killnum = 1;
				for (int i = 0; killnum > 0; ++i)
				{
					if (contestants[i].alive)
					{
						--contestants[i].lives;
						if (singleMode) Say(contestants[i].name + " has lost another life. They have " + std::to_string(contestants[i].lives) + " lives left.\n", file, PS2Mode * 10000/alivenum);
						--killnum;
					}
				}
				break;
			}
            int prizenum = round((double)(alivenum) / 10);
            for (int i = contestantNum - 1; prizenum > 0; --i) //who gets a prize? YOU get a prize!
            {
                if (contestants[i].alive)
                {
                    contestants[i].prized = true;
                    if (singleMode) Say(contestants[i].name + " gets a DRP!\n", file, PS2Mode * 10000/alivenum);
                    --prizenum;
                }
            }
			int ranking = 0; //only used in JS3, but it needs to be declared in this scope
            for (int i = 0; i < contestantNum; ++i) //update survival rates to new data
            {
                if (contestants[i].alive) contestants[i].rounds[currentRound]++;
				switch(twist)
				{
				case JS3:
					if (contestants[i].alive)
					{
						contestants[i].rounds[currentRound]++;
						contestants[i].points += ceil(((double)(ranking)/((double)(alivenum)-1))*50);
						if(contestants[i].points >= 100)
						{
							contestants[i].points -= 100;
							if(contestants[i].lives < 4)
							{
								++contestants[i].lives;
								if (singleMode) Say(contestants[i].name + " has regained a life! They now have " + std::to_string(contestants[i].lives) + " lives.\n", file, PS2Mode * 10000/alivenum);
							}
						}
						if (contestants[i].lives <= 0) 
						{
							contestants[i].alive = false;
							if (singleMode) Say(contestants[i].name + " has died.\n", file, PS2Mode * 10000/alivenum);
						}
						++ranking;
					}
				}
            }
            ++currentRound;
            winner = GetWinner(contestants, contestantNum);
			if(lbMode == 1)
			{
				file << "0000 [STARTLB]\n";
				for(int i = 1; i <= alivenum; ++i)
				{
					file << "0000 " << contestants[contestantNum - i].name << "\n0000 " << contestants[contestantNum - i].score << "\n0000 ";
					if(i == alivenum) file << "[ENDLB]";
					file << "\n";
				}
			}
        }
        for (int i = 0; i < contestantNum; ++i)
        {
            contestants[i].Reset();
        }
        if (singleMode) Say("The winner is: " + winner->name + "!\n", file, PS2Mode * 4000);
        else winner->wins++;
        currentRound = 0;
    }
    if (!singleMode)
    {
        double survPercent = 0;
        for (int i = 0; i < numRounds - 1; ++i)
        {
            Say("----------ROUND " + std::to_string(i + currentRoundS) + "----------\n", file);
            for (int j = 0; j < contestantNum; ++j)
            {
                survPercent = ((double)(contestants[j].rounds[i])/(double)(simsS)) * 100;
                Say(contestants[j].name + " survived round " + std::to_string(i + currentRoundS) + " " + std::to_string(survPercent) + "% of the time.\n", file);
            }
        }
        Say("----------WINS----------\n", file);
        for (int i = 0; i < contestantNum; ++i)
        {
            survPercent = ((double)(contestants[i].wins) / (double)(simsS)) * 100;
            Say(contestants[i].name + " won " + std::to_string(survPercent) + "% of the time.\n", file);
        }
    }
    delete[] contestants;
    std::cout << "Simulation complete. Enter anything to exit.";
	std::string temp1;
	std::cin >> temp1;
    return 0;
}
