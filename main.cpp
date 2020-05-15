#include <iostream>
#include <vector>


using namespace std;

//Function to approximate
double dydx(double x, double y){
    return y;
}

// Compute yn+1 given xn, yn and step size h
double runge_kutte_4th_order(double x, double y, double h){

    double k1 = h * dydx(x,y);
    double k2 = h* dydx(x + (h/2), y + (k1/2));
    double k3 = h* dydx(x + (h/2), y + (k2/2));
    double k4 = h* dydx(x + h, y + k3);

    double y_next = y + (k1 + (2*k2) + (2*k3) + k4)/6.;

    return y_next;
}

int main(){

double x, y, h, y_next;

x = 0;
y = 1;
h = 0.01;

for (int iter = 0; iter< 200000; iter++){

    y_next = runge_kutte_4th_order(x,y,h);
    
    cout<< "yn+1 = " << y_next<< "  yn = " << y << " x = "<< x<< endl;
    // Update x and y for next iteration
    x = x + h;
    y = y_next;

    // Break when x = 1, i.e. y = exp(1) = 2.7182...
    if (x > 1)
        break;
    }
return 0;
}