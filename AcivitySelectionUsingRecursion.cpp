#include<iostream>
#include<algorithm>
using namespace std;
class time{
    public:
    int start;
    int end;
};
bool compareF(time a,time b)
{
    return (a.end<b.end);
}
int main()
{
    cout<<"Enter max no. of activities: \n";
    int n;
    cin>>n;
    time* array= new time[n];
    cout<<"Enter starting and finish times of n activites: \n";
    for(int i=0 ; i<n ; i++)
    {
        cin>>array[i].start;
        cin>>array[i].end;
    }
    sort(array,array+n,compareF);
    
}