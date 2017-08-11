//made by anonymouse#1643, read readme.md for more info and howto
#include <iostream> //cout
#include <random> //normal_distribution
#include <chrono> //system_clock
#include <string> //better than c-style, or so says SO
#include <fstream> //read from contestantdata.txt, Say
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
    Contestant() //this default should never be in the array for long
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
    }
    void Randomize(bool singleMode) //randomize the score. should probably move the cout to main but im lazy
    {
        std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
        std::normal_distribution<double> normDist(avgRS, stDev);
        score = normDist(generator);
        if(singleMode) std::cout << name << " got " << score << ".\n";
    }
    void Reset() //between games
    {
        alive = true;
        score = 0;
    }
};

void Say(std::string msg, std::ofstream &file)
{
    std::cout << msg;
    file << msg;
}

Contestant *GetWinner(Contestant *contestants, int contestantNum) //if there's only one contestant where alive=true, return a pointer to it
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
    std::ofstream file;
    file.open("output.txt");
    bool singleMode = false;
    int sims = 1;
    int simsS = 1;
    int contestantNum = 10;
    int currentRoundS = 1;
    int currentRound = 0;
    int numRounds = 0;
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
    simsS = sims;
    if (sims <= 5) singleMode = true; //i might make the user be able to toggle this
    for (double i = contestantNum; i > 1; ++numRounds) //how many rounds are there?
    {
        int kill = round(i * 3 / 20);
        if (kill == 0) kill = 1;
        i -= kill;
    }
    Contestant *contestants = ParseContestantData(contestantNum, numRounds);
    Contestant *winner = nullptr;
    for (; sims > 0; --sims) //you think this is bad? I could have used while(sims-->0). be thankful.
    {
        winner = nullptr;
        while (winner == nullptr)
        {
            if (singleMode) std::cout << "Round " << currentRound + currentRoundS << " start!\n"; //eventually I'll replace these couts with my own function
            for (int i = 0; i < contestantNum; ++i)
            {
                if (contestants[i].alive)
                {
                    contestants[i].Randomize(singleMode);
                    if (contestants[i].prized) //this is for double response prizes. should probably clean this up a bit
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
            int killnum = round((double)(alivenum) * 3 / 20); //how many to kill... maybe make death rate customizable in the future? for now, 15%
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
            for (int i = contestantNum - 1; prizenum > 0; --i) //who gets a prize? YOU get a prize!
            {
                if (contestants[i].alive)
                {
                    contestants[i].prized = true;
                    if (singleMode) std::cout << contestants[i].name << " gets a DRP!\n";
                    --prizenum;
                }
            }
            for (int i = 0; i < contestantNum; ++i) //update survival rates to new data
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
            if (sims % 1000 == 0) //because the sims go too fast to cls every sim
            {
                system("cls");
                std::cout << 100 * ((double)(simsS - sims) / simsS) << "% complete.\n"; //because sometimes, when waiting an hour for sims to complete, you just want to know how far you are...
            }
        }
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
    file.close();
    system("pause");
    return 0;
}