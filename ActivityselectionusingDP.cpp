#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
class Pair{
    public:
    int start;
    int finish;
};
class Sol{
    public:
    int finish; //last finish time before ith element
    int count;
};
int compareX(Pair job1, Pair job2)
{
    return job1.finish < job2.finish;
}
void selector(vector<Pair> jobs)
{
   sort(jobs.begin(),jobs.end(), compareX);
    vector<Sol> A; //stores number of jobs compatiles before i
    Sol temp;
    temp.finish= jobs[0].finish;
    temp.count=1;
    A.push_back(temp);
    for(int i=1 ; i< jobs.size() ; i++)
    {
        if(jobs[i].start >= A[i-1].finish)
          {
              
              temp.finish=jobs[i].finish;
              temp.count= A[i-1].count +1;
          }
          else
          {
              temp.finish= A[i-1].finish;
              temp.count= A[i-1].count;
          }
          A.push_back(temp);
    }
    for(int i=0 ; i<A.size() ; i++)
     cout<<A[i].count<<" ";
}
int main()
{
    vector <Pair> jobs= { {1,2},{3,4},{0,6},{5,7},{8,9},{5,9}};
    selector(jobs);
    return 0;
}