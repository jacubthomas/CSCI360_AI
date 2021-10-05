#include "PuzzleGenerator.h"
#include <limits>
#include <cmath>

using namespace std;

PuzzleGenerator::PuzzleGenerator(int _nRows, int _nColumns, int _minVal, int _maxVal)
    :nRows(_nRows), nColumns(_nColumns), minVal(_minVal), maxVal(_maxVal)
{}

Puzzle PuzzleGenerator::GeneratePuzzle()
{
    timer.StartTimer();
    maxtime = 59.9;    // To make sure we don't exceed a minute
    
    double random_walk_time = 5;    // 5 seconds.
    
//    return RandomWalk(random_walk_time);    // Do a random walk for some time and return the solution.
    return hillClimb(random_walk_time);

}

Puzzle PuzzleGenerator::RandomWalk(double timelimit)
{
    // A very simple function that start at a random configuration and keeps randomly modifying it
    // until t hits the time limit. Returns the best solution found so far.

    Puzzle p(nRows, nColumns, minVal, maxVal);    // Generate a random puzzle with the specified values.
    
    // Keep track of the best puzzle found so far (and its value).
    Puzzle bestPuzzle = p;
    int bestValue = p.GetValue();
    
    // Keep track of the time so we don't exceed it.
    Timer t;
    t.StartTimer();
    
    // Loop until we hit the time limit.
    while (t.GetElapsedTime() < timelimit-0.1)    // To make sure that we don't exceed the time limit, we stop just before we hit the time limit.
    {
        // Generate a successor of p by randomly changing the value of a random cell
        // (since we are doing a random walk, we just replace p with its successor).
        p = p.GetRandomSuccessor();
        int value = p.GetValue();
        
        // Update the current best solution.
        if (value > bestValue)    // Calling GetValue() for the first time is costly
                                        // since the puzzle has to be evaluated first.
        {
//            std::cout << "value: " << value << ", bestvalue: " << bestValue << std::endl;
            bestValue = value;    // Calling it a second time simply returns the value that was computed before.
            bestPuzzle = p;
        }
    }
    
    return bestPuzzle;
    
    // The following code is not executed in this function. It exists just as an example for getting all the successors of a puzzle.
    vector<Puzzle> successors;
    bestPuzzle.GetAllSuccessors(successors);
}

Puzzle PuzzleGenerator::hillClimb(double timelimit)
{
    Puzzle p(nRows, nColumns, minVal, maxVal);    // Generate a random puzzle with the specified values.
   
    // Keep track of the best puzzle found so far (and its value).
    Puzzle bestPuzzle = p;
    int bestValue = p.GetValue();
    
    // look at all successors from current puzzle
    vector<Puzzle> successors;
    bestPuzzle.GetAllSuccessors(successors);
    
    // Keep track of the time so we don't exceed it.
    Timer t;
    t.StartTimer();
    int count = 0;
    while (t.GetElapsedTime() < timelimit-0.1)    // To make sure that we don't exceed the time limit, we stop just before we hit the time limit.
    {
        // Generate a successor of p by randomly changing the value of a random cell
        // (since we are doing a random walk, we just replace p with its successor)
        if(count < 10){
            p = p.GetClimbingSuccessor(successors, bestValue);
            p.GetSomeSuccessors(successors);
        }
        else
        {
            p = p.GetRandomSuccessor();
            count = 0;
            p.GetSomeSuccessors(successors);
        }
        int value = p.GetValue();
        
        // Update the current best solution.
        if (value > bestValue)    // Calling GetValue() for the first time is costly
                                        // since the puzzle has to be evaluated first.
        {
            bestValue = value;    // Calling it a second time simply returns the value that was computed before.
            bestPuzzle = p;
            bestPuzzle.GetSomeSuccessors(successors);
            count = 0;
        } else if(value == bestValue)
            count++;
    }
    
    return bestPuzzle;
}
bool PuzzleGenerator::ProbabilityGenerator(int bestvalue, int potential)
{
    int delta = potential - bestvalue;
    if(delta >= 0)
        return pCoinFlip(1.0);
    int denom = 100 - bestvalue;
    double Tau = exp(delta/denom);
//        std::cout << "delta: " << delta << ", bestvalue: " << bestvalue << ", potential: " << potential << ", denom: " << denom << ", Tau: " << Tau << std::endl;
        return pCoinFlip(Tau);
}
bool PuzzleGenerator::pCoinFlip(double probability)
{
    //int coin = rand()%10+1;
    int coin = rand()%100+1;
    double prob = probability * 100;
//    std::cout << "probability: " << probability << ", coin: " << coin << ", prob: " << prob << std::endl;
    if(coin <= prob)
        return true;
    return false;
}


// e^(-5/infinity) = 1

// e^(-5/30)

// e^(-5/1)




/*
 bool PuzzleGenerator::ProbabilityGenerator(int bestvalue, int potential)
 {
     int delta = potential - bestvalue;
     
 //    if(delta <= 0)
 //        return pCoinFlip(1.0);
 //        double Tau;
 //            if(bestvalue != 0)
 //                Tau = exp(delta/(100.0/bestvalue));
 //            else
 //                Tau = 100;
 //        std::cout << "delta: " << delta << ", bestvalue: " << bestvalue << ", potential: " << potential << ", Tau: " << Tau << std::endl;
 //        return pCoinFlip(Tau);
     if(delta >= 0)
         return pCoinFlip(1.0);
     int denom;
     if(bestvalue < 0)
         denom = 1000000;
     else denom = 100-bestvalue;
     double Tau = exp(delta/denom);
 //            if(bestvalue != 0)
 //            else
 //                Tau = 100;
 //        std::cout << "delta: " << delta << ", bestvalue: " << bestvalue << ", potential: " << potential << ", denom: " << denom << ", Tau: " << Tau << std::endl;
         return pCoinFlip(Tau);
 }
 */
