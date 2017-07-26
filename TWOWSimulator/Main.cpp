//made by anonymouse#1643, read readme.txt for more info and howto
#include <iostream>
#include <random>
#include <chrono>
#include <string>
#include <fstream>
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
    Contestant()
    {
        avgRS = 50;
        stDev = 10;
        name = "DefaultContestant";
        score = 0;
        wins = 0;
        alive = true;
        prized = false;
        rounds = nullptr;
    }
    Contestant(double pavgRS, double pstDev, int pnumRounds, std::string pname)
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
    }
    void Randomize(bool singleMode)
    {
        std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
        std::normal_distribution<double> normDist(avgRS, stDev);
        score = normDist(generator);
        if(singleMode) std::cout << name << " got " << score << ".\n";
    }
    void Reset()
    {
        alive = true;
        score = 0;
    }
};

Contestant *GetWinner(Contestant *contestants, int contestantNum)
{
    Contestant *winner = nullptr;
    for (int i = 0; i < contestantNum; ++i)
    {
        if (contestants[i].alive)
        {
            if (winner == nullptr) winner = &contestants[i];
            else return nullptr;
        }
    }
    return winner;
}

void ContestantBubbleSort(Contestant *contestants, int contestantNum)
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

Contestant *ParseContestantData(int contestantNum, int numRounds)
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
        try
        {
            avgRS = std::stod(temp);
        }
        catch (int e)
        {
            std::cout << "Error " << e << " in ParseContestantData. Make sure contestantdata.txt is formatted correctly.\n";
        }
        std::getline(data, temp);
        double stDev = 15;
        try
        {
            stDev = std::stod(temp);
        }
        catch (int e)
        {
            std::cout << "Error " << e << " in ParseContestantData. Make sure contestantdata.txt is formatted correctly.\n";
        }
        contestants[i] = Contestant(avgRS, stDev, numRounds, name);
        std::getline(data, temp);
    }
    data.close();
    return contestants;
}

int main(void)
{
    bool singleMode = false;
    int sims = 1;
    int simsS = 1;
    int contestantNum = 10;
    int currentRoundS = 1;
    int currentRound = 0;
    int numRounds = 0;
    std::cout << "Type the current round number.\n";
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
    simsS = sims;
    if (sims <= 5) singleMode = true;
    for (double i = contestantNum; i > 1; ++numRounds)
    {
        int kill = round(i * 3 / 20);
        if (kill == 0) kill = 1;
        i -= kill;
    }
    Contestant *contestants = ParseContestantData(contestantNum, numRounds);
    Contestant *winner = nullptr;
    for (; sims > 0; --sims)
    {
        winner = nullptr;
        while (winner == nullptr)
        {
            if (singleMode) std::cout << "Round " << currentRound + currentRoundS << " start!\n";
            for (int i = 0; i < contestantNum; ++i)
            {
                if (contestants[i].alive)
                {
                    contestants[i].Randomize(singleMode);
                    if (contestants[i].prized)
                    {
                        double temp = contestants[i].score;
                        if(singleMode) std::cout << contestants[i].name << " also had a DRP!\n";
                        contestants[i].Randomize(singleMode);
                        if (temp > contestants[i].score) contestants[i].score = temp;
                        contestants[i].prized = false;
                    }
                }
            }
            ContestantBubbleSort(contestants, contestantNum);
            int alivenum = 0;
            for (int i = 0; i < contestantNum; ++i)
            {
                if (contestants[i].alive)
                {
                    ++alivenum;
                }
            }
            int killnum = round((double)(alivenum) * 3 / 20);
            if (killnum == 0) killnum = 1;
            for (int i = 0; killnum > 0; ++i)
            {
                if (contestants[i].alive)
                {
                    contestants[i].alive = false;
                    if (singleMode) std::cout << contestants[i].name << " has died.\n";
                    --killnum;
                }
            }
            int prizenum = round((double)(alivenum) / 10);
            if (prizenum == 0) prizenum = 1;
            for (int i = contestantNum - 1; prizenum > 0; --i)
            {
                if (contestants[i].alive)
                {
                    contestants[i].prized = true;
                    if (singleMode) std::cout << contestants[i].name << " gets a DRP!\n";
                    --prizenum;
                }
            }
            for (int i = 0; i < contestantNum; ++i)
            {
                if (contestants[i].alive) contestants[i].rounds[currentRound]++;
            }
            ++currentRound;
            winner = GetWinner(contestants, contestantNum);
        }
        for (int i = 0; i < contestantNum; ++i)
        {
            contestants[i].Reset();
        }
        if (singleMode) std::cout << "The winner is: " << winner->name << "\n";
        else
        {
            winner->wins++;
            if (sims % 1000 == 0)
            {
                system("cls");
                std::cout << 100 * ((double)(simsS - sims) / simsS) << "% complete.\n";
            }
        }
        currentRound = 0;
    }
    if (!singleMode)
    {
        double survPercent = 0;
        for (int i = 0; i < numRounds - 1; ++i)
        {
            std::cout << "----------ROUND " << i + currentRoundS << "----------\n";
            for (int j = 0; j < contestantNum; ++j)
            {
                survPercent = ((double)(contestants[j].rounds[i])/(double)(simsS)) * 100;
                std::cout << contestants[j].name << " survived round " << i + currentRoundS << " " << survPercent << "% of the time.\n";
            }
        }
        std::cout << "----------WINS----------\n";
        for (int i = 0; i < contestantNum; ++i)
        {
            survPercent = ((double)(contestants[i].wins) / (double)(simsS)) * 100;
            std::cout << contestants[i].name << " won " << survPercent << "% of the time.\n";
        }
    }
    delete[] contestants;
    system("pause");
    return 0;
}