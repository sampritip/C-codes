#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
class time{
    public:
     int start;
     int end;
};
bool compareF(time a,time b)
{
    return (a.end < b.end);
}
vector<time> printMaxActivity(time*array, int n)
{
    sort(array,array+n, compareF);
    vector <time> V;
    time temp; temp.start=array[0].start; temp.end= array[0].end;
    V.push_back(temp);
    int j=0;
    for(int i=1 ; i<n ; i++)
    {
        if(array[i].start >= array[j].end)
        {
            temp.start=array[i].start; temp.end= array[i].end;
            V.push_back(temp);
            j=i;
        }
    }
    return V;

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
    vector <time> R=printMaxActivity(array,n);
    cout<<"\nNo. of activites possible are: "<<R.size()<<"\n";
    for(int i=0 ; i<R.size() ; i++)
    {
        cout<<R[i].start<<" "<<R[i].end<<"\n";
    }
}