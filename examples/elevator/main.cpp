#include "fsmlist.hpp"

#include <iostream>

using namespace std;


int main()
{
  fsm_list::start();

  Call call;
  FloorSensor sensor;

  while(1)
  {
    char c;

    cout << "c=Call, f=FloorSensor, a=Alarm, q=Quit ? ";
    cin >> c;
    switch(c) {
    case 'c':
      cout << "Floor ? ";
      cin >> call.floor;
      send_event(call);
      break;
    case 'f':
      cout << "Floor ? ";
      cin >> sensor.floor;
      send_event(sensor);
      break;
    case 'a':
      send_event(Alarm());
      break;
    case 'q':
      cout << "Thanks for playing!" << endl;
      return 0;
    default:
      cout << "Invalid input" << endl;
    };
  }
}
